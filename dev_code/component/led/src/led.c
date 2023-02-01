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
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(USER_LED_PORT, USER_LED_PIN, GPIO_PIN_RESET);

    /*Configure GPIO pin : PB13 */
    GPIO_InitStruct.Pin = USER_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USER_LED_PORT, &GPIO_InitStruct);    
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