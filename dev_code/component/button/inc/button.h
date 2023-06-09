/*
 * Title : button.h
 * Copyright : HCLTech
 * Author : Trang Do
 * Creation Date : 31/01/2023
 * ------- ---------- --------
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /* Include --------------------------------------------------------------------------------*/
#include "common.h"
    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef enum
    {
        eBUTTON_SELECT,
        eBUTTON_UP,
        eBUTTON_DOWN,
        eBUTTON_BACK,
        eBUTTON_MENU
    } tenButtonType;

    typedef enum
    {
        eNONE,
        ePRESSED,
        eHOLD
    } tenButtonState;

    /* Export Function Declarations -----------------------------------------------------------*/
    tenStatus      BTN_enInit(void);
    void           BTN_voMainFunction(uint32_t u32LoopTime);
    tenButtonState BTN_enGetState(tenButtonType enButton);

#ifdef __cplusplus
}
#endif

#endif