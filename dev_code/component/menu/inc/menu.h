/*
 * Title :  menu.h
 * Copyright : HCL
 * Author :  Hoang Hoang
 * Creation Date :  20/02/2023
 * ------- ---------- --------
 */

#ifndef _MENU_H_
#define _MENU_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Include --------------------------------------------------------------------------------*/
#define MAX_MENU_LIST        5
#define MAX_CHARACTER_LENGTH 20

#include "common.h"

    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef enum
    {
        ePROCESSING,
        eCOMPLETED,
    } tenProcessStatus;

    typedef struct tstMenu tstMenu;
    struct tstMenu
    {
        char       cName[MAX_CHARACTER_LENGTH];
        tstMenu*   pstParent;
        uint8_t    u8CurrentIndex;
        uint8_t    u8Size;
        tstMenu*   apstMenuList[MAX_MENU_LIST];
        tenProcessStatus (*pvoDoWork)(void);
    };

    /* Export Function Declarations -----------------------------------------------------------*/
    tenStatus MENU_enCreate(tstMenu* pstMenuVal, char* cName, tenProcessStatus (*pvoDoWork)(void));
    tenStatus MENU_enAddLink(tstMenu* pstParent, tstMenu* pstChild);
    tenStatus MENU_voNext(tstMenu** stCurrentMenu);
    tenStatus MENU_voBack(tstMenu** stCurrentMenu);
    tenStatus MENU_voUp(tstMenu** stCurrentMenu);
    tenStatus MENU_voDown(tstMenu** stCurrentMenu);

#ifdef __cplusplus
}
#endif

#endif