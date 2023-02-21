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
#define MAX_LIST_OPTION         5
/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/
typedef struct link link;
struct link
{
    option *option_val;
    char    name[MAX_CHARACTER_LENGTH];
    void (*DoWork)();
};
/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_voTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

option Topic0, Topic1, Topic2, Topic1_1, Topic1_2, Topic1_3, Topic1_4, Topic1_1_1, Topic1_1_2, Topic2_1;

option  Topic0, Topic1, Topic2, Topic1_1, Topic1_2, Topic1_3, Topic1_4, Topic1_1_1, Topic1_1_2, Topic2_1;
option *a[][MAX_LIST_OPTION] = {
    {&Topic0, &Topic1, &Topic2},
    {&Topic1, &Topic1_1, &Topic1_2, &Topic1_3, &Topic1_4},
    {&Topic1_1, &Topic1_1_1, &Topic1_1_2},
    {&Topic2, &Topic2_1},
};
link b[] = {
    {&Topic0, "topic0", 0},
    {&Topic1, "topic1", 0},
    {&Topic2, "topic2", 0},
    {&Topic2_1, "topic2_1", 0},
    {&Topic1_1, "topic1_1", 0},
    {&Topic1_2, "topic1_2", 0},
    {&Topic1_3, "topic1_3", 0},
    {&Topic1_4, "topic1_4", 0},
    {&Topic1_1_1, "topic1_1_1", 0},
    {&Topic1_1_2, "topic1_1_2", 0},
};
option *current_option = &Topic1;
char    cKey;
int     k           = 0;
int     numberoftab = 0;

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void printf_func();
void        option_add_links(option **child, int size)
{
    for (int i = 1; i < size; i++)
    {
        if (*(child + i) != NULL) option_add_link(*child, *(child + i));
    }
}
/* Private functions definition   -------------------------------------------------------*/
void in(int numberoftab)
{
    for (int i = 0; i < numberoftab; i++)
    {
        printf("\t");
    }
}
static void MENU_voTask(void *pvoArgument)
{
    printf_func();
    for (;;)
    {
        // in(3);
        // printf("hello\n\r");
        // printf("hello  \t");
        // printf("ok ");
        // printf("\n\r");
        // // trace("\r\t\n hello");
        osDelay(MENU_TASK_DELAY_TIME_MS);
    }
}
// static void printf_func(option *option_val)
// {
//     for (int i = 0; i < option_val->parent->size; i++)
//     {
//         trace("%s\r\n", option_val->parent->option_list[i]->name);
//     }
// }
static void printf_func()
{
    for (int i = 0; i < 9; i++)
    {
        in(numberoftab);
        printf("%s\n\r", current_option->name);

        if (current_option->option_list[0] == NULL)
        {
            if (current_option->parent->current_index < current_option->parent->size - 1)
            {
                current_option->parent->current_index++;

                current_option = current_option->parent->option_list[current_option->parent->current_index];
            }
            else
            {
                numberoftab--;
                current_option->parent->parent->current_index++;
                current_option = current_option->parent->parent->option_list[current_option->parent->parent->current_index];
            }
        }
        else
        {
            numberoftab++;
            current_option = current_option->option_list[0];
        }
    }
}
/* Export functions definition   --------------------------------------------------------*/
void MENU_voTaskTestInit(void)
{
    for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++)
    {
        option_create(b[i].option_val, b[i].name, b[i].DoWork);
    }

    for (int i = 0; i < sizeof(a) / sizeof(*a); i++)
    {
        option_add_links(a[i], MAX_LIST_OPTION);
    }
    MENU_voTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
