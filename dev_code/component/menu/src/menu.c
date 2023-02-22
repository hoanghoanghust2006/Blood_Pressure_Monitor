/*
 * Title : < Name of the file. >
 * Copyright : HCL
 * Author : < Name of the author. >
 * Creation Date : < Date in DD/MMM/YY format >
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include <stdio.h>
#include "common.h"
#include "trace.h"
#include "menu.h"
#include <string.h>

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/
void MENU_voCreateOption(tstOption* pstOptionVal, char* cName, void (*pvoDoWork)())
{
    strcpy(pstOptionVal->cName, cName);
    pstOptionVal->u8CurrentIndex    = 0;
    pstOptionVal->u8Size            = 0;
    pstOptionVal->pvoDoWork         = pvoDoWork;
}

void MENU_voAddOptionLink(tstOption* pstParent, tstOption* pstChild)
{
    pstChild->pstParent                          = pstParent;
    pstParent->apstOptionList[pstParent->u8Size] = pstChild;
    pstParent->u8Size++;
}

/* Export functions definition   --------------------------------------------------------*/
