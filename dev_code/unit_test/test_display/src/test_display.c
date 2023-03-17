/*
 * Title : test_display.c
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date :  Mar 06,2023
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */
#ifdef TEST_DISPLAY

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "trace.h"
#include "display.h"
#include "st7789.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
tstTime    stTime    = {6, 3, 2023, 59, 44, 2};
tstStorage stStorage = {{6, 3, 2023, 59, 44, 2}, 124, 70, 80};

osThreadId_t         DISPLAY_pvoTaskHandle;
const osThreadAttr_t stDISPLAYTaskTest = {
    .name       = "StTaskTest",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void DISPLAY_voTaskTest(void* pvoArgument);
static void DISPLAY_voTest(void);

/* Private functions definition   -------------------------------------------------------*/
static void DISPLAY_voTest(void)
{
    DPL_enDisplayRecordHistory(&stStorage, 2);
    osDelay(2000);

    for (tenDateSetupState i = eDAY; i <= eYEAR; i++)
    {
        DPL_enDisplaySetupDate(&stTime, i);
        osDelay(2000);
    }
    DPL_enSetupDateSuccess();
    osDelay(1000);

    for (tenTimeSetupState i = eHOUR; i <= eSECOND; i++)
    {
        DPL_enDisplaySetupTime(&stTime, i);
        osDelay(2000);
    }
    DPL_enSetupTimeSuccess();
    osDelay(1000);
}

static void DISPLAY_voTaskTest(void* pvoArgument)
{
    ST7789_enInit();
    DISPLAY_voTest();
    for (;;)
    {
        osDelay(500);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void DISPLAY_voTaskTestInit(void)
{
    DISPLAY_pvoTaskHandle = osThreadNew(DISPLAY_voTaskTest, NULL, &stDISPLAYTaskTest);
}

#endif /*TEST_DISPLAY*/
