/*
 * Title : < menu.h >
 * Copyright : HCL
 * Author : < Hoang Hoang. >
 * Creation Date : < 20/02/2023 format >
 * ------- ---------- --------
 */

#ifndef _MENU_H_
#define _MENU_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Include --------------------------------------------------------------------------------*/
#define OPTION_CAPACITY      5
#define MAX_CHARACTER_LENGTH 20
    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef struct
    {
    } tstMenu;

    typedef struct tstOption tstOption;

    struct tstOption
    {
        char       cName[MAX_CHARACTER_LENGTH];
        tstOption* pstParent;
        uint8_t    u8CurrentIndex;
        uint8_t    u8Size;
        tstOption* apstOptionList[OPTION_CAPACITY];
        void (*pvoDoWork)();
    };

    /* Export Function Declarations -----------------------------------------------------------*/
    void MENU_voCreateOption(tstOption* pstOptionVal, char* cName, void (*pvoDoWork)());
    void MENU_voAddOptionLink(tstOption* pstParent, tstOption* pstChild);

#ifdef __cplusplus
}
#endif

#endif