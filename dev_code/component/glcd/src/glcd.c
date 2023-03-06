/*
 * Title : glcd.c
 * Copyright : HCL
 * Author : < hoang.thong >
 * Creation Date : 14/02/2023
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "cmsis_os2.h"
#include "common.h"
#include "glcd.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/
#define SCLK_PORT GPIOH
#define SCLK_PIN  GPIO_PIN_2

#define CS_PORT GPIOI
#define CS_PIN  GPIO_PIN_3

#define SID_PORT GPIOD
#define SID_PIN  GPIO_PIN_3

#define RST_PORT GPIOB
#define RST_PIN  GPIO_PIN_7

#define NUM_OF_ROWS    64
#define HALF_OF_ROWS   (NUM_OF_ROWS / 2)
#define NUM_OF_COLUMNS 128

#define WIDTH_OF_FONT_SMALL  3
#define HEIGHT_OF_FONT_SMALL 5

#define WIDTH_OF_FONT_MEDIUM  5
#define HEIGHT_OF_FONT_MEDIUM 7
#define INDEX_OF_FONT_MEDIUM  32

#define WIDTH_OF_FONT_LARGE  7
#define HEIGHT_OF_FONT_LARGE 9

#define DISTANCE_PIXEL_OF_SMALL  4
#define DISTANCE_PIXEL_OF_MEDIUM 6
#define DISTANCE_PIXEL_OF_LARGE  10

#define NUM_BIT_OF_BYTE 8
#define NUM_ZONE_OF_ROW 8

#define EXTENDED_INSTRUCTION_ON 0x34
#define GRAPHIC_MODE_ON         0x36
#define DISPLAY_CONTROL_OFF     0x08
#define SYNC_BIT                0xf8
#define SYNC_4_BIT              0xf0
#define DISPLAY_CONTROL_ON      0x0C
#define ADDRESS_BASE            0x80
#define ENTRY_MODE_SET          0x06
#define LINE_BASE_DDRAM         0x88
#define CLEAR_SCREEN            0x01
#define CLEAR_VALUE             0x00
#define END_INIT                0x02
#define MODE_8_BIT              0x30

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
static uint8_t        su8Image[(NUM_OF_COLUMNS * NUM_OF_ROWS) / NUM_BIT_OF_BYTE];
static tenGraphicMode enGraphicCheck = eDISABLE;
TIM_HandleTypeDef     htim2;

/* Private function prototypes declarations   -------------------------------------------*/
static tenStatus GLCD_enInitTIM2(void);
static tenStatus GLCD_enInitIO(void);
static void      GLCD_voSendByteSpi(uint8_t u8byte);
static void      GLCD_voSendCmd(uint8_t u8Cmd);
static void      GLCD_voSendData(uint8_t u8Data);
static void      GLCD_voDelayUs(uint16_t u16Delay_us);
static void      GLCD_voSetGraphicMode(tenGraphicMode enGraphicMode);

/* Private functions definition   -------------------------------------------------------*/
static tenStatus GLCD_enInitTIM2(void)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0};
    htim2.Instance                             = TIM2;
    htim2.Init.Prescaler                       = 80 - 1;
    htim2.Init.CounterMode                     = TIM_COUNTERMODE_UP;
    htim2.Init.Period                          = 0xffff - 1;
    htim2.Init.ClockDivision                   = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload               = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        return eFAIL;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        return eFAIL;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        return eFAIL;
    }
    return eSUCCESS;
}

static tenStatus GLCD_enInitIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SID_PORT, SID_PIN, GPIO_PIN_RESET);

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);

    /* Configure GPIO pin : CS_PIN */
    GPIO_InitStruct.Pin   = CS_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CS_PORT, &GPIO_InitStruct);

    /* Configure GPIO pin : RST_PIN */
    GPIO_InitStruct.Pin   = RST_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RST_PORT, &GPIO_InitStruct);

    /* Configure GPIO pin : SID_PIN */
    GPIO_InitStruct.Pin   = SID_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SID_PORT, &GPIO_InitStruct);

    /* Configure GPIO pin : SCLK_PIN */
    GPIO_InitStruct.Pin   = SCLK_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SCLK_PORT, &GPIO_InitStruct);

    return eSUCCESS;
}
static void GLCD_voSendByteSpi(uint8_t u8Byte)
{
    for (uint8_t i = 0; i < NUM_BIT_OF_BYTE; i++)
    {
        if ((u8Byte << i) & ADDRESS_BASE)
        {
            /* SID = 1 or MOSI */
            HAL_GPIO_WritePin(SID_PORT, SID_PIN, GPIO_PIN_SET);
        }
        else
        {
            /* SID = 0 */
            HAL_GPIO_WritePin(SID_PORT, SID_PIN, GPIO_PIN_RESET);
        }

        /* SCLK = 0 or SCK */
        HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);

        /* SCLK = 1 */
        HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET);
    }
}

