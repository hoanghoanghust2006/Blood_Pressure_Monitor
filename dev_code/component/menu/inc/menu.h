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

    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef struct tstMenu tstMenu;
    struct tstMenu
    {
        char       cName[MAX_CHARACTER_LENGTH];
        tstMenu*   pstParent;
        uint8_t    u8CurrentIndex;
        uint8_t    u8Size;
        tstMenu*   apstMenuList[MAX_MENU_LIST];
        void (*pvoDoWork)(void);
    };

    /* Export Function Declarations -----------------------------------------------------------*/
    void MENU_voCreate(tstMenu* pstMenuVal, char* cName, void (*pvoDoWork)(void));
    void MENU_voAddLink(tstMenu* pstParent, tstMenu* pstChild);

#ifdef __cplusplus
}
#endif

#endif