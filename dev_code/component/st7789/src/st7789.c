/**
 * Title : st7789.c
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Feb 20, 2022
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "trace.h"
#include "st7789.h"
#include "st7789_port.h"
#include "fonts.h"
#include "cmsis_os.h"
#include "stm32l4xx_hal.h"

/* Private define constants -------------------------------------------------------------*/
#define ST7789_WIDTH  240
#define ST7789_HEIGHT 240

#define U16_SIZE     65535
#define RESIZE_VALUE 80

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN  0x10
#define ST7789_SLPOUT 0x11
#define ST7789_PTLON  0x12
#define ST7789_NORON  0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR  0x30
#define ST7789_COLMOD 0x3A
#define ST7789_MADCTL 0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1 0xDA
#define ST7789_RDID2 0xDB
#define ST7789_RDID3 0xDC
#define ST7789_RDID4 0xDD

#define ST7789_COLOR_MODE_16BIT 0x55
#define ST7789_COLOR_MODE_18BIT 0x66

#define ST7789_GATECTRL               0XB7 /* Gate Control */
#define ST7789_DEFAULT_GATECTRL_VALUE 0x35 /*Default value */

#define ST7789_VCOMSET               0xBB /* VCOM setting */
#define ST7789_DEFAULT_VCOMSET_VALUE 0x19 /* 0.725v (default 0.75v for 0x20) */

#define ST7789_LCMCTRL               0xC0 /* LCMCTRL */
#define ST7789_DEFAULT_LCMCTRL_VALUE 0x2C /* Default value */

#define ST7789_VDV_VRH_EN            0xC2 /* VDV and VRH command Enable */
#define ST7789_DEFAULT_VDV_VRH_VALUE 0x01 /* Default value */

#define ST7789_VRHSET               0xC3 /* VRH set */
#define ST7789_DEFAULT_VRHSET_VALUE 0x12 /* +-4.45v (defalut +-4.1v for 0x0B) */

#define ST7789_VDVSET               0xC4 /* VDV set */
#define ST7789_DEFAULT_VDVSET_VALUE 0x20 /* Default value */

#define ST7789_FRAMERATE               0xC6 /* Frame rate control in normal mode */
#define ST7789_DEFAULT_FRAMERATE_VALUE 0x0F /* Default value (60HZ) */

#define ST7789_POWERCTRL           0xD0 /* Power control */
#define ST7789_DEFAULT_POWERCTRL_1 0xA4 /* Default value 1*/
#define ST7789_DEFAULT_POWERCTRL_2 0xA1 /* Default value 2*/

#define ST7789_POSITIVE_VOLT_GAMCTRL 0xE0 /* Positive Voltage Gamma Control */
#define ST7789_NEGATIVE_VOLT_GAMCTRL 0xE1 /* Negative Voltage Gamma Control */

/* Private macros -----------------------------------------------------------------------*/
#define ABS(x) ((x) > 0 ? (x) : -(x))

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
static uint8_t u8XShift;
static uint8_t u8YShift;

/* Private function prototypes declarations   -------------------------------------------*/
static void ST7789_voDrawPixel(uint16_t u16X, uint16_t u16Y, uint16_t u16Color);
static void ST7789_voWriteCommand(uint8_t u8Cmd);
static void ST7789_voWriteData(uint8_t *pu8Buff, uint32_t u32BuffSize);
static void ST7789_voWriteSmallData(uint8_t u8Data);
static void ST7789_voSetAddressWindow(uint16_t u16X0, uint16_t u16Y0, uint16_t u16X1, uint16_t u16Y1);
static void ST7789_voWriteColor(uint16_t u16Color);

/* Private functions definition   -------------------------------------------------------*/
/* Write command to ST7789 controller */
static void ST7789_voWriteCommand(uint8_t u8Cmd)
{
    ST7789_voDcClear();
    ST7789_voTransmit(&u8Cmd, sizeof(u8Cmd), 200);
}

/**
 * Write data to ST7789 controller
 * pu8Buff -> pointer of data buffer
 * u32BuffSize -> size of the data buffer
 */
static void ST7789_voWriteData(uint8_t *pu8Buff, uint32_t u32BuffSize)
{
    uint16_t u16ChunkSize;
    ST7789_voDcSet();
    while (u32BuffSize > 0)
    {
        u16ChunkSize = (u32BuffSize > U16_SIZE) ? U16_SIZE : u32BuffSize;
        ST7789_voTransmit(pu8Buff, u16ChunkSize, 200);
        pu8Buff += u16ChunkSize;
        u32BuffSize -= u16ChunkSize;
    }
}

