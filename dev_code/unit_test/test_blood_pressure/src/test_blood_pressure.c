/*
 * Title : test_blood_pressure.c
 * Copyright : HCL
 * Author : Nam Ngo
 * Creation Date : Sep 29, 2022
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : Version 1.0 by Nam Ngo
 * ------- ---------- --------
 */

#ifdef TEST_BLOOD_PRESSURE

/* System Include -----------------------------------------------------------------------*/
#include <stdint.h>

/* Local Include ------------------------------------------------------------------------*/
#include "test_blood_pressure.h"
#include "blood_pressure.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
#if defined(STM32H735xx)
extern UART_HandleTypeDef  huart3;
static UART_HandleTypeDef *huart_pressure = &huart3;
#elif defined(STM32H7B3xxQ)
extern UART_HandleTypeDef  huart1;
static UART_HandleTypeDef *huart_pressure = &huart1;
#endif

static uint8_t u8Data_Rx[1];

osThreadId_t         PRE_pvoTaskTestHandle;
const osThreadAttr_t stPressureTaskTest = {
    .name       = "PreTaskTest",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void PRE_voTaskTest(void *pvoArgument);

/* Private functions definition   -------------------------------------------------------*/
static void PRE_voTaskTest(void *pvoArgument)
{
    /* USER CODE BEGIN 5 */
    HAL_UART_Receive_IT(huart_pressure, (uint8_t *)&u8Data_Rx, sizeof(u8Data_Rx));
    trace("*****  Test Blood Pressure  *****\r\n");
    trace("=> Enter 'S' to start run program\r\n");
    trace("=> Enter 'C' to immediately cancel\r\n");

    /* Infinite loop */
    for (;;)
    {
        osDelay(10);
    }
    /* USER CODE END 5 */
}

/* Export functions definition   --------------------------------------------------------*/
void PRE_voTaskTestInit(void)
{
    PRE_pvoTaskTestHandle = osThreadNew(PRE_voTaskTest, NULL, &stPressureTaskTest);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart_pressure->Instance)
    {
        /* Check charector received */
        switch (u8Data_Rx[0])
        {
            case 'S':
                PRE_voRequestStartProcess();
                break;
            case 'C':
                PRE_voRequestCancelProcess();
                break;
            default:
                break;
        }
        u8Data_Rx[0] = '\0';
        HAL_UART_Receive_IT(huart_pressure, (uint8_t *)&u8Data_Rx, sizeof(u8Data_Rx));
    }
}
#endif /* TEST_BLOOD_PRESSURE */