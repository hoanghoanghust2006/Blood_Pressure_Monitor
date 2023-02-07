/*
 * Title : test_template.c
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

/* Private define constants -------------------------------------------------------------*/
#define TEMP_TASK_DELAY_TIME_MS 5
/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         TEMP_pvoTaskHandle;
const osThreadAttr_t stTempTask = {
    .name       = "TemplateTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void TEMP_voTask(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void TEMP_voTask(void *pvoArgument)
{
    for (;;)
    {

    }
}

/* Export functions definition   --------------------------------------------------------*/
void TEMP_voTaskTestInit(void)
{
    TEMP_pvoTaskHandle = osThreadNew(TEMP_voTask, NULL, &stTempTask);
}
