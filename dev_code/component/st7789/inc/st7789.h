/**
 * Title : st7789.h
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Feb 20, 2022
 * ------- ---------- --------
 */

#ifndef _ST7789_H_
#define _ST7789_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/
#include "fonts.h"

/* Define constants -----------------------------------------------------------------------*/
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0X8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARK_BLUE   0X01CF
#define LIGHT_BLUE  0X7D7C
#define GRAY_BLUE   0X5458
#define LIGHT_GREEN 0X841F
#define LGRAY       0XC618
#define LGRAY_BLUE  0XA651
#define LBBLUE      0X2B12

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
typedef enum
{
    ST7789_ROTATION1 = 0,
    ST7789_ROTATION2 = 1,
    ST7789_ROTATION3 = 2,
    ST7789_ROTATION4 = 3
} tenOrder;

/* Export Function Declarations -----------------------------------------------------------*/
tenStatus ST7789_enInit(void);
void ST7789_voFillColor(uint16_t u16Color);
void ST7789_voSetRotation(tenOrder enRotation);
void ST7789_voFill(uint16_t u16XStart, uint16_t u16YStart, uint16_t u16XEnd, uint16_t u16YEnd, uint16_t u16Color);

/* Graphical functions. */
void ST7789_voDrawLine(uint16_t u16X0, uint16_t u16Y0, uint16_t u16X1, uint16_t u16Y1, uint16_t u16Color);
void ST7789_voDrawRectangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16Color);
void ST7789_voDrawCircle(uint16_t u16X0, uint16_t u16Y0, uint8_t u8R, uint16_t u16Color);
void ST7789_voDrawImage(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H, const uint16_t *pu16Data);

/* Extented Graphical functions. */
void ST7789_voDrawFilledRectangle(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H, uint16_t u16Color);
void ST7789_voDrawTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3, uint16_t u16Color);
void ST7789_voDrawFilledTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3, uint16_t u16Color);
void ST7789_voDrawFilledCircle(int16_t i16X0, int16_t i16Y0, int16_t i16R, uint16_t u16Color);
void ST7789_voFill(uint16_t u16XStart, uint16_t u16YStart, uint16_t u16XEnd, uint16_t u16YEnd, uint16_t u16Color);

/* Text functions. */
void ST7789_voWriteChar(uint16_t u16X, uint16_t u16Y, uint8_t u8Ch, tstFontDef stFont, uint16_t u16Color, uint16_t u16BackGroundColor);
void ST7789_voWriteString(uint16_t u16X, uint16_t u16Y, const char *pcStr, tstFontDef stFont, uint16_t u16Color, uint16_t u16BackGroundColor);

#ifdef __cplusplus
}
#endif

#endif