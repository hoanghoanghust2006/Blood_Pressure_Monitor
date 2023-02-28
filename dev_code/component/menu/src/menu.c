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
tenStatus MENU_enCreate(tstMenu* pstMenuVal, char* cName, void (*pvoDoWork)())
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

/* Export functions definition   --------------------------------------------------------*/
