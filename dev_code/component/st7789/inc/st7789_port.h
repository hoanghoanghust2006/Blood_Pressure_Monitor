/**
 * Title : st7789_port.h
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Feb 20, 2022
 * ------- ---------- --------
 */

#ifndef _ST7789_PORT_H_
#define _ST7789_PORT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Include --------------------------------------------------------------------------------*/
#include "common.h"

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
void ST7789_voRstClear(void);
void ST7789_voRstSet(void);
void ST7789_voDcClear(void);
void ST7789_voDcSet(void);
void ST7789_voCsSet(void);
void ST7789_voCsClear(void);
tenStatus ST7789_enInitIO(void);
tenStatus ST7789_voTransmit(uint8_t *pu8Data, uint16_t u16Size, uint32_t u32Timeout);

/* Export Variables Declarations -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif