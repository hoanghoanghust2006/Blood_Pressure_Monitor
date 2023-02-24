/*
 * Title : test_menu.c
 * Copyright : HCL
 * Author : HoangHoang
 * Creation Date : 20/02/2023
 * Description : Test menu
 * Limitations :
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History :
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
typedef struct
{
    tstMenu   *pstMenuVal;
    char       cName[MAX_CHARACTER_LENGTH];
    void (*pvoDoWork)(void);
} tstPreMenu;

typedef enum
{
    eDoNothing
} tenMenuFunctionStatus;

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_pvoTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

static uint8_t u8NumOfLinks = 0;
static uint8_t u8NumOfMenus = 0;

static tstMenu stMenu0;
static tstMenu stMenu1, stMenu2, stMenu3;
static tstMenu stMenu1_1, stMenu1_2, stMenu1_3, stMenu1_4, stMenu2_1, stMenu3_1, stMenu3_2;
static tstMenu stMenu1_1_1, stMenu1_1_2;

/* Array includes the parameters of each option: address, name and the function*/
static tstPreMenu astPreMenu[] =
    {
        {&stMenu0, "Menu0", eDoNothing},
        {&stMenu1, "Menu1", eDoNothing},
        {&stMenu2, "Menu2", eDoNothing},
        {&stMenu3, "Menu3", eDoNothing},
        {&stMenu1_1, "Menu1_1", eDoNothing},
        {&stMenu1_2, "Menu1_2", eDoNothing},
        {&stMenu1_3, "Menu1_3", eDoNothing},
        {&stMenu1_4, "Menu1_4", eDoNothing},
        {&stMenu2_1, "Menu2_1", eDoNothing},
        {&stMenu3_1, "Menu3_1", eDoNothing},
        {&stMenu3_2, "Menu3_2", eDoNothing},
        {&stMenu1_1_1, "Menu1_1_1", eDoNothing},
        {&stMenu1_1_2, "Menu1_1_2", eDoNothing},
};

/* 2D Array of pointers to options. The first indicates parent poiter, others are child pointers*/
static tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1] =
    {
        {&stMenu0, &stMenu1, &stMenu2, &stMenu3, NULL},
        {&stMenu1, &stMenu1_1, &stMenu1_2, &stMenu1_3, &stMenu1_4, NULL},
        {&stMenu1_1, &stMenu1_1_1, &stMenu1_1_2, NULL},
        {&stMenu2, &stMenu2_1, NULL},
        {&stMenu3, &stMenu3_1, &stMenu3_2, NULL}};

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void MENU_voCreateAll(tstPreMenu *pastPreMenu, uint8_t u8NumOfMenus);
static void MENU_voAddLinks(tstMenu **pastMenu);
static void MENU_voAddAllLinks(tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1], uint8_t u8NumOfLinks);
static void MENU_voPrintTabs(uint8_t u8CurrentMenuLevel);
static void MENU_voDisplayTree(uint8_t u8NumOfMenus);

/* Private functions definition   -------------------------------------------------------*/
static void MENU_voCreateAll(tstPreMenu *pastPreMenu, uint8_t u8NumOfMenus)
{
    for (uint8_t i = 0; i < u8NumOfMenus; i++)
    {
        MENU_voCreate(pastPreMenu[i].pstMenuVal, pastPreMenu[i].cName, pastPreMenu[i].pvoDoWork);
    }
}

/* Linking a parent option to child options*/
static void MENU_voAddLinks(tstMenu **pastMenu)
{
    for (uint8_t i = 1; i < MAX_MENU_LIST + 1; i++)
    {
        if (*(pastMenu + i) != NULL)
        {
            MENU_voAddLink(*pastMenu, *(pastMenu + i));
        }
        else
        {
            break;
        }
    }
}

static void MENU_voPrintTabs(uint8_t u8CurrentMenuLevel)
{
    for (uint8_t i = 0; i < u8CurrentMenuLevel; i++)
    {
        printf("\t");
    }
}

/* Linking all parent options to their child options*/
static void MENU_voAddAllLinks(tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1], uint8_t u8NumOfLinks)
{
    for (uint8_t i = 0; i < u8NumOfLinks; i++)
    {
        MENU_voAddLinks(apstAllMenuLink[i]);
    }
}
static void MENU_voTask(void *pvoArgument)
{
    MENU_voDisplayTree(u8NumOfMenus);
    for (;;)
    {
    }
}

static void MENU_voDisplayTree(uint8_t u8NumOfMenus)
{
    tstMenu *pstCurrentMenu     = &stMenu0;
    uint8_t  u8CurrentMenuLevel = 0;
    for (uint8_t i = 0; i < u8NumOfMenus; i++)
    {
        MENU_voPrintTabs(u8CurrentMenuLevel);
        printf("%s\n\r", pstCurrentMenu->cName);

        if (pstCurrentMenu->apstMenuList[0] == NULL)
        {
            if (pstCurrentMenu->pstParent->u8CurrentIndex < pstCurrentMenu->pstParent->u8Size - 1)
            {
                pstCurrentMenu = pstCurrentMenu->pstParent->apstMenuList[++pstCurrentMenu->pstParent->u8CurrentIndex];
            }
            else
            {
                u8CurrentMenuLevel--;
                pstCurrentMenu = pstCurrentMenu->pstParent->pstParent->apstMenuList[++pstCurrentMenu->pstParent->pstParent->u8CurrentIndex];
            }
        }
        else
        {
            u8CurrentMenuLevel++;
            pstCurrentMenu = pstCurrentMenu->apstMenuList[0];
        }
    }
}

/* Export functions definition   --------------------------------------------------------*/
void MENU_voTaskTestInit(void)
{
    u8NumOfLinks = sizeof(apstAllMenuLink) / sizeof(*apstAllMenuLink);
    u8NumOfMenus = sizeof(astPreMenu) / sizeof(astPreMenu[0]);

    MENU_voCreateAll(astPreMenu, u8NumOfMenus);
    MENU_voAddAllLinks(apstAllMenuLink, u8NumOfLinks);

    MENU_pvoTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
