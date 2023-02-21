/*
 * Title : test_rtc.c
 * Copyright : HCLTech
 * Author : Vu Hieu
 * Creation Date : 07/02/2023
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "trace.h"
#include "test_rtc.h"
#include "stm32l4xx_hal.h"
#include "rtc.h"
/* Private define constants -------------------------------------------------------------*/
#define RTC_TASK_DELAY_TIME_MS 5

/* Private macros -----------------------------------------------------------------------*/
tstTime tstNewTime = {
    .u8Day    = 31,
    .u8Month  = 12,
    .u16Year  = 2023,
    .u8Hour   = 23,
    .u8Minute = 59,
    .u8Second = 57};

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         RTC_pvoTaskHandle;
const osThreadAttr_t stRtcTask = {
    .name       = "RTCTestTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void RTC_voTask(void* pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void RTC_voTask(void* pvoArgument)
{
    if (eSUCCESS != RTC_enSetDateTime(&tstNewTime))
    {
        trace_error();
    }

    tstTime tstGetTime;
    for (;;)
    {
        RTC_enGetDateTime(&tstGetTime);

        /* Display time Format: hh:mm:ss */
        trace("TIME: %02d:%02d:%02d\r\n", tstGetTime.u8Hour, tstGetTime.u8Minute, tstGetTime.u8Second);

        /* Display date Format: dd-mm-yy */
        trace("DATE: %02d-%02d-%2d\r\n", tstGetTime.u8Day, tstGetTime.u8Month, tstGetTime.u16Year);
        osDelay(1000);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void RTC_voTaskTestInit(void)
{
    RTC_pvoTaskHandle = osThreadNew(RTC_voTask, NULL, &stRtcTask);
}