/*
 * Title : test_glcd.c
 * Copyright : HCLTech
 * Author : Hoang Thong
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
#include "test_glcd.h"
#include "trace.h"
#include "glcd.h"
#include "image_test.h"
/* Private define constants -------------------------------------------------------------*/
#define GLCD_TASK_DELAY_TIME_MS 500

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         GLCD_pvoTaskHandle;
const osThreadAttr_t stGlcdTask = {
    .name       = "GlcdlateTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void GLCD_voTask(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void GLCD_voTask(void *pvoArgument)
{
    GLCD_enInit();
    GLCD_voClearScreen();
    GLCD_voDisplayImage(0, 0, acu8Logo32x32, 32, 32);
    GLCD_voDisplayString(45, 0, "HCL TECH", &stFont57);
    GLCD_voDisplayString(45, 30, "0123456789", &stFont35);
    GLCD_voDisplayString(45, 50, "0123456789", &stFont79);
    GLCD_voUpdate();
    for (;;)
    {
        osDelay(GLCD_TASK_DELAY_TIME_MS);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void GLCD_voTaskTestInit(void)
{
    GLCD_pvoTaskHandle = osThreadNew(GLCD_voTask, NULL, &stGlcdTask);
}