/**
 * Write data to ST7789 controller, simplify for 8bit data.
 * data -> data to write
 */
static void ST7789_voWriteSmallData(uint8_t u8Data)
{
    ST7789_voDcSet();
    ST7789_voTransmit(&u8Data, sizeof(u8Data), 200);
}

/**
 * Send 16 bit Color.
 * u16Color -> color parameter
 */
static void ST7789_voWriteColor(uint16_t u16Color)
{
    uint8_t u8Data[] = {u16Color >> 8, u16Color & 0xFF};
    ST7789_voWriteData(u8Data, sizeof(u8Data));
}

/**
 * Set address of DisplayWindow
 * u16Xi&u16Yi -> coordinates of window
 */
static void ST7789_voSetAddressWindow(uint16_t u16X0, uint16_t u16Y0, uint16_t u16X1, uint16_t u16Y1)
{
    uint16_t u16XStart = u16X0 + u8XShift, u16XEnd = u16X1 + u8XShift;
    uint16_t u16YStart = u16Y0 + u8YShift, u16YEnd = u16Y1 + u8YShift;

    /* Column Address set */
    ST7789_voWriteCommand(ST7789_CASET);
    {
        uint8_t u8Data[] = {u16XStart >> 8, u16XStart & 0xFF, u16XEnd >> 8, u16XEnd & 0xFF};
        ST7789_voWriteData(u8Data, sizeof(u8Data));
    }

    /* Row Address set */
    ST7789_voWriteCommand(ST7789_RASET);
    {
        uint8_t u8Data[] = {u16YStart >> 8, u16YStart & 0xFF, u16YEnd >> 8, u16YEnd & 0xFF};
        ST7789_voWriteData(u8Data, sizeof(u8Data));
    }

    /* Write to RAM */
    ST7789_voWriteCommand(ST7789_RAMWR);
}

/**
 * Draw a Pixel
 * u16X&u16Y -> coordinate to Draw
 */
static void ST7789_voDrawPixel(uint16_t u16X, uint16_t u16Y, uint16_t u16Color)
{
    if ((u16X < 0) || (u16X >= ST7789_WIDTH) ||
        (u16Y < 0) || (u16Y >= ST7789_HEIGHT))
    {
        return;
    }

    ST7789_voSetAddressWindow(u16X, u16Y, u16X, u16Y);
    ST7789_voWriteColor(u16Color);
}

/* Export functions definition   --------------------------------------------------------*/
tenStatus ST7789_enInit(void)
{
    ST7789_enInitIO();

    ST7789_voRstClear();
    osDelay(25);

    ST7789_voRstSet();
    osDelay(25);

    /*Interface Pixel Format*/
    ST7789_voWriteCommand(ST7789_COLMOD);
    ST7789_voWriteSmallData(ST7789_COLOR_MODE_16BIT); /*16 bit/pixel mode*/

    /* MADCTL(Display Rotation) */
    ST7789_voSetRotation(ST7789_ROTATION1);

    /* Internal LCD Voltage generator settings */
    /* Gate Control */
    ST7789_voWriteCommand(ST7789_GATECTRL);
    ST7789_voWriteSmallData(ST7789_DEFAULT_GATECTRL_VALUE);

    /* VCOM setting */
    ST7789_voWriteCommand(ST7789_VCOMSET);
    ST7789_voWriteSmallData(ST7789_DEFAULT_VCOMSET_VALUE); /* 0.725v (default 0.75v for 0x20) */

    /* LCMCTRL */
    ST7789_voWriteCommand(ST7789_LCMCTRL);
    ST7789_voWriteSmallData(ST7789_DEFAULT_LCMCTRL_VALUE); /* Default value */

    /* VDV and VRH command Enable */
    ST7789_voWriteCommand(ST7789_VDV_VRH_EN);
    ST7789_voWriteSmallData(ST7789_DEFAULT_VDV_VRH_VALUE); /* Default value */

    /* VRH set */
    ST7789_voWriteCommand(ST7789_VRHSET);
    ST7789_voWriteSmallData(ST7789_DEFAULT_VRHSET_VALUE); /* +-4.45v (defalut +-4.1v for 0x0B) */

    /* VDV set */
    ST7789_voWriteCommand(ST7789_VDVSET);
    ST7789_voWriteSmallData(ST7789_DEFAULT_VDVSET_VALUE); /* Default value */

    /* Frame rate control in normal mode */
    ST7789_voWriteCommand(ST7789_FRAMERATE);
    ST7789_voWriteSmallData(ST7789_DEFAULT_FRAMERATE_VALUE); /* Default value (60HZ) */

    /* Power control */
    ST7789_voWriteCommand(ST7789_POWERCTRL);
    ST7789_voWriteSmallData(ST7789_DEFAULT_POWERCTRL_1); /* Default value 1*/
    ST7789_voWriteSmallData(ST7789_DEFAULT_POWERCTRL_2); /* Default value 2*/

    /* Positive Voltage Gamma Control */
    ST7789_voWriteCommand(ST7789_POSITIVE_VOLT_GAMCTRL);
    {
        uint8_t u8Data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
        ST7789_voWriteData(u8Data, sizeof(u8Data));
    }

    /* Negative Voltage Gamma Control */
    ST7789_voWriteCommand(ST7789_NEGATIVE_VOLT_GAMCTRL);
    {
        uint8_t u8Data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
        ST7789_voWriteData(u8Data, sizeof(u8Data));
    }

    /*  Inversion ON */
    ST7789_voWriteCommand(ST7789_INVON);

    /* Out of sleep mode */
    ST7789_voWriteCommand(ST7789_SLPOUT);

    /* Normal Display on */
    ST7789_voWriteCommand(ST7789_NORON);

    /* Main screen turned on */
    ST7789_voWriteCommand(ST7789_DISPON);
    osDelay(50);
    ST7789_voFillColor(WHITE);
    return eSUCCESS;
}

