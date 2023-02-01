/*
 * Title : led.c
 * Copyright : HCL
 * Author : Nam Ngo
 * Creation Date : Sep 9, 2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#define USER_LED_PORT GPIOB
#define USER_LED_PIN  GPIO_PIN_13

#define LED_DELAY_CYCLE 50

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/

/* Export functions definition   --------------------------------------------------------*/
void LED_voInit(void)
{
}

void LED_voMainFunction(void)
{
    static uint8_t u8LedDelayIndex = 0;
    if (u8LedDelayIndex > LED_DELAY_CYCLE)
    {
        HAL_GPIO_TogglePin(USER_LED_PORT, USER_LED_PIN);
        u8LedDelayIndex = 0;
    }
    else
    {
        u8LedDelayIndex++;
    }
}