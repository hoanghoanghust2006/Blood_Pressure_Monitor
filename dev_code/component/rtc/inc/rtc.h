/*
 * Title : rtc.h
 * Copyright : HCL
 * Author : < Name of the author. >
 * Creation Date : < Date in DD/MMM/YY format >
 * ------- ---------- --------
 */

#ifndef _RTC_H_
#define _RTC_H_

/* Include --------------------------------------------------------------------------------*/
#include "common.h"
#include "stm32l4xx_hal.h"

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
tenStatus RTC_enInit(void);
void      RTC_voMainFunction(uint32_t u32LoopTime);
tenStatus RTC_enSetDateTime(const tstTime* stSetTime);
tenStatus RTC_enGetDateTime(tstTime* stGetTime);
#endif