/* Fill the DisplayWindow with single_u16Color */
void ST7789_voFillColor(uint16_t u16Color)
{
    uint16_t u16Index;
    uint16_t u16Index2;
    ST7789_voSetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
    for (u16Index = 0; u16Index < ST7789_WIDTH; u16Index++)
    {
        for (u16Index2 = 0; u16Index2 < ST7789_HEIGHT; u16Index2++)
        {
            ST7789_voWriteColor(u16Color);
        }
    }
}

/**
 * Set the rotation direction of the display
 * u8Mm -> rotation parameter(please refer it in st7789.h)
 */
void ST7789_voSetRotation(tenOrder enRotation)
{
    ST7789_voWriteCommand(ST7789_MADCTL);
    switch (enRotation)
    {
        case ST7789_ROTATION1:
            u8XShift = 0;
            u8YShift = RESIZE_VALUE;
            ST7789_voWriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
            break;
        case ST7789_ROTATION2:
            u8XShift = RESIZE_VALUE;
            u8YShift = 0;
            ST7789_voWriteSmallData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
            break;
        case ST7789_ROTATION3:
            u8XShift = 0;
            u8YShift = 0;
            ST7789_voWriteSmallData(ST7789_MADCTL_RGB);
            break;
        case ST7789_ROTATION4:
            u8XShift = 0;
            u8YShift = 0;
            ST7789_voWriteSmallData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
            break;
        default:
            break;
    }
}

/**
 * Fill an Area with singleu16Color
 * u16XStart&u16YStart -> coordinate of the start point
 * u16XEnd&u16YEnd -> coordinate of the end point
 */
void ST7789_voFill(uint16_t u16XStart, uint16_t u16YStart, uint16_t u16XEnd, uint16_t u16YEnd, uint16_t u16Color)
{
    uint16_t u16Index, u16Index2;
    if ((u16XEnd < 0) || (u16XEnd >= ST7789_WIDTH) ||
        (u16YEnd < 0) || (u16YEnd >= ST7789_HEIGHT))
    {
        return;
    }

    ST7789_voSetAddressWindow(u16XStart, u16YStart, u16XEnd, u16YEnd);

    for (u16Index = u16YStart; u16Index <= u16YEnd; u16Index++)
    {
        for (u16Index2 = u16XStart; u16Index2 <= u16XEnd; u16Index2++)
        {
            ST7789_voWriteColor(u16Color);
        }
    }
}

/**
 * draw a line with singleu16Color
 * u16X0&u16Y0 -> coordinate of the start point
 * u16X1&u16Y1 -> coordinate of the end point
 * u16Color ->u16Color of the line to Draw
 */
