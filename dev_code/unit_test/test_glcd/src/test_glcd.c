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
#include "font_glcd.h"
#include "stm32l4xx_hal.h"
/* Private define constants -------------------------------------------------------------*/
#define GLCD_TASK_DELAY_TIME_MS 5

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         GLCD_pvoTaskHandle;
const osThreadAttr_t stGlcdTask = {
    .name       = "GlcdlateTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

tstGlcdDislayFont Font35 = {eSMALL, eHorizon, 3, 5, (uint8_t *)u8Font3x5};
tstGlcdDislayFont Font79 = {eLARGE, eHorizon, 7, 9, (uint8_t *)u8Font7x9};
tstGlcdDislayFont Font57 = {eMEDIUM, eVetical, 5, 7, (uint8_t *)u8Font5x7};

/* Private function prototypes declarations   -------------------------------------------*/
static void GLCD_voTask(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void GLCD_voTask(void *pvoArgument)
{
    GLCD_enInit();
    GLCD_voClearScreen();
    GLCD_voSetPixel(0, 0);
    GLCD_voUpdate();
    GLCD_voDisplayImage(20, 20, acu8Logo32x32, 32, 32);
    osDelay(2000);
    GLCD_voClearImage();
    GLCD_voDisplayImage(0, 0, acu8BitMap128x64, 128, 64);
    osDelay(2000);
    GLCD_voClearImage();
    GLCD_voDisplayImage(20, 20, acu8Logo64x64, 64, 64);
    osDelay(2000);
    GLCD_voClearImage();
    GLCD_voDisplayImage(0, 0, acu8Logo64x32, 64, 32);
    osDelay(2000);
    GLCD_voClearImage();
    GLCD_voDisplayString(3, 4, "Thong", Font57);
    GLCD_voUpdate();
    trace("1\r\n");
    for (;;)
    {
        osDelay(1000);
    }
}
/* Export functions definition   --------------------------------------------------------*/
void GLCD_voTaskTestInit(void)
{
    GLCD_pvoTaskHandle = osThreadNew(GLCD_voTask, NULL, &stGlcdTask);
}
