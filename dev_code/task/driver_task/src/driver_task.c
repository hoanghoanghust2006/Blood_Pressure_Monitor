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
#include "button.h"
#include "rtc.h"
#include "glcd.h"
#include "trace.h"
#include "storage.h"
#include "st7789.h"
#include "display.h"

/* Private define constants -------------------------------------------------------------*/
#define DRV_TASK_DELAY_TIME_MS 5

/* Private macros -----------------------------------------------------------------------*/

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
    BTN_enInit();
    GLCD_enInit();
    STO_voInit();
    ST7789_enInit();

    for (;;)
    {
        uint32_t u32DriverTaskStartTick = osKernelGetTickCount();

        LED_voMainFunction();
        RTC_voMainFunction(DRV_TASK_DELAY_TIME_MS);
        BTN_voMainFunction(DRV_TASK_DELAY_TIME_MS);
        STO_voMainFunction();

        osDelayUntil(u32DriverTaskStartTick + DRV_TASK_DELAY_TIME_MS);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void DRIV_voInitTask(void)
{
    DRIV_pvoTaskHandle = osThreadNew(DRIV_voTask, NULL, &stDriverTask);
}
