/*
 * Title : < glcd.h >
 * Copyright : HCL
 * Author : < hoang.thong >
 * Creation Date : < Date in 14/02/2023 format >
 * ------- ---------- --------
 */

#ifndef _GLCD_H_
#define _GLCD_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/
#include "common.h"
    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef enum
    {
        eENABLE,
        eDISABLE
    } tenGraphicMode;

    typedef enum
    {
        eSMALL,
        eMEDIUM,
        eLARGE
    } tenTextSize;

    /* Export Function Declarations -----------------------------------------------------------*/
    tenStatus GLCD_enInit(void);
    void GLCD_voSetGraphicMode(tenGraphicMode enGraphicMode);
    void      GLCD_voDisplayImage(uint8_t u8X, uint8_t u8Y, uint8_t* u8P, uint8_t u8W, uint8_t u8H);
    void      GLCD_voDisplayString(tenTextSize enSize, uint8_t u8X, uint8_t u8Y, char* cString);
    /* Clear screen in any mode */
    void GLCD_voClearScreen(void);
    void GLCD_voUpdate(void);

    /* Draw BitMap on the display */
    void GLCD_voDrawBitMap(const uint8_t* cu8Graphic);
    void GLCD_voDrawImage(uint8_t u8H, uint8_t u8W, const uint8_t* cu8Graphic);

    /* Update the display with the selected graphics */
    void GLCD_voSetPixel(uint8_t u8X, uint8_t u8Y);

    /* Draw line from (X0, Y0) to (X1, Y1) */
    void GLCD_voDrawLine(uint8_t u8X0, uint8_t u8Y0, uint8_t u8X1, uint8_t u8Y1);

    /* Draw rectangle from (X,Y) w- width, h- height */
    void GLCD_voDrawRectangle(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H);

    /* Draw filled rectangle */
    void GLCD_voDrawFilledRectangle(uint16_t u16X, uint16_t u16Y, uint16_t u16W, uint16_t u16H);

    /* Draw circle with centre (X0, Y0) and radius= radius */
    void GLCD_voDrawCircle(uint8_t u8X0, uint8_t u8Y0, uint8_t u8Radius);

    /* Draw Filled Circle with centre (X0, Y0) and radius= r */
    void GLCD_voDrawFilledCircle(int16_t u16X0, int16_t i16Y0, int16_t i16R);

    /* Draw Traingle with coordimates (x1, y1), (x2, y2), (x3, y3) */
    void GLCD_voDrawTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3);

    /* Draw Filled Traingle with coordimates (x1, y1), (x2, y2), (x3, y3) */
    void GLCD_voDrawFilledTriangle(uint16_t u16X1, uint16_t u16Y1, uint16_t u16X2, uint16_t u16Y2, uint16_t u16X3, uint16_t u16Y3);
    void GLCD_Delay_us(uint16_t u16Delay);

#ifdef __cplusplus
}
#endif

#endif