void ST7789_voDrawLine(uint16_t u16X0, uint16_t u16Y0, uint16_t u16X1, uint16_t u16Y1,
                       uint16_t u16Color)
{
    uint16_t u16Swap;
    uint16_t u16Steep = ABS(u16Y1 - u16Y0) > ABS(u16X1 - u16X0);
    if (u16Steep)
    {
        u16Swap = u16X0;
        u16X0   = u16Y0;
        u16Y0   = u16Swap;

        u16Swap = u16X1;
        u16X1   = u16Y1;
        u16Y1   = u16Swap;
    }

    if (u16X0 > u16X1)
    {
        u16Swap = u16X0;
        u16X0   = u16X1;
        u16X1   = u16Swap;

        u16Swap = u16Y0;
        u16Y0   = u16Y1;
        u16Y1   = u16Swap;
    }

    int16_t i16Dx, i16Dy;
    i16Dx = u16X1 - u16X0;
    i16Dy = ABS(u16Y1 - u16Y0);

    int16_t i16Err = i16Dx / 2;
    int16_t i16yStep;

    if (u16Y0 < u16Y1)
    {
        i16yStep = 1;
    }
    else
    {
        i16yStep = -1;
    }

    for (; u16X0 <= u16X1; u16X0++)
    {
        if (u16Steep)
        {
            ST7789_voDrawPixel(u16Y0, u16X0, u16Color);
        }
        else
        {
            ST7789_voDrawPixel(u16X0, u16Y0, u16Color);
        }
        i16Err -= i16Dy;
        if (i16Err < 0)
        {
            u16Y0 += i16yStep;
            i16Err += i16Dx;
        }
    }
}

/**
 * Draw a Rectangle with singleu16Color
 * xi&yi -> 2 coordinates of 2 top points
 * u16Color ->u16Color of the Rectangle line
 */
void ST7789_voDrawRectangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16Color)
{
    ST7789_voDrawLine(u16X1, u16Y1, u16X2, u16Y1, u16Color);
    ST7789_voDrawLine(u16X1, u16Y1, u16X1, u16Y2, u16Color);
    ST7789_voDrawLine(u16X1, u16Y2, u16X2, u16Y2, u16Color);
    ST7789_voDrawLine(u16X2, u16Y1, u16X2, u16Y2, u16Color);
}

/**
 * Draw a circle with singleu16Color
 * i16X0&i16Y0 -> coordinate of circle center
 * i16R -> radius of circle
 * 16Color ->u16Color of circle line
 */
void ST7789_voDrawCircle(uint16_t u16X0, uint16_t u16Y0, uint8_t u8R, uint16_t u16Color)
{
    int16_t i16F        = 1 - u8R;
    int16_t i16i16DdF_x = 1;
    int16_t i16DdFy     = -2 * u8R;
    int16_t i16X        = 0;
    int16_t i16Y        = u8R;

    ST7789_voDrawPixel(u16X0, u16Y0 + u8R, u16Color);
    ST7789_voDrawPixel(u16X0, u16Y0 - u8R, u16Color);
    ST7789_voDrawPixel(u16X0 + u8R, u16Y0, u16Color);
    ST7789_voDrawPixel(u16X0 - u8R, u16Y0, u16Color);

    while (i16X < i16Y)
    {
        if (i16F >= 0)
        {
            i16Y--;
            i16DdFy += 2;
            i16F += i16DdFy;
        }
        i16X++;
        i16i16DdF_x += 2;
        i16F += i16i16DdF_x;

        ST7789_voDrawPixel(u16X0 + i16X, u16Y0 + i16Y, u16Color);
        ST7789_voDrawPixel(u16X0 - i16X, u16Y0 + i16Y, u16Color);
        ST7789_voDrawPixel(u16X0 + i16X, u16Y0 - i16Y, u16Color);
        ST7789_voDrawPixel(u16X0 - i16X, u16Y0 - i16Y, u16Color);

        ST7789_voDrawPixel(u16X0 + i16Y, u16Y0 + i16X, u16Color);
        ST7789_voDrawPixel(u16X0 - i16Y, u16Y0 + i16X, u16Color);
        ST7789_voDrawPixel(u16X0 + i16Y, u16Y0 - i16X, u16Color);
        ST7789_voDrawPixel(u16X0 - i16Y, u16Y0 - i16X, u16Color);
    }
}

/**
 * Draw an Image on the screen
 * U16X&u16Y -> start point of the Image
 * u16W&u16H -> u8Width & u8Height of the Image to Draw
 * pu16Data -> pointer of the Image array
 */
