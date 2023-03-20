/**
 * Title : fonts.h
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Feb 20, 2022
 * ------- ---------- --------
 */

#ifndef __FONT_H
#define __FONT_H

/* Include --------------------------------------------------------------------------------*/
#include "common.h"

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
typedef struct
{
    const uint8_t   u8Width;
    const uint8_t   u8Height;
    const uint16_t *pu16Data;
} tstFontDef;

/* Export Function Declarations -----------------------------------------------------------*/

/* Export variable Declarations -----------------------------------------------------------*/
extern tstFontDef stFont7x10;
extern tstFontDef stFont11x18;
extern tstFontDef stFont16x26;

#endif