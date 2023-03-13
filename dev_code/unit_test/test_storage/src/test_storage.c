/*
 * Title : test_storage.c
 * Copyright : HCL
 * Author : Vu Hieu
 * Creation Date : 01/03/2023
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#ifdef TEST_STO

/* System Include -----------------------------------------------------------------------*/
#include <stdlib.h>
#include <ctype.h>
/* Local Include ------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "stm32l4xx_hal.h"
#include "test_storage.h"
#include "storage.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/
#define MAX_NUM_OF_RECORD 90

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
#if defined(STM32L496xx)
extern UART_HandleTypeDef  huart2;
static UART_HandleTypeDef *huart_storage = &huart2;
#elif defined(STM32L496xx)
extern UART_HandleTypeDef  huart1;
static UART_HandleTypeDef *huart_storage = &huart1;
#endif

static uint8_t       u8DataRx;
static uint8_t       u8CmdData;
static tstStorage    arrStorage[MAX_NUM_OF_RECORD];
static osThreadId_t  STO_voTestTaskHandle;
const osThreadAttr_t stStorageTask = {
    .name       = "StorageTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void STO_voTestTask(void *pvoArgument);
static void STO_voTestGetNumRecords();
static void STO_voTestGetRecords();
static void STO_voTestCreateRecords();
static void STO_voTestDeleteRecords();

/* Private lookup table    --------------------------------------------------------------*/
static tstStorageHandle stStorageHandle[] = {{'N', STO_voTestGetNumRecords},
                                             {'G', STO_voTestGetRecords},
                                             {'R', STO_voTestCreateRecords},
                                             {'D', STO_voTestDeleteRecords}};
/* Private functions definition   -------------------------------------------------------*/
static void STO_voTestTask(void *pvoArgument)
{
    HAL_UART_Receive_IT(huart_storage, (uint8_t *)&u8DataRx, sizeof(u8DataRx));
    trace("Press 'N' to get Number of record \r\n");
    trace("Press 'G' to Get all of record \r\n");
    trace("Press 'R' to Random one record \r\n");
    trace("Press 'D' to Delete all of record\r\n\n");

    /* Infinite loop */
    for (;;)
    {
        for (uint8_t i = 0; (i < sizeof(stStorageHandle) / sizeof(tstStorageHandle)) && u8CmdData != 0; i++)
        {
            if (u8CmdData == stStorageHandle[i].u8Cmd)
            {
                stStorageHandle[i].fnPtrTestHandle();
                u8CmdData = 0;
                break;
            }
        }
        osDelay(10);
    }
}

static void STO_voTestGetNumRecords()
{
    trace("Request Total Records\r\n\n");
    trace("Total Measurement : %d\r\n\n", STO_u8GetNumOfRecords());
}
static void STO_voTestGetRecords()
{
    trace("Request Read Data from Flash Memory\r\n\n");
    uint8_t u8NumActualRecord = STO_voGetRecords(arrStorage, 90);
    if (u8NumActualRecord == 0)
    {
        trace("There is no record in memory\r\n\n");
    }
    for (uint8_t u8Index = 0; u8Index < u8NumActualRecord; u8Index++)
    {
        trace("Time measurement %d -> Day:%u | Month:%u | Year:%u | Hour:%u | Minute:%u | Second:%u | Sys:%u | Dia:%u | HB:%u\r\n\n",
              (u8Index + 1),
              arrStorage[u8Index].stRecordTime.u8Day,
              arrStorage[u8Index].stRecordTime.u8Month,
              arrStorage[u8Index].stRecordTime.u16Year,
              arrStorage[u8Index].stRecordTime.u8Hour,
              arrStorage[u8Index].stRecordTime.u8Minute,
              arrStorage[u8Index].stRecordTime.u8Second,
              arrStorage[u8Index].u8Sys,
              arrStorage[u8Index].u8Dia,
              arrStorage[u8Index].u8HeartRate);
    }
}
static void STO_voTestCreateRecords()
{
    tstStorage stNewRecordTemp;
    trace("Request Random New Records\r\n\n");
    stNewRecordTemp.stRecordTime.u8Day    = (uint8_t)rand();
    stNewRecordTemp.stRecordTime.u8Month  = (uint8_t)rand();
    stNewRecordTemp.stRecordTime.u16Year  = (uint8_t)rand();
    stNewRecordTemp.stRecordTime.u8Hour   = (uint8_t)rand();
    stNewRecordTemp.stRecordTime.u8Minute = (uint8_t)rand();
    stNewRecordTemp.stRecordTime.u8Second = (uint8_t)rand();
    stNewRecordTemp.u8Sys                 = (uint8_t)rand();
    stNewRecordTemp.u8Dia                 = (uint8_t)rand();
    stNewRecordTemp.u8HeartRate           = (uint8_t)rand();
    trace("Add valued is Day:%u | Month:%u | Year:%u | Hour:%u | Minute:%u | Second:%u | Sys:%u | Dia:%u | HB:%u\r\n\n",
          stNewRecordTemp.stRecordTime.u8Day,
          stNewRecordTemp.stRecordTime.u8Month,
          stNewRecordTemp.stRecordTime.u16Year,
          stNewRecordTemp.stRecordTime.u8Hour,
          stNewRecordTemp.stRecordTime.u8Minute,
          stNewRecordTemp.stRecordTime.u8Second,
          stNewRecordTemp.u8Sys,
          stNewRecordTemp.u8Dia,
          stNewRecordTemp.u8HeartRate);
    STO_voSaveRecord(&stNewRecordTemp);
}
static void STO_voTestDeleteRecords()
{
    trace("Delete all of records\r\n\n");
    STO_DeleteAllRecord();
}
/* Export functions definition   --------------------------------------------------------*/
void STO_voTaskTestInit(void)
{
    STO_voTestTaskHandle = osThreadNew(STO_voTestTask, NULL, &stStorageTask);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart_storage->Instance)
    {
        // Value of u8DataRx is cleared after go out of callback func
        // so we need to save the value to another variable
        // and we only save if the value is an alphabet character
        if (isalpha(u8DataRx))
        {
            u8CmdData = u8DataRx;
        }
        HAL_UART_Receive_IT(huart_storage, (uint8_t *)&u8DataRx, sizeof(u8DataRx));
    }
}

#endif /* TEST_STO */