static void GLCD_voSendCmd(uint8_t u8Cmd)
{
    /* Pull the CS high */
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    /* Send the SYNC + RS(0) */
    GLCD_voSendByteSpi(SYNC_BIT + (0 << 1));

    /* Send the higher nibble first */
    GLCD_voSendByteSpi(u8Cmd & SYNC_4_BIT);

    /* Send the lower nibble */
    GLCD_voSendByteSpi((u8Cmd << 4) & SYNC_4_BIT);

    /* Delay 50us */
    GLCD_voDelayUs(50);

    /* Pull the CS low */
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

static void GLCD_voSendData(uint8_t u8Data)
{
    /* Pull the CS high */
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    /* Send the SYNC + RS(1) */
    GLCD_voSendByteSpi(SYNC_BIT + (1 << 1));

    /* Send the higher nibble first */
    GLCD_voSendByteSpi(u8Data & SYNC_4_BIT);

    /* Send the lower nibble */
    GLCD_voSendByteSpi((u8Data << 4) & SYNC_4_BIT);

    GLCD_voDelayUs(50);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

/* Delay with microseconds */
static void GLCD_voDelayUs(uint16_t u16Delay_us)
{
    /* Reset the counter */
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while ((__HAL_TIM_GET_COUNTER(&htim2)) < u16Delay_us)
    {
        /* Wait for the delay to complete */
        ;
    }
}

static void GLCD_voSetGraphicMode(tenGraphicMode enGraphicMode)
{
    /* 8bit mode */
    GLCD_voSendCmd(MODE_8_BIT);
    osDelay(1);

    if (enGraphicMode == eENABLE)
    {
        /* Switch to Extended instructions */
        GLCD_voSendCmd(EXTENDED_INSTRUCTION_ON);
        osDelay(1);

        /* Enable graphics */
        GLCD_voSendCmd(GRAPHIC_MODE_ON);
        osDelay(1);

        /* Update the variable */
        enGraphicCheck = eENABLE;
    }
    else
    {
        /* Update the variable */
        enGraphicCheck = eDISABLE;
    }
}

/* Export functions definition   --------------------------------------------------------*/
tenStatus GLCD_enInit(void)
{
    /* Delay init */
    GLCD_enInitTIM2();
    HAL_TIM_Base_Start(&htim2);

    /* Init IO interface */
    if (GLCD_enInitIO() != eSUCCESS)
    {
        return eFAIL;
    }

    /* Set RESET = 0 then RESET = 1 */
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
    osDelay(1);

    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
    osDelay(1);

    /* 8bit mode */
    GLCD_voSendCmd(MODE_8_BIT);
    GLCD_voDelayUs(110);

    GLCD_voSendCmd(MODE_8_BIT);
    GLCD_voDelayUs(40);

    /* D = 0, C = 0, B = 0 */
    GLCD_voSendCmd(DISPLAY_CONTROL_OFF);
    GLCD_voDelayUs(110);

    /* Clear screen */
    GLCD_voSendCmd(CLEAR_SCREEN);
    osDelay(12);

    /* Cursor increment right no shift */
    GLCD_voSendCmd(ENTRY_MODE_SET);
    osDelay(1);

    /* D=1, C=0, B=0 */
    GLCD_voSendCmd(DISPLAY_CONTROL_ON);
    osDelay(1);

    /* Return to home */
    GLCD_voSendCmd(END_INIT);
    osDelay(1);

    /* Turn on Graphic mode */
    GLCD_voSetGraphicMode(eENABLE);

    return eSUCCESS;
}

void GLCD_voSetPixel(uint8_t u8X, uint8_t u8Y)
{
    uint8_t u8StartRow    = 0;
    uint8_t u8StartColumn = 0;
    uint8_t u8EndRow      = 0;
    uint8_t u8EndColumn   = 0;

    if (u8Y < NUM_OF_ROWS && u8X < NUM_OF_COLUMNS)
    {
        /* Identify the pixel */
        uint8_t* p = su8Image + ((u8Y * (NUM_OF_COLUMNS / NUM_ZONE_OF_ROW)) + (u8X / NUM_BIT_OF_BYTE));

        /* Set the value for pixel */
        *p |= 0x80u >> (u8X % NUM_BIT_OF_BYTE);

        /* Change the dirty rectangle to account for a pixel being dirty (we assume it was changed) */
        if (u8StartRow > u8Y)
        {
            u8StartRow = u8Y;
        }
        if (u8EndRow <= u8Y)
        {
            u8EndRow = u8Y + 1;
        }
        if (u8StartColumn > u8X)
        {
            u8StartColumn = u8X;
        }
        if (u8EndColumn <= u8X)
        {
            u8EndColumn = u8X + 1;
        }
    }
}

void GLCD_voClearPixel(uint8_t u8X, uint8_t u8Y)
{
    uint8_t u8StartRow    = 0;
    uint8_t u8StartColumn = 0;
    uint8_t u8EndRow      = 0;
    uint8_t u8EndColumn   = 0;

    if (u8Y < NUM_OF_ROWS && u8X < NUM_OF_COLUMNS)
    {
        /* Identify the pixel */
        uint8_t* p = su8Image + ((u8Y * (NUM_OF_COLUMNS / NUM_ZONE_OF_ROW)) + (u8X / NUM_BIT_OF_BYTE));

        /* Clear the value for pixel */
        *p &= ~(0x80u >> (u8X % NUM_BIT_OF_BYTE));

        /* Change the dirty rectangle to account for a pixel being dirty (we assume it was changed) */
        if (u8StartRow > u8Y)
        {
            u8StartRow = u8Y;
        }
        if (u8EndRow <= u8Y)
        {
            u8EndRow = u8Y + 1;
        }
        if (u8StartColumn > u8X)
        {
            u8StartColumn = u8X;
        }
        if (u8EndColumn <= u8X)
        {
            u8EndColumn = u8X + 1;
        }
    }
}

void GLCD_voDisplayString(uint8_t u8X, uint8_t u8Y, char* cString, const tstGlcdDislayFont* enFont)
{
    uint8_t u8Char;
    while (*cString)
    {
        /* Check Horizontal */
        if (enFont->enPosition == eHorizontal)
        {
            for (uint8_t u8IndexY = 0; u8IndexY < enFont->u8Height; u8IndexY++)
            {
                /* Get position to display */
                u8Char = *(enFont->pu8Font + u8IndexY + enFont->u8Height * (*cString - '/'));
                trace_line();
                trace("u8Char: %02X\r\n", u8Char);
                for (uint8_t u8IndexX = 0; u8IndexX < enFont->u8Width; u8IndexX++)
                {
                    /* Left Shift */
                    if (u8Char & (0x01 << u8IndexX))
                    {
                        GLCD_voSetPixel(u8X + u8IndexX, u8Y + u8IndexY);
                    }
                    else
                    {
                        GLCD_voClearPixel(u8X + u8IndexX, u8Y + u8IndexY);
                    }
                }
            }
            cString++;
            u8X = u8X + enFont->u8Width + 1;
        }
        else
        {
            /* Check Vertical */
            for (uint8_t u8IndexX = 0; u8IndexX < enFont->u8Width; u8IndexX++)
            {
                /* Get position to display */
                u8Char = *(enFont->pu8Font + u8IndexX + enFont->u8Width * (*cString - 32));
                for (uint8_t u8IndexY = 0; u8IndexY < enFont->u8Height; u8IndexY++)
                {
                    if (u8Char & (1 << ((enFont->u8Height - 1) - u8IndexY)))
                    {
                        GLCD_voSetPixel(u8X + u8IndexX, u8Y + ((enFont->u8Height - 1) - u8IndexY));
                    }
                    else
                    {
                        GLCD_voClearPixel(u8X + u8IndexX, u8Y + ((enFont->u8Height - 1) - u8IndexY));
                    }
                }
            }
            cString++;
            u8X = u8X + enFont->u8Width + 1;
        }
    }
}

void GLCD_voDisplayImage(uint8_t u8X, uint8_t u8Y, const uint8_t* u8Image, uint8_t u8Width, uint8_t u8Height)
{
    /* Find byte per line */
    uint8_t u8Byte = u8Width / NUM_BIT_OF_BYTE;
    for (uint32_t i = 0; i < (u8Width * u8Height) / NUM_BIT_OF_BYTE; i++)
    {
        for (uint8_t j = 0; j < NUM_BIT_OF_BYTE; j++)
        {
            /* Read bit from array byte */
            if (*u8Image & (0x80 >> j))
            {
                GLCD_voSetPixel((u8X + (i % u8Byte) * NUM_BIT_OF_BYTE + j), (u8Y + i / u8Byte));
            }
            else
            {
                GLCD_voClearPixel((u8X + (i % u8Byte) * NUM_BIT_OF_BYTE + j), (u8Y + i / u8Byte));
            }
        }
        u8Image++;
    }
}

void GLCD_voUpdate(void)
{
    for (uint8_t u8Y = 0; u8Y < NUM_OF_ROWS; u8Y++)
    {
        for (uint8_t u8X = 0; u8X < NUM_ZONE_OF_ROW; u8X++)
        {
            /* Draws top half of the screen.
             In extended instruction mode, vertical and horizontal coordinates must be specified before sending data in. */
            if (u8Y < HALF_OF_ROWS)
            {
                /* Vertical coordinate of the screen is specified first. (0-31) */
                GLCD_voSendCmd(ADDRESS_BASE | u8Y);

                /* Then horizontal coordinate of the screen is specified. (0-8) */
                GLCD_voSendCmd(ADDRESS_BASE | u8X);
            }
            else
            {
                /* Vertical coordinate of the screen is specified first. (32-63) */
                GLCD_voSendCmd(ADDRESS_BASE | (u8Y - HALF_OF_ROWS));

                /* Then horizontal coordinate of the screen is specified. (0-8) */
                GLCD_voSendCmd(LINE_BASE_DDRAM | u8X);
            }

            /* Data to the upper byte is sent to the coordinate. */
            GLCD_voSendData(su8Image[2 * u8X + 16 * u8Y]);

            /* Data to the lower byte is sent to the coordinate. */
            GLCD_voSendData(su8Image[2 * u8X + 1 + 16 * u8Y]);
        }
    }
}

void GLCD_voClearScreen(void)
{
    /* if the graphic mode is set */
    if (enGraphicCheck == eENABLE)
    {
        uint8_t u8X, u8Y;
        for (u8Y = 0; u8Y < NUM_OF_ROWS; u8Y++)
        {
            if (u8Y < HALF_OF_ROWS)
            {
                GLCD_voSendCmd(ADDRESS_BASE | u8Y);
                GLCD_voSendCmd(ADDRESS_BASE);
            }
            else
            {
                GLCD_voSendCmd(ADDRESS_BASE | (u8Y - HALF_OF_ROWS));
                GLCD_voSendCmd(LINE_BASE_DDRAM);
            }

            /* Send data to Rows */
            for (u8X = 0; u8X < NUM_BIT_OF_BYTE; u8X++)
            {
                GLCD_voSendData(0);
                GLCD_voSendData(0);
            }
        }
    }
    else
    {
        /* Clear the display using command */
        GLCD_voSendCmd(0x01);
        osDelay(2);
    }

    /* Clear su8Image */
    for (uint16_t i = 0; i < (NUM_OF_COLUMNS * NUM_OF_ROWS) / NUM_BIT_OF_BYTE; i++)
    {
        su8Image[i] = CLEAR_VALUE;
    }
}

/* Draw a line, start point (X0, Y0), end point (X1, Y1) */
void GLCD_voDrawLine(uint8_t u8X0, uint8_t u8Y0, uint8_t u8X1, uint8_t u8Y1)
{
    int16_t i16Dx  = (u8X1 >= u8X0) ? u8X1 - u8X0 : u8X0 - u8X1;
    int16_t i16Dy  = (u8Y1 >= u8Y0) ? u8Y1 - u8Y0 : u8Y0 - u8Y1;
    int16_t i16Sx  = (u8X0 < u8X1) ? 1 : -1;
    int16_t i16Sy  = (u8Y0 < u8Y1) ? 1 : -1;
    int16_t i16Err = i16Dx - i16Dy;

    for (;;)
    {
        GLCD_voSetPixel(u8X0, u8Y0);
        if (u8X0 == u8X1 && u8Y0 == u8Y1)
        {
            break;
        }
        int16_t i16E2 = i16Err + i16Err;
        if (i16E2 > -i16Dy)
        {
            i16Err -= i16Dy;
            u8X0 += i16Sx;
        }
        if (i16E2 < i16Dx)
        {
            i16Err += i16Dx;
            u8Y0 += i16Sy;
        }
    }
}

/* Draw rectangle, start point (x,y), w -> width, h -> height */
void GLCD_voDrawRectangle(uint16_t u8X, uint16_t u8Y, uint16_t u8W, uint16_t u8H)
{
    /* Check input parameters */
    if (
        u8X >= NUM_OF_COLUMNS ||
        u8Y >= NUM_OF_ROWS)
    {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((u8X + u8W) >= NUM_OF_COLUMNS)
    {
        u8W = NUM_OF_COLUMNS - u8X;
    }
    if ((u8Y + u8H) >= NUM_OF_ROWS)
    {
        u8H = NUM_OF_ROWS - u8Y;
    }

    /* Draw 4 lines: Top, Bottom, Left, Right */
    GLCD_voDrawLine(u8X, u8Y, u8X + u8W, u8Y);
    GLCD_voDrawLine(u8X, u8Y + u8H, u8X + u8W, u8Y + u8H);
    GLCD_voDrawLine(u8X, u8Y, u8X, u8Y + u8H);
    GLCD_voDrawLine(u8X + u8W, u8Y, u8X + u8W, u8Y + u8H);
}

/* Draw filled rectangle, Start point (x,y), w -> width, h -> height */
void GLCD_voDrawFilledRectangle(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H)
{
    int8_t i;

    /* Check input parameters */
    if (
        u16X >= NUM_OF_COLUMNS ||
        u16Y >= NUM_OF_ROWS)
    {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((u16X + u16W) >= NUM_OF_COLUMNS)
    {
        u16W = NUM_OF_COLUMNS - u16X;
    }
    if ((u16Y + u16H) >= NUM_OF_ROWS)
    {
        u16H = NUM_OF_ROWS - u16Y;
    }

    /* Draw lines */
    for (i = 0; i <= u16H; i++)
    {
        /* Draw lines */
        GLCD_voDrawLine(u16X, u16Y + i, u16X + u16W, u16Y + i);
    }
}

/* Draw circle, centre (x0,y0), radius = radius */
void GLCD_voDrawCircle(uint8_t u8X0, uint8_t u8Y0, uint8_t u8Radius)
{
    int16_t i16F     = 1 - (int16_t)u8Radius;
    int16_t i16ddF_x = 1;

    int16_t i16ddF_y = -2 * (int16_t)u8Radius;
    int16_t i16X     = 0;

    GLCD_voSetPixel(u8X0, u8Y0 + u8Radius);
    GLCD_voSetPixel(u8X0, u8Y0 - u8Radius);
    GLCD_voSetPixel(u8X0 + u8Radius, u8Y0);
    GLCD_voSetPixel(u8X0 - u8Radius, u8Y0);

    int16_t i16Y = u8Radius;
    while (i16X < i16Y)
    {
        if (i16F >= 0)
        {
            i16Y--;
            i16ddF_y += 2;
            i16F += i16ddF_y;
        }
        i16X++;
        i16ddF_x += 2;
        i16F += i16ddF_x;
        GLCD_voSetPixel(u8X0 + i16X, u8Y0 + i16Y);
        GLCD_voSetPixel(u8X0 - i16X, u8Y0 + i16Y);
        GLCD_voSetPixel(u8X0 + i16X, u8Y0 - i16Y);
        GLCD_voSetPixel(u8X0 - i16X, u8Y0 - i16Y);
        GLCD_voSetPixel(u8X0 + i16Y, u8Y0 + i16X);
        GLCD_voSetPixel(u8X0 - i16Y, u8Y0 + i16X);
        GLCD_voSetPixel(u8X0 + i16Y, u8Y0 - i16X);
        GLCD_voSetPixel(u8X0 - i16Y, u8Y0 - i16X);
    }
}

/* Draw Filled Circle */
void GLCD_voDrawFilledCircle(int16_t u16X0, int16_t i16Y0, int16_t i16R)
{
    int16_t u16F     = 1 - i16R;
    int16_t u16ddF_x = 1;
    int16_t u16ddF_y = -2 * i16R;
    int16_t u16X     = 0;
    int16_t i16y     = i16R;

    GLCD_voSetPixel(u16X0, i16Y0 + i16R);
    GLCD_voSetPixel(u16X0, i16Y0 - i16R);
    GLCD_voSetPixel(u16X0 + i16R, i16Y0);
    GLCD_voSetPixel(u16X0 - i16R, i16Y0);
    GLCD_voDrawLine(u16X0 - i16R, i16Y0, u16X0 + i16R, i16Y0);

    while (u16X < i16y)
    {
        if (u16F >= 0)
        {
            i16y--;
            u16ddF_y += 2;
            u16F += u16ddF_y;
        }
        u16X++;
        u16ddF_x += 2;
        u16F += u16ddF_x;

        GLCD_voDrawLine(u16X0 - u16X, i16Y0 + i16y, u16X0 + u16X, i16Y0 + i16y);
        GLCD_voDrawLine(u16X0 + u16X, i16Y0 - i16y, u16X0 - u16X, i16Y0 - i16y);

        GLCD_voDrawLine(u16X0 + i16y, i16Y0 + u16X, u16X0 - i16y, i16Y0 + u16X);
        GLCD_voDrawLine(u16X0 + i16y, i16Y0 - u16X, u16X0 - i16y, i16Y0 - u16X);
    }
}

/* Draw Traingle with coordimates (x1, y1), (x2, y2), (x3, y3) */
void GLCD_voDrawTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3)
{
    /* Draw lines */
    GLCD_voDrawLine(u16X1, u16Y1, u16X2, u16Y2);
    GLCD_voDrawLine(u16X2, u16Y2, u16X3, u16Y3);
    GLCD_voDrawLine(u16X3, u16Y3, u16X1, u16Y1);
}

/* Draw Filled Traingle with coordimates (x1, y1), (x2, y2), (x3, y3) */
void GLCD_voDrawFilledTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3)
{
    int16_t i16deltax    = 0;
    int16_t i16deltay    = 0;
    int16_t u16X         = 0;
    int16_t i16y         = 0;
    int16_t u16Xinc1     = 0;
    int16_t u16Xinc2     = 0;
    int16_t i16yinc1     = 0;
    int16_t i16yinc2     = 0;
    int16_t i16den       = 0;
    int16_t i16num       = 0;
    int16_t i16numadd    = 0;
    int16_t i16numpixels = 0;
    int16_t i16curpixel  = 0;

#define ABS(u16X) ((u16X) > 0 ? (u16X) : -(u16X))

    i16deltax = ABS(u16X2 - u16X1);
    i16deltay = ABS(u16Y2 - u16Y1);
    u16X      = u16X1;
    i16y      = u16Y1;

    if (u16X2 >= u16X1)
    {
        u16Xinc1 = 1;
        u16Xinc2 = 1;
    }
    else
    {
        u16Xinc1 = -1;
        u16Xinc2 = -1;
    }

    if (u16Y2 >= u16Y1)
    {
        i16yinc1 = 1;
        i16yinc2 = 1;
    }
    else
    {
        i16yinc1 = -1;
        i16yinc2 = -1;
    }

    if (i16deltax >= i16deltay)
    {
        u16Xinc1     = 0;
        i16yinc2     = 0;
        i16den       = i16deltax;
        i16num       = i16deltax / 2;
        i16numadd    = i16deltay;
        i16numpixels = i16deltax;
    }
    else
    {
        u16Xinc2     = 0;
        i16yinc1     = 0;
        i16den       = i16deltay;
        i16num       = i16deltay / 2;
        i16numadd    = i16deltax;
        i16numpixels = i16deltay;
    }

    for (i16curpixel = 0; i16curpixel <= i16numpixels; i16curpixel++)
    {
        GLCD_voDrawLine(u16X, i16y, u16X3, u16Y3);

        i16num += i16numadd;
        if (i16num >= i16den)
        {
            i16num -= i16den;
            u16X += u16Xinc1;
            i16y += i16yinc1;
        }
        u16X += u16Xinc2;
        i16y += i16yinc2;
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM2)
    {
        /* TIM2 clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
}