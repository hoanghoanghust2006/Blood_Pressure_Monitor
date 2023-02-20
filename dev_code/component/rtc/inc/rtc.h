/*
 * Title : rtc.h
 * Copyright : HCL
 * Author : Vu Hieu
 * Creation Date : 20/02/2023
 * ------- ---------- --------
 */

#ifndef _RTC_H_
#define _RTC_H_

/* Include --------------------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "common.h"

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
tenStatus RTC_enInit(void);
void      RTC_voMainFunction(uint32_t u32LoopTime);
tenStatus RTC_enSetDateTime(const tstTime* stSetTime);
tenStatus RTC_enGetDateTime(tstTime* stGetTime);

#endif