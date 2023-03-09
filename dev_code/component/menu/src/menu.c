/*
 * Title : menu.c
 * Copyright : HCL
 * Author :  HoangHoang
 * Creation Date :  20/02/2023
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/
#include <string.h>

/* Local Include ------------------------------------------------------------------------*/
#include "trace.h"
#include "menu.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/
tenStatus MENU_enCreate(tstMenu* pstMenuVal, char* cName, tenProcessStatus (*pvoDoWork)(void))
{
    if (strlen(cName) <= MAX_CHARACTER_LENGTH)
    {
        strcpy(pstMenuVal->cName, cName);
    }
    else
    {
        printf("Exceed max character length");
        return eFAIL;
    }
    pstMenuVal->u8CurrentIndex = 0;
    pstMenuVal->u8Size         = 0;
    pstMenuVal->pvoDoWork      = pvoDoWork;
    return eSUCCESS;
}

tenStatus MENU_enAddLink(tstMenu* pstParent, tstMenu* pstChild)
{
    if (pstParent->u8Size < MAX_MENU_LIST)
    {
        pstChild->pstParent                        = pstParent;
        pstParent->apstMenuList[pstParent->u8Size] = pstChild;
        pstParent->u8Size++;
    }
    else
    {
        printf("Exceed max menu list");
        return eFAIL;
    }
    return eSUCCESS;
}

void MENU_voNext(tstMenu** stCurrentMenu)
{
    if ((*stCurrentMenu)->apstMenuList[0] != NULL)
    {
        *stCurrentMenu = (*stCurrentMenu)->apstMenuList[0];
    }
}

void MENU_voBack(tstMenu** stCurrentMenu)
{
    if ((*stCurrentMenu)->pstParent->pstParent != NULL)
    {
        (*stCurrentMenu)->pstParent->u8CurrentIndex = 0;
        *stCurrentMenu                              = (*stCurrentMenu)->pstParent;
    }
}

void MENU_voUp(tstMenu** stCurrentMenu)
{
    *stCurrentMenu = (*stCurrentMenu)->pstParent;
    if ((*stCurrentMenu)->u8CurrentIndex == 0)
    {
        (*stCurrentMenu)->u8CurrentIndex = ((*stCurrentMenu)->u8Size);
    }
    (*stCurrentMenu)->u8CurrentIndex--;
    *stCurrentMenu = (*stCurrentMenu)->apstMenuList[(*stCurrentMenu)->u8CurrentIndex];
}

void MENU_voDown(tstMenu** stCurrentMenu)
{
    *stCurrentMenu = (*stCurrentMenu)->pstParent;
    (*stCurrentMenu)->u8CurrentIndex++;
    if ((*stCurrentMenu)->u8CurrentIndex >= (*stCurrentMenu)->u8Size)
    {
        (*stCurrentMenu)->u8CurrentIndex = 0;
    }
    *stCurrentMenu = (*stCurrentMenu)->apstMenuList[(*stCurrentMenu)->u8CurrentIndex];
}

/* Export functions definition   --------------------------------------------------------*/
