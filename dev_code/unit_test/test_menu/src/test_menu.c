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
/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_pvoTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

tstOption  Topic0, Topic1, Topic2, Topic3, Topic1_1, Topic1_2, Topic1_3, Topic1_4, Topic1_1_1, Topic1_1_2, Topic2_1, Topic3_1, Topic3_2;
tstOption *pstCurrentOption;
uint8_t    u8NumOfLinks, u8NumOfOptions;

tstPreOption astPreOption[] =
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

tstOption *apstAllOptionLink[][MAX_LIST_OPTION] =
    {
        {&Topic0, &Topic1, &Topic2, &Topic3},
        {&Topic1, &Topic1_1, &Topic1_2, &Topic1_3, &Topic1_4},
        {&Topic1_1, &Topic1_1_1, &Topic1_1_2},
        {&Topic2, &Topic2_1},
        {&Topic3, &Topic3_1, &Topic3_2}};

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void MENU_CreateAllOptions(tstPreOption *pastPreOption, uint8_t u8NumOfOptions);
static void MENU_OptionAddLinks(tstOption **pastOption);
static void MENU_OptionAddAllLinks(tstOption *pastAllOptionLink[][MAX_LIST_OPTION], uint8_t u8NumOfLinks);
static void MENU_PrintTabs(uint8_t u8CurrentOptionLevel);
static void MENU_DisplayOptionTree(uint8_t u8NumOfOptions);

/* Private functions definition   -------------------------------------------------------*/
static void MENU_CreateAllOptions(tstPreOption *pastPreOption, uint8_t u8NumOfOptions)
{
    for (uint8_t i = 0; i < u8NumOfOptions; i++)
    {
        MENU_voCreateOption(pastPreOption[i].pstOptionVal, pastPreOption[i].cName, pastPreOption[i].pvoDoWork);
    }
}

static void MENU_OptionAddLinks(tstOption **pastOption)
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

static void MENU_PrintTabs(uint8_t u8CurrentOptionLevel)
{
    for (uint8_t i = 0; i < u8CurrentOptionLevel; i++)
    {
        printf("\t");
    }
}
static void MENU_OptionAddAllLinks(tstOption *pastAllOptionLink[][MAX_LIST_OPTION], uint8_t u8NumOfLinks)
{
    for (uint8_t i = 0; i < u8NumOfLinks; i++)
    {
        MENU_OptionAddLinks(pastAllOptionLink[i]);
    }
}
static void MENU_voTask(void *pvoArgument)
{
    MENU_DisplayOptionTree(u8NumOfOptions);
    for (;;)
    {
    }
}

static void MENU_DisplayOptionTree(uint8_t u8NumOfOptions)
{
    uint8_t u8CurrentOptionLevel = 0;
    for (uint8_t i = 0; i < u8NumOfOptions; i++)
    {
        MENU_PrintTabs(u8CurrentOptionLevel);
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

    MENU_CreateAllOptions(astPreOption, u8NumOfOptions);
    MENU_OptionAddAllLinks(apstAllOptionLink, u8NumOfLinks);
    pstCurrentOption   = &Topic0;
    MENU_pvoTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
