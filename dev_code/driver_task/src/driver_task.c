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

/* Private define constants -------------------------------------------------------------*/
#define DRV_TASK_DELAY_TIME_MS 10
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

#ifdef ENABLE_WATCHDOG
    WDG_voInit();
#endif

    for (;;)
    {
        uint32_t u32DriverTaskStartTick = osKernelGetTickCount();
        LED_voMainFunction();

#ifdef ENABLE_WATCHDOG
        WDG_voMainFunction();
#endif

        osDelayUntil(u32DriverTaskStartTick + DRV_TASK_DELAY_TIME_MS);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void DRIV_voTaskInit(void)
{
    DRIV_pvoTaskHandle = osThreadNew(DRIV_voTask, NULL, &stDriverTask);
}
