/*
 * Title : font_glcd.h
 * Copyright : HCL
 * Author : < hoang.thong >
 * Creation Date : 01/03/2023
 * ------- ---------- --------
 */

#ifndef _FONT_GLCD_H_
#define _FONT_GLCD_H_

#ifdef __cplusplus
extern "C"
{
#endif
    /* Include --------------------------------------------------------------------------------*/

    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef enum
    {
        eVertical,
        eHorizontal
    } tenPosition;

    typedef enum
    {
        eSMALL,
        eMEDIUM,
        eLARGE
    } tenSizeFont;

    typedef struct
    {
        tenSizeFont enSize;
        tenPosition enPosition;
        uint8_t     u8Width;
        uint8_t     u8Height;
        uint8_t*    pu8Font;
    } tstGlcdDislayFont;

    /* Export Function Declarations -----------------------------------------------------------*/
    extern tstGlcdDislayFont stFont35;
    extern tstGlcdDislayFont stFont79;
    extern tstGlcdDislayFont stFont57;
#ifdef __cplusplus
}
#endif
#endif