void ST7789_voDrawImage(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H, const uint16_t *pu16Data)
{
    if ((u16X >= ST7789_WIDTH) || (u16Y >= ST7789_HEIGHT))
    {
        return;
    }

    if ((u16X + u16W - 1) >= ST7789_WIDTH)
    {
        return;
    }

    if ((u16Y + u16H - 1) >= ST7789_HEIGHT)
    {
        return;
    }

    ST7789_voSetAddressWindow(u16X, u16Y, u16X + u16W - 1, u16Y + u16H - 1);
    ST7789_voWriteData((uint8_t *)pu16Data, sizeof(uint16_t) * u16W * u16H);
}

/**
 * Write a char
 * u16X&u16Y -> cursor of the start point.
 * u8Ch -> char to write
 * stFont -> fontstyle of the string
 * u16Color ->u16Color of the char
 * u16BackGroundColor -> backgroundu16Color of the char
 */
void ST7789_voWriteChar(uint16_t u16X, uint16_t u16Y, uint8_t u8Ch, tstFontDef stFont, uint16_t u16Color, uint16_t u16BackGroundColor)
{
    uint32_t u32index, u32b, u32index2;
    ST7789_voSetAddressWindow(u16X, u16Y, u16X + stFont.u8Width - 1, u16Y + stFont.u8Height - 1);

    for (u32index = 0; u32index < stFont.u8Height; u32index++)
    {
        u32b = stFont.pu16Data[(u8Ch - 32) * stFont.u8Height + u32index];
        for (u32index2 = 0; u32index2 < stFont.u8Width; u32index2++)
        {
            if ((u32b << u32index2) & 0x8000)
            {
                ST7789_voWriteColor(u16Color);
            }
            else
            {
                ST7789_voWriteColor(u16BackGroundColor);
            }
        }
    }
}

/**
 * Write a string
 * u16X & u16Y->cursor of the start point
 * pcStr->string to write
 * stFont->fontstyle of the string
 * u16Color->u16Color of the string
 * u16BackGroundColor->backgroundu16Color of the string
 */
void ST7789_voWriteString(uint16_t u16X, uint16_t u16Y, const char *pcStr, tstFontDef stFont, uint16_t u16Color, uint16_t u16BackGroundColor)
{
    while (*pcStr)
    {
        if (u16X + stFont.u8Width >= ST7789_WIDTH)
        {
            u16X = 0;
            u16Y += stFont.u8Height;
            if (u16Y + stFont.u8Height >= ST7789_HEIGHT)
            {
                break;
            }

            if (*pcStr == ' ')
            {
                pcStr++;
                continue;
            }
        }
        ST7789_voWriteChar(u16X, u16Y, *pcStr, stFont, u16Color, u16BackGroundColor);
        u16X += stFont.u8Width;
        pcStr++;
    }
}

/**
 * Draw a filled Rectangle with singleu16Color
 * u16X & u16Y->coordinates of the starting point
 * u16W & u16H->u8Width &u8Height of the Rectangle
 * u16Color->u16Color of the Rectangle
 */
void ST7789_voDrawFilledRectangle(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H, uint16_t u16Color)
{
    uint8_t u8Index;

    /* Check input parameters */
    if (u16X >= ST7789_WIDTH ||
        u16Y >= ST7789_HEIGHT)
    {
        /* Return error */
        return;
    }

    /* Check u8Width and u8Height */
    if ((u16X + u16W) >= ST7789_WIDTH)
    {
        u16W = ST7789_WIDTH - u16X;
    }
    if ((u16Y + u16H) >= ST7789_HEIGHT)
    {
        u16H = ST7789_HEIGHT - u16Y;
    }

    /* Draw lines */
    for (u8Index = 0; u8Index <= u16H; u8Index++)
    {
        /* Draw lines */
        ST7789_voDrawLine(u16X, u16Y + u8Index, u16X + u16W, u16Y + u8Index, u16Color);
    }
}

/**
 * Draw a Triangle with singleu16Color
 * u16xi & u16yi->3 coordinates of 3 top points.
 * u16Color->color of the line
 */
void ST7789_voDrawTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3, uint16_t u16Color)
{
    /* Draw lines */
    ST7789_voDrawLine(u16X1, u16Y1, u16X2, u16Y2, u16Color);
    ST7789_voDrawLine(u16X2, u16Y2, u16X3, u16Y3, u16Color);
    ST7789_voDrawLine(u16X3, u16Y3, u16X1, u16Y1, u16Color);
}

