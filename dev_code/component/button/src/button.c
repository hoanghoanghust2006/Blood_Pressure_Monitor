/*
 * Title : button.c
 * Copyright : HCLTech
 * Author : Trang Do
 * Creation Date : 31/01/2023
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "common.h"
#include "button.h"

/* Private define constants -------------------------------------------------------------*/
#define BUTTON_MENU_PORT   GPIOF
#define BUTTON_MENU_PIN    GPIO_PIN_11
#define BUTTON_SELECT_PORT GPIOC
#define BUTTON_SELECT_PIN  GPIO_PIN_13
#define BUTTON_UP_PORT     GPIOI
#define BUTTON_UP_PIN      GPIO_PIN_8
#define BUTTON_DOWN_PORT   GPIOI
#define BUTTON_DOWN_PIN    GPIO_PIN_10
#define BUTTON_BACK_PORT   GPIOI
#define BUTTON_BACK_PIN    GPIO_PIN_9

#define PRESS_TIME_DETECT 50
#define HOLD_TIME_DETECT  2000

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/
typedef struct
{
    tenButtonType  enBtnName;
    GPIO_TypeDef*  pstBtnPort;
    uint32_t       u32BtnPin;
    uint32_t       u32PressCounter;
    tenButtonState enBtnState;
} tstButton;

/* Private file-local global variables   ------------------------------------------------*/
static tstButton astButton[] = {
    {eBUTTON_SELECT, BUTTON_SELECT_PORT, BUTTON_SELECT_PIN, 0, eNONE},
    {eBUTTON_UP, BUTTON_UP_PORT, BUTTON_UP_PIN, 0, eNONE},
    {eBUTTON_DOWN, BUTTON_DOWN_PORT, BUTTON_DOWN_PIN, 0, eNONE},
    {eBUTTON_BACK, BUTTON_BACK_PORT, BUTTON_BACK_PIN, 0, eNONE},
    {eBUTTON_MENU, BUTTON_MENU_PORT, BUTTON_MENU_PIN, 0, eNONE}};

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/

/* Export functions definition   --------------------------------------------------------*/
tenStatus BTN_enInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    /*Configure GPIO pins : BUTTON_DOWN_PIN */
    GPIO_InitStruct.Pin  = BUTTON_DOWN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_DOWN_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : BUTTON_BACK_PIN */
    GPIO_InitStruct.Pin  = BUTTON_BACK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_BACK_PORT, &GPIO_InitStruct);

    /*Configure GPIO pins : BUTTON_UP_PIN */
    GPIO_InitStruct.Pin  = BUTTON_UP_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_UP_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : BUTTON_MENU_PIN */
    GPIO_InitStruct.Pin  = BUTTON_MENU_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_MENU_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : BUTTON_SELECT_PIN */
    GPIO_InitStruct.Pin  = BUTTON_SELECT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BUTTON_SELECT_PORT, &GPIO_InitStruct);

    return eSUCCESS;
}

void BTN_voMainFunction(uint32_t u32LoopTime)
{
    for (uint8_t u8Index = 0; u8Index < sizeof(astButton) / sizeof(tstButton); u8Index++)
    {
        /* Detect the button is Pressed or not */
        if (HAL_GPIO_ReadPin(astButton[u8Index].pstBtnPort, astButton[u8Index].u32BtnPin) == GPIO_PIN_SET)
        {
            astButton[u8Index].u32PressCounter++;
        }
        else
        {
            astButton[u8Index].u32PressCounter = 0;
            astButton[u8Index].enBtnState      = eNONE;
        }

        /* Check the button is Pressed or Held */
        if (astButton[u8Index].u32PressCounter == PRESS_TIME_DETECT / u32LoopTime)
        {
            astButton[u8Index].enBtnState = ePRESSED;
        }
        else if (astButton[u8Index].u32PressCounter >= HOLD_TIME_DETECT / u32LoopTime)
        {
            astButton[u8Index].u32PressCounter -= 100;
            astButton[u8Index].enBtnState = eHOLD;
        }
        else
        {
            /* Do nothing */
        }
    }
}

tenButtonState BTN_voGetState(tenButtonType enButton)
{
    tenButtonState enResult = eNONE;
    for (uint8_t u8Index = 0; u8Index < sizeof(astButton) / sizeof(tstButton); u8Index++)
    {
        if (enButton == astButton[u8Index].enBtnName)
        {
            /* Get the State of button */
            enResult = astButton[u8Index].enBtnState;
            /* Return button state to default */
            astButton[u8Index].enBtnState = eNONE;
        }
    }
    return enResult;
}
