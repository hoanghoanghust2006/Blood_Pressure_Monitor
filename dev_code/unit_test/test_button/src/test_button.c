/*
 * Title : test_button.c
 * Copyright : HCL
 * Author : Do Trang
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

/* Private define constants -------------------------------------------------------------*/
#define BTN_TASK_DELAY_TIME_MS 5
/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         BTN_pvoTaskHandle;
const osThreadAttr_t stBtnTask = {
    .name       = "ButtonTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void BTN_voTask(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void BTN_voTask(void *pvoArgument)
{
    for (;;)
    {

        trace("Hello from unit test button\r\n");
        osDelay(500);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void BTN_voTaskTestInit(void)
{
    BTN_pvoTaskHandle = osThreadNew(BTN_voTask, NULL, &stBtnTask);
}
