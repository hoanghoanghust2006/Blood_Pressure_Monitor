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
#include "button.h"

/* Private define constants -------------------------------------------------------------*/
#define BTN_TASK_DELAY_TIME_MS 10

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/
typedef struct
{
    tenButtonType enButtonName;
    char         *cString;
} tstButtonCheck;

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         BTN_pvoTaskHandle;
const osThreadAttr_t stBtnTask = {
    .name       = "ButtonTestTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

tstButtonCheck astBtnCheck[] =
    {
        {eBUTTON_SELECT, "Select"},
        {eBUTTON_UP, "Up"},
        {eBUTTON_DOWN, "Down"},
        {eBUTTON_BACK, "Back"},
        {eBUTTON_MENU, "Menu"}};

/* Private function prototypes declarations   -------------------------------------------*/
static void BTN_voTask(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void BTN_voTask(void *pvoArgument)
{
    for (;;)
    {
        tenButtonState enCheckState = eNONE;
        for (uint8_t u8Index = 0; u8Index < sizeof(astBtnCheck) / sizeof(tstButtonCheck); u8Index++)
        {
            enCheckState = BTN_voGetState(astBtnCheck[u8Index].enButtonName);
            if (enCheckState == ePRESSED)
            {
                trace("Button %s is pressed\r\n", astBtnCheck[u8Index].cString);
            }
            else if (enCheckState == eHOLD)
            {
                trace("Button %s is held\r\n", astBtnCheck[u8Index].cString);
            }
            else
            {
                /* Do nothing */
            }
        }

        osDelay(BTN_TASK_DELAY_TIME_MS);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void BTN_voTaskTestInit(void)
{
    BTN_pvoTaskHandle = osThreadNew(BTN_voTask, NULL, &stBtnTask);
}
