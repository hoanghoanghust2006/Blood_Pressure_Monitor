/*
 * Title : rtc.c
 * Copyright : HCLTech
 * Author : Vu Hieu
 * Creation Date : 20/02/2023
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "common.h"
#include "trace.h"
#include "rtc.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
static RTC_HandleTypeDef hrtc;

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/

/* Export functions definition   --------------------------------------------------------*/
tenStatus RTC_enInit(void)
{
    RTC_TimeTypeDef sTime  = {0};
    RTC_DateTypeDef sDate  = {0};

    /* Initialize RTC Only */
    hrtc.Instance            = RTC;
    hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv   = 127;
    hrtc.Init.SynchPrediv    = 255;
    hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        trace_error();
        return eFAIL;
    }

    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x2608)
    {
        sTime.Hours          = 23;
        sTime.Minutes        = 59;
        sTime.Seconds        = 00;
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
        if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
        {
            trace_error();
            return eFAIL;
        }
        sDate.WeekDay = RTC_WEEKDAY_MONDAY;
        sDate.Month   = 01;
        sDate.Date    = 01;
        sDate.Year    = 23;

        if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
        {
            trace_error();
            return eFAIL;
        }
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x2608);
    }

    return eSUCCESS;
}

void RTC_voMainFunction(uint32_t u32LoopTime)
{
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    if (rtcHandle->Instance == RTC)
    {
        /* Initializes the peripherals clock */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
        {
            trace_error();
        }
        /* RTC clock enable */
        __HAL_RCC_RTC_ENABLE();
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{
    if (rtcHandle->Instance == RTC)
    {
        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE();
    }
}

tenStatus RTC_enSetDateTime(const tstTime* stSetTime)
{
    RTC_TimeTypeDef sTime  = {0};
    RTC_DateTypeDef sDate  = {0};

    sTime.Hours   = stSetTime->u8Hour;
    sTime.Minutes = stSetTime->u8Minute;
    sTime.Seconds = stSetTime->u8Second;

    sDate.Month = stSetTime->u8Month;
    sDate.Date  = stSetTime->u8Day;
    sDate.Year  = stSetTime->u16Year - 2000;

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        trace_error();
        return eFAIL;
    }

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        trace_error();
        return eFAIL;
    }

    return eSUCCESS;
}

tenStatus RTC_enGetDateTime(tstTime* stGetTime)
{
    RTC_TimeTypeDef gTime  = {0};
    RTC_DateTypeDef gDate  = {0};

    if (HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        trace_error();
        return eFAIL;
    }

    if (HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        trace_error();
        return eFAIL;
    }

    stGetTime->u8Hour   = gTime.Hours;
    stGetTime->u8Minute = gTime.Minutes;
    stGetTime->u8Second = gTime.Seconds;

    stGetTime->u8Month = gDate.Month;
    stGetTime->u8Day   = gDate.Date;
    stGetTime->u16Year = gDate.Year + 2000;

    return eSUCCESS;
}