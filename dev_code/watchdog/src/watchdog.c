/*
 * Title :  watch.c
 * Copyright : HCL
 * Author :  minh.thach
 * Creation Date :  01/09/2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "watchdog.h"
#include "stm32h7xx_hal.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
extern IWDG_HandleTypeDef hiwdg1;

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/

/* Export functions definition   --------------------------------------------------------*/
void WDG_voInit(void)
{
}

void WDG_voMainFunction(void)
{
    /*
     *  set up 1 second Refresh WDG
     *  RL = (Time(ms)*32000)/(PR*1000) - 1
     *  RL is IWDG down-counter reload value
     *  PR is IWDG Counter clock prescaler
     *  IWDG down-counter reload value must be between 0 and 4095
     *  Choose Time =5000, PR = 64 => RL = 2499
     */
    HAL_IWDG_Refresh(&hiwdg1);
}