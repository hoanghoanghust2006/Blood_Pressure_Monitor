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

    typedef struct option option;

    struct option
    {
        char    name[MAX_CHARACTER_LENGTH];
        option* parent;
        int     current_index;
        int     size;
        option* option_list[OPTION_CAPACITY];
        void (*DoWork)();
    };
    /* Export Function Declarations -----------------------------------------------------------*/
    void option_create(option* option_val, char* name, void (*DoWork)());
    void option_add_link(option* parent, option* child);
#ifdef __cplusplus
}
#endif

#endif