/**
 * Draw a filled Triangle with singleu16Color
 * u16X1 & u16Y1->3 coordinates of 3 top points.
 * u16Color->color of the triangle
 */
void ST7789_voDrawFilledTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3, uint16_t u16Color)
{
    int16_t i16Deltax = 0, i16Deltay = 0, i16X = 0, i16Y = 0, i16Xinc1 = 0, i16Xinc2 = 0,
            i16Yinc1 = 0, i16Yinc2 = 0, i16Den = 0, i16Num = 0, i16NumAdd = 0, i16NumPixels = 0,
            i16CurPixel = 0;

    i16Deltax = ABS(u16X2 - u16X1);
    i16Deltay = ABS(u16Y2 - u16Y1);
    i16X      = u16X1;
    i16Y      = u16Y1;

    if (u16X2 >= u16X1)
    {
        i16Xinc1 = 1;
        i16Xinc2 = 1;
    }
    else
    {
        i16Xinc1 = -1;
        i16Xinc2 = -1;
    }

    if (u16Y2 >= u16Y1)
    {
        i16Yinc1 = 1;
        i16Yinc2 = 1;
    }
    else
    {
        i16Yinc1 = -1;
        i16Yinc2 = -1;
    }

    if (i16Deltax >= i16Deltay)
    {
        i16Xinc1     = 0;
        i16Yinc2     = 0;
        i16Den       = i16Deltax;
        i16Num       = i16Deltax / 2;
        i16NumAdd    = i16Deltay;
        i16NumPixels = i16Deltax;
    }
    else
    {
        i16Xinc2     = 0;
        i16Yinc1     = 0;
        i16Den       = i16Deltay;
        i16Num       = i16Deltay / 2;
        i16NumAdd    = i16Deltax;
        i16NumPixels = i16Deltay;
    }

    for (i16CurPixel = 0; i16CurPixel <= i16NumPixels; i16CurPixel++)
    {
        ST7789_voDrawLine(i16X, i16Y, u16X3, u16Y3, u16Color);

        i16Num += i16NumAdd;
        if (i16Num >= i16Den)
        {
            i16Num -= i16Den;
            i16X += i16Xinc1;
            i16Y += i16Yinc1;
        }
        i16X += i16Xinc2;
        i16Y += i16Yinc2;
    }
}

/**
 * Draw a Filled circle with singleu16Color
 * i16X0 & i16Y0->coordinate of circle center
 * i16R->radius of circle
 * u16Color->u16Color of circle
 */
void ST7789_voDrawFilledCircle(int16_t i16X0, int16_t i16Y0, int16_t i16R, uint16_t u16Color)
{
    int16_t i16F    = 1 - i16R;
    int16_t i16DdFx = 1;
    int16_t i16DdFy = -2 * i16R;
    int16_t i16X    = 0;
    int16_t i16Y    = i16R;

    ST7789_voDrawPixel(i16X0, i16Y0 + i16R, u16Color);
    ST7789_voDrawPixel(i16X0, i16Y0 - i16R, u16Color);
    ST7789_voDrawPixel(i16X0 + i16R, i16Y0, u16Color);
    ST7789_voDrawPixel(i16X0 - i16R, i16Y0, u16Color);
    ST7789_voDrawLine(i16X0 - i16R, i16Y0, i16X0 + i16R, i16Y0, u16Color);

    while (i16X < i16Y)
    {
        if (i16F >= 0)
        {
            i16Y--;
            i16DdFy += 2;
            i16F += i16DdFy;
        }
        i16X++;
        i16DdFx += 2;
        i16F += i16DdFx;

        ST7789_voDrawLine(i16X0 - i16X, i16Y0 + i16Y, i16X0 + i16X, i16Y0 + i16Y, u16Color);
        ST7789_voDrawLine(i16X0 + i16X, i16Y0 - i16Y, i16X0 - i16X, i16Y0 - i16Y, u16Color);

        ST7789_voDrawLine(i16X0 + i16Y, i16Y0 + i16X, i16X0 - i16Y, i16Y0 + i16X, u16Color);
        ST7789_voDrawLine(i16X0 + i16Y, i16Y0 - i16X, i16X0 - i16Y, i16Y0 - i16X, u16Color);
    }
}
