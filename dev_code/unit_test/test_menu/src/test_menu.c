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
#include <stdio.h>

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

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         MENU_pvoTaskHandle;
const osThreadAttr_t stMenuTask = {
    .name       = "MenuTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

static tstMenu stMenu0;
static tstMenu stMenu1, stMenu2, stMenu3;
static tstMenu stMenu1_1, stMenu1_2, stMenu1_3, stMenu1_4, stMenu2_1, stMenu3_1, stMenu3_2;
static tstMenu stMenu1_1_1, stMenu1_1_2, stMenu2_1_1, stMenu2_1_2;
static tstMenu stMenu1_1_2_1, stMenu1_1_2_2;

/* Array includes the parameters of each option: address, name and the function*/
static tstPreMenu astPreMenu[] =
    {
        {&stMenu0, "Menu0", 0},
        {&stMenu1, "Menu1", 0},
        {&stMenu2, "Menu2", 0},
        {&stMenu3, "Menu3", 0},
        {&stMenu1_1, "Menu1_1", 0},
        {&stMenu1_2, "Menu1_2", 0},
        {&stMenu1_3, "Menu1_3", 0},
        {&stMenu1_4, "Menu1_4", 0},
        {&stMenu2_1, "Menu2_1", 0},
        {&stMenu3_1, "Menu3_1", 0},
        {&stMenu3_2, "Menu3_2", 0},
        {&stMenu1_1_1, "Menu1_1_1", 0},
        {&stMenu1_1_2, "Menu1_1_2", 0},
        {&stMenu2_1_1, "Menu2_1_1", 0},
        {&stMenu2_1_2, "Menu2_1_2", 0},
        {&stMenu1_1_2_1, "Menu1_1_2_1", 0},
        {&stMenu1_1_2_2, "Menu1_1_2_2", 0}};

/* 2D Array of pointers to options. The first indicates parent poiter, others are child pointers*/
static tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1] =
    {
        {&stMenu0, &stMenu1, &stMenu2, &stMenu3, NULL},
        {&stMenu1, &stMenu1_1, &stMenu1_2, &stMenu1_3, &stMenu1_4},
        {&stMenu1_1, &stMenu1_1_1, &stMenu1_1_2, NULL},
        {&stMenu2_1, &stMenu2_1_1, &stMenu2_1_2},
        {&stMenu2, &stMenu2_1, NULL},
        {&stMenu3, &stMenu3_1, &stMenu3_2, NULL},
        {&stMenu1_1_2, &stMenu1_1_2_1, &stMenu1_1_2_2, NULL}};

/* Private function prototypes declarations   -------------------------------------------*/
static void MENU_voTask(void *pvoArgument);
static void MENU_voCreateAll(tstPreMenu *pastPreMenu);
static void MENU_voAddLinks(tstMenu **pastMenu);
static void MENU_voAddAllLinks(tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1]);
static void MENU_voPrintTabs(uint8_t u8CurrentMenuLevel);
static void MENU_voDisplayTree();

/* Private functions definition   -------------------------------------------------------*/
static void MENU_voCreateAll(tstPreMenu *pastPreMenu)
{
    uint8_t i = 0;
    while ((pastPreMenu + i)->pstMenuVal != NULL)
    {
        MENU_enCreate((pastPreMenu + i)->pstMenuVal, (pastPreMenu + i)->cName, (pastPreMenu + i)->pvoDoWork);
        i++;
    }
}

/* Linking a parent option to child options*/
static void MENU_voAddLinks(tstMenu **pastMenu)
{
    uint8_t i = 1;
    while (*(pastMenu + i) != NULL)
    {
        MENU_enAddLink(*pastMenu, *(pastMenu + i));
        i++;
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
static void MENU_voAddAllLinks(tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1])
{
    uint8_t i = 0;
    while ((apstAllMenuLink[i][MAX_MENU_LIST + 1]) != NULL)
    {
        MENU_voAddLinks(apstAllMenuLink[i]);
        i++;
    }
}

static void MENU_voDisplayTree()
{
    tstMenu *pstCurrentMenu     = &stMenu0;
    uint8_t  u8CurrentMenuLevel = 0;

    /*Loop for printing all menu options*/
    while (pstCurrentMenu != NULL)
    {
        MENU_voPrintTabs(u8CurrentMenuLevel);
        printf("%s\n\r", pstCurrentMenu->cName);

        if (pstCurrentMenu->apstMenuList[0] == NULL)
        {
            pstCurrentMenu = pstCurrentMenu->pstParent;
            while (pstCurrentMenu->u8CurrentIndex >= pstCurrentMenu->u8Size - 1)
            {
                u8CurrentMenuLevel--;
                pstCurrentMenu = pstCurrentMenu->pstParent;
            }
            pstCurrentMenu->u8CurrentIndex++;
            pstCurrentMenu = pstCurrentMenu->apstMenuList[pstCurrentMenu->u8CurrentIndex];
        }
        else
        {
            u8CurrentMenuLevel++;
            pstCurrentMenu = pstCurrentMenu->apstMenuList[0];
        }
    }
}

static void MENU_voTask(void *pvoArgument)
{
    MENU_voCreateAll(astPreMenu);
    MENU_voAddAllLinks(apstAllMenuLink);
    MENU_voDisplayTree();
    for (;;)
    {
    }
}

/* Export functions definition   --------------------------------------------------------*/
void MENU_voTaskTestInit(void)
{
    MENU_pvoTaskHandle = osThreadNew(MENU_voTask, NULL, &stMenuTask);
}
