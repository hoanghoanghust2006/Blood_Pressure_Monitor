/*
 * Title : trace.c
 * Copyright : HCL
 * Author : nguyen_trung
 * Creation Date : 20/08/2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/


/* Local Include ------------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private define constants -------------------------------------------------------------*/


/* Private macros -----------------------------------------------------------------------*/


/* Private type definitions  ------------------------------------------------------------*/


/* Private file-local global variables   ------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Private function prototypes declarations   -------------------------------------------*/


/* Private functions definition   -------------------------------------------------------*/
int _write(int file, char *ptr, int len)
{
	int rt = len;
	HAL_StatusTypeDef lStatus = HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	if (lStatus != HAL_OK)
	{
		rt = -1;
	}
	return rt;
}

/* Export functions definition   --------------------------------------------------------*/
