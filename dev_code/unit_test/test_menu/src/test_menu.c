/*
 * Title : test_menu.c
 * Copyright : HCL
 * Author : HoangHoang
 * Creation Date : 21/02/2023
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
#include "test_menu.h"
#include "menu.h"
/* Private define constants -------------------------------------------------------------*/
#define MENU_TASK_DELAY_TIME_MS 500
/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_voTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

option  Topic0, Topic1, Topic2, Topic1_1, Topic1_2;
option *current_option;

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void printf_func(option *option_val);
/* Private functions definition   -------------------------------------------------------*/
static void MENU_voTask(void *pvoArgument)
{
    for (;;)
    {
        // trace("hello\r");
        // printf_func(current_option);
        trace("%s   select\r\n", Topic1.parent->name);
        osDelay(MENU_TASK_DELAY_TIME_MS);
    }
}
static void printf_func(option *option_val)
{
    for (int i = 0; i < option_val->parent->size; i++)
    {
        if (i == option_val->parent->current_index)
        {
            trace("%s   select\r\\n", option_val->name);
        }
        else
            trace("%s\r\n", option_val->parent->option_list[i]->name);
    }
}
/* Export functions definition   --------------------------------------------------------*/
void MENU_voTaskTestInit(void)
{
    option_create(&Topic0, "Topic0", 0);
    option_create(&Topic1, "Topic1", 0);
    option_create(&Topic1_1, "Topic1_1", 0);
    option_create(&Topic1_2, "Topic2", 0);
    option_add_link(&Topic0, &Topic1);
    option_add_link(&Topic0, &Topic2);
    current_option    = &Topic0;
    MENU_voTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
