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

typedef struct preOption
{
    option *option_val;
    char    name[MAX_CHARACTER_LENGTH];
    void (*DoWork)();
} preOption;
/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_pvoTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

option  Topic0, Topic1, Topic2, Topic3, Topic1_1, Topic1_2, Topic1_3, Topic1_4, Topic1_1_1, Topic1_1_2, Topic2_1, Topic3_1, Topic3_2;
option *current_option;
uint8_t num_of_links, num_of_options;

preOption astPreOption[] =
    {
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
        {&Topic3, "topic3", 0},
        {&Topic3_1, "topic3_1", 0},
        {&Topic3_2, "topic3_2", 0}};

option *pastAllOptionLink[][MAX_LIST_OPTION] =
    {
        {&Topic0, &Topic1, &Topic2, &Topic3},
        {&Topic1, &Topic1_1, &Topic1_2, &Topic1_3, &Topic1_4},
        {&Topic1_1, &Topic1_1_1, &Topic1_1_2},
        {&Topic2, &Topic2_1},
        {&Topic3, &Topic3_1, &Topic3_2}};

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void all_option_create(preOption *astPreOption, uint8_t num_of_options);
static void option_add_links(option **pastOption);
static void option_add_all_links(option *pastAllOptionLink[][MAX_LIST_OPTION], uint8_t num_of_links);
static void print_tabs(uint8_t current_option_level);
static void display_option_tree(uint8_t num_of_options);

/* Private functions definition   -------------------------------------------------------*/
static void all_option_create(preOption *astPreOption, uint8_t num_of_options)
{
    for (uint8_t i = 0; i < num_of_options; i++)
    {
        option_create(astPreOption[i].option_val, astPreOption[i].name, astPreOption[i].DoWork);
    }
}

static void option_add_links(option **pastOption)
{
    for (uint8_t i = 1; i < MAX_LIST_OPTION; i++)
    {
        if (*(pastOption + i) != NULL)
            option_add_link(*pastOption, *(pastOption + i));
        else
        {
            break;
        }
    }
}

static void print_tabs(uint8_t current_option_level)
{
    for (uint8_t i = 0; i < current_option_level; i++)
    {
        printf("\t");
    }
}
static void option_add_all_links(option *pastAllOptionLink[][MAX_LIST_OPTION], uint8_t num_of_links)
{
    for (uint8_t i = 0; i < num_of_links; i++)
    {
        option_add_links(pastAllOptionLink[i]);
    }
}
static void MENU_voTask(void *pvoArgument)
{
    display_option_tree(num_of_options);
    for (;;)
    {
    }
}

static void display_option_tree(uint8_t num_of_options)
{
    uint8_t current_option_level = 0;
    for (uint8_t i = 0; i < num_of_options; i++)
    {
        print_tabs(current_option_level);
        printf("%s\n\r", current_option->name);

        if (current_option->option_list[0] == NULL)
        {
            if (current_option->parent->current_index < current_option->parent->size - 1)
            {
                current_option = current_option->parent->option_list[++current_option->parent->current_index];
            }
            else
            {
                current_option_level--;
                current_option = current_option->parent->parent->option_list[++current_option->parent->parent->current_index];
            }
        }
        else
        {
            current_option_level++;
            current_option = current_option->option_list[0];
        }
    }
}
/* Export functions definition   --------------------------------------------------------*/
void MENU_voTaskTestInit(void)
{
    num_of_links   = sizeof(pastAllOptionLink) / sizeof(*pastAllOptionLink);
    num_of_options = sizeof(astPreOption) / sizeof(astPreOption[0]);

    all_option_create(astPreOption, num_of_options);
    option_add_all_links(pastAllOptionLink, num_of_links);
    current_option     = &Topic0;
    MENU_pvoTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
