/*
 * Title : driver_task.c
 * Copyright : HCL
 * Author : Nam Ngo
 * Creation Date : 31/08/2022
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "led.h"
#include "rtc.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/
#define DRV_TASK_DELAY_TIME_MS 5
/* Private macros -----------------------------------------------------------------------*/

tstTime tstNewTime = {.u8Day = 31, .u8Month = 12, .u16Year = 23, .u8Hour = 23, .u8Minute = 59, .u8Second = 57};

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         DRIV_pvoTaskHandle;
const osThreadAttr_t stDriverTask = {
    .name       = "DriverTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityHigh,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void DRIV_voTask(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void DRIV_voTask(void *pvoArgument)
{
    LED_voInit();
    RTC_enInit();
    if (eFAIL != RTC_enSetDateTime(&tstNewTime))
    {
        trace("Set time is OK\r\n");
    }
    else
    {
        trace("Set time isn't OK\r\n");
    }
    for (;;)
    {
        uint32_t u32DriverTaskStartTick = osKernelGetTickCount();

        LED_voMainFunction();
        RTC_voMainFunction(DRV_TASK_DELAY_TIME_MS);
        osDelayUntil(u32DriverTaskStartTick + DRV_TASK_DELAY_TIME_MS);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void DRIV_voInitTask(void)
{
    DRIV_pvoTaskHandle = osThreadNew(DRIV_voTask, NULL, &stDriverTask);
}
