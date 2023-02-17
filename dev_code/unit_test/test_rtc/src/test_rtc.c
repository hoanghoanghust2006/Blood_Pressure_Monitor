/*
 * Title : test_rtc.c
 * Copyright : HCLTech
 * Author : Vu Hieu
 * Creation Date : 07/02/2023
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "trace.h"
#include "test_rtc.h"
#include "stm32l4xx_hal.h"
#include "rtc.h"
/* Private define constants -------------------------------------------------------------*/
#define RTC_TASK_DELAY_TIME_MS 5
/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

RTC_HandleTypeDef hrtc;

/* Private file-local global variables   ------------------------------------------------*/
osThreadId_t         RTC_pvoTaskHandle;
const osThreadAttr_t stRtcTask = {
    .name       = "RtclateTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void RTC_voTask(void* pvoArgument);
// static void MX_RTC_Init(void);

/* Private functions definition   -------------------------------------------------------*/
static void RTC_voTask(void* pvoArgument)
{
    // MX_RTC_Init();
    // RTC_DateTypeDef sDate;
    // RTC_TimeTypeDef sTime;
    tstTime tstGetTime;
    for (;;)
    {
        RTC_enGetDateTime(&tstGetTime);
        /* Get the RTC current Time */
        // HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        // /* Get the RTC current Date */
        // HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
        /* Display time Format: hh:mm:ss */
        trace("TIME: %02d:%02d:%02d\r\n", tstGetTime.u8Hour, tstGetTime.u8Minute, tstGetTime.u8Second);
        /* Display date Format: dd-mm-yy */
        trace("DATE: %02d-%02d-%2d\r\n", tstGetTime.u8Day, tstGetTime.u8Month, tstGetTime.u16Year + 2000);
        osDelay(1000);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void RTC_voTaskTestInit(void)
{
    RTC_pvoTaskHandle = osThreadNew(RTC_voTask, NULL, &stRtcTask);
}

// void MX_RTC_Init(void)
// {
//     /* USER CODE BEGIN RTC_Init 0 */

//     /* USER CODE END RTC_Init 0 */

//     RTC_TimeTypeDef sTime = {0};
//     RTC_DateTypeDef sDate = {0};

//     /* USER CODE BEGIN RTC_Init 1 */

//     /* USER CODE END RTC_Init 1 */

//     /* Initialize RTC Only */

//     hrtc.Instance            = RTC;
//     hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;
//     hrtc.Init.AsynchPrediv   = 127;
//     hrtc.Init.SynchPrediv    = 255;
//     hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;
//     hrtc.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
//     hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//     hrtc.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
//     if (HAL_RTC_Init(&hrtc) != HAL_OK)
//     {
//         //  Error_Handler();
//         trace("[RTC] - INIT ERROR\r\n");
//     }

//     /* USER CODE BEGIN Check_RTC_BKUP */

//     /* USER CODE END Check_RTC_BKUP */

//     /* Initialize RTC and set the Time and Date */

//     sTime.Hours          = 0x10;
//     sTime.Minutes        = 0x00;
//     sTime.Seconds        = 0x00;
//     sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//     sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//     if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
//     {
//         // Error_Handler();
//         trace("[RTC] -SET TIME ERRO\r\nR");
//     }
//     sDate.WeekDay = RTC_WEEKDAY_MONDAY;
//     sDate.Month   = RTC_MONTH_FEBRUARY;
//     sDate.Date    = 0x14;
//     sDate.Year    = 0x23;

//     if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
//     {
//         // Error_Handler();
//         trace("[RTC] - SET DATE ERROR\r\n");
//     }
//     /* USER CODE BEGIN RTC_Init 2 */

//     HAL_RTC_MspInit(&hrtc);

//     /* USER CODE END RTC_Init 2 */
// }

// void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
// {
//     RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//     if (rtcHandle->Instance == RTC)
//     {
//         /** Initializes the peripherals clock
//          */
//         PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
//         PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
//         if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//         {
//             trace_error();
//         }

//         /* RTC clock enable */
//         __HAL_RCC_RTC_ENABLE();
//     }
// }

// // void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
// // {
// //     if (rtcHandle->Instance == RTC)
// //     {
// //         /* Peripheral clock disable */
// //         __HAL_RCC_RTC_DISABLE();
// //     }
// // }