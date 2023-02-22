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
typedef struct
{
    tstOption *pstOptionVal;
    char       cName[MAX_CHARACTER_LENGTH];
    void (*pvoDoWork)();
} tstPreOption;

typedef enum
{
    eDoNothing
} tenOptionFunctionStatus;

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_pvoTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

static uint8_t u8NumOfLinks, u8NumOfOptions;

tstOption  stTopic0, stTopic1, stTopic2, stTopic3, stTopic1_1, stTopic1_2, stTopic1_3, stTopic1_4, stTopic1_1_1, stTopic1_1_2, stTopic2_1, stTopic3_1, stTopic3_2;
tstOption *pstCurrentOption;

/* Array include the parameters of each option: address, name and the function*/
tstPreOption astPreOption[] =
    {
        {&stTopic0, "topic0", eDoNothing},
        {&stTopic1, "topic1", eDoNothing},
        {&stTopic2, "topic2", eDoNothing},
        {&stTopic3, "topic3", eDoNothing},
        {&stTopic1_1, "topic1_1", eDoNothing},
        {&stTopic1_2, "topic1_2", eDoNothing},
        {&stTopic1_3, "topic1_3", eDoNothing},
        {&stTopic1_4, "topic1_4", eDoNothing},
        {&stTopic2_1, "topic2_1", eDoNothing},
        {&stTopic3_1, "topic3_1", eDoNothing},
        {&stTopic3_2, "topic3_2", eDoNothing},
        {&stTopic1_1_1, "topic1_1_1", eDoNothing},
        {&stTopic1_1_2, "topic1_1_2", eDoNothing},

};

/* 2D Array of pointers to options. The first indicates parent poiter, others are child pointers*/
tstOption *apstAllOptionLink[][MAX_LIST_OPTION] =
    {
        {&stTopic0, &stTopic1, &stTopic2, &stTopic3},
        {&stTopic1, &stTopic1_1, &stTopic1_2, &stTopic1_3, &stTopic1_4},
        {&stTopic1_1, &stTopic1_1_1, &stTopic1_1_2},
        {&stTopic2, &stTopic2_1},
        {&stTopic3, &stTopic3_1, &stTopic3_2}};

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void MENU_voCreateAllOptions(tstPreOption *pastPreOption, uint8_t u8NumOfOptions);
static void MENU_voOptionAddLinks(tstOption **pastOption);
static void MENU_voOptionAddAllLinks(tstOption *apstAllOptionLink[][MAX_LIST_OPTION], uint8_t u8NumOfLinks);
static void MENU_voPrintTabs(uint8_t u8CurrentOptionLevel);
static void MENU_voDisplayOptionTree(uint8_t u8NumOfOptions);

/* Private functions definition   -------------------------------------------------------*/

static void MENU_voCreateAllOptions(tstPreOption *pastPreOption, uint8_t u8NumOfOptions)
{
    for (uint8_t i = 0; i < u8NumOfOptions; i++)
    {
        MENU_voCreateOption(pastPreOption[i].pstOptionVal, pastPreOption[i].cName, pastPreOption[i].pvoDoWork);
    }
}

/* Linking a parent option to child options*/
static void MENU_voOptionAddLinks(tstOption **pastOption)
{
    for (uint8_t i = 1; i < MAX_LIST_OPTION; i++)
    {
        if (*(pastOption + i) != NULL)
            MENU_voAddOptionLink(*pastOption, *(pastOption + i));
        else
        {
            break;
        }
    }
}

static void MENU_voPrintTabs(uint8_t u8CurrentOptionLevel)
{
    for (uint8_t i = 0; i < u8CurrentOptionLevel; i++)
    {
        printf("\t");
    }
}

/* Linking all parent options to their child options*/
static void MENU_voOptionAddAllLinks(tstOption *apstAllOptionLink[][MAX_LIST_OPTION], uint8_t u8NumOfLinks)
{
    for (uint8_t i = 0; i < u8NumOfLinks; i++)
    {
        MENU_voOptionAddLinks(apstAllOptionLink[i]);
    }
}
static void MENU_voTask(void *pvoArgument)
{
    MENU_voDisplayOptionTree(u8NumOfOptions);
    for (;;)
    {
    }
}

static void MENU_voDisplayOptionTree(uint8_t u8NumOfOptions)
{
    uint8_t u8CurrentOptionLevel = 0;
    for (uint8_t i = 0; i < u8NumOfOptions; i++)
    {
        MENU_voPrintTabs(u8CurrentOptionLevel);
        printf("%s\n\r", pstCurrentOption->cName);

        if (pstCurrentOption->apstOptionList[0] == NULL)
        {
            if (pstCurrentOption->pstParent->u8CurrentIndex < pstCurrentOption->pstParent->u8Size - 1)
            {
                pstCurrentOption = pstCurrentOption->pstParent->apstOptionList[++pstCurrentOption->pstParent->u8CurrentIndex];
            }
            else
            {
                u8CurrentOptionLevel--;
                pstCurrentOption = pstCurrentOption->pstParent->pstParent->apstOptionList[++pstCurrentOption->pstParent->pstParent->u8CurrentIndex];
            }
        }
        else
        {
            u8CurrentOptionLevel++;
            pstCurrentOption = pstCurrentOption->apstOptionList[0];
        }
    }
}
/* Export functions definition   --------------------------------------------------------*/
void MENU_voTaskTestInit(void)
{
    u8NumOfLinks   = sizeof(apstAllOptionLink) / sizeof(*apstAllOptionLink);
    u8NumOfOptions = sizeof(astPreOption) / sizeof(astPreOption[0]);

    MENU_voCreateAllOptions(astPreOption, u8NumOfOptions);
    MENU_voOptionAddAllLinks(apstAllOptionLink, u8NumOfLinks);
    pstCurrentOption   = &stTopic0;
    MENU_pvoTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
