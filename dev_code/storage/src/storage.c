/*
 * Title : storage.c
 * Copyright : HCL
 * Author : hoang.le
 * Creation Date : 31/08/2022
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/
#include <string.h>

/* Local Include ------------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "storage.h"
#include "common.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/
#if defined(STM32H735xx)
#define FLASH_WORD             32
#define ADDRESS_STORAGE_MEMORY 0x080E0000
#define FLASH_BANK_NUM         FLASH_BANK_1
#elif defined(STM32H7B3xxQ)
#define FLASH_WORD             16
#define ADDRESS_STORAGE_MEMORY 0x081E0000
#define FLASH_BANK_NUM         FLASH_BANK_2
#endif
#define MAX_NUM_OF_RECORDS 30
#define ERASE_DATA         0xFF

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
static tstStorage stStorage[MAX_NUM_OF_RECORDS];
static uint8_t    u8NumOfRecord = 0;

/* Private function prototypes declarations   -------------------------------------------*/
static uint32_t   STO_u32GetSector(uint32_t u32Address);
static void       STO_voEraseMemory(uint32_t u32address);
static void       STO_voSaveOneRecord(uint32_t u32address, const tstStorage *stStorage);
static void       STO_voSaveRecordsToMemory(tstStorage stStorage[], uint8_t u8Length, uint32_t u32Address);
static tstStorage STO_tstGetOneRecordFromMemory(uint32_t u32address);
static void       STO_voGetRecordsFromMemory(tstStorage stStorage[], uint8_t u8Length, uint32_t u32Address);

/* Private functions definition   -------------------------------------------------------*/
static uint32_t STO_u32GetSector(uint32_t u32Address)
{
    return (u32Address - FLASH_BASE) / FLASH_SECTOR_SIZE;
}

/* Clear a address area in flash */
static void STO_voEraseMemory(uint32_t u32address)
{
    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    uint32_t u32FirstSector = 0;
    uint32_t u32SectorError = 0;
    u32FirstSector          = STO_u32GetSector(u32address);

    /* Fill EraseInit structure*/
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks     = FLASH_BANK_NUM;
    EraseInitStruct.Sector    = u32FirstSector;
    EraseInitStruct.NbSectors = 1;
#if defined(STM32H735xx)
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_4;
#elif defined(STM32H7B3xxQ)
    EraseInitStruct.VoltageRange = 0;
#endif
    HAL_FLASHEx_Erase(&EraseInitStruct, &u32SectorError);

    uint8_t arrErase[FLASH_WORD];
    memset(arrErase, ERASE_DATA, FLASH_WORD);

    /* TODO: Fake write 0xFF to memory */
    for (uint8_t i = 0; i < MAX_NUM_OF_RECORDS; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, u32address, (uint32_t)arrErase);
        u32address += FLASH_WORD;
    }

    /* Lock the Flash to disable the flash control register access */
    HAL_FLASH_Lock();
}

/* Write data to storage */
static void STO_voSaveOneRecord(uint32_t u32address, const tstStorage *stStorage)
{
    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, u32address, (uint32_t)stStorage);

    /* Lock the Flash to disable the flash control register access */
    HAL_FLASH_Lock();
}

/* Read data to storage */
static tstStorage STO_tstGetOneRecordFromMemory(uint32_t u32address)
{
    tstStorage ret;
    memcpy((uint8_t *)&ret, (uint8_t *)u32address, sizeof(tstStorage));
    return ret;
}

/* Store measured value in flash */
static void STO_voSaveRecordsToMemory(tstStorage stStorage[], uint8_t u8Length, uint32_t u32Address)
{
    /*
     * Disable interrupt to ensure the erase and program flash
     * progress is not being interrupted by anything
     */
    __disable_irq();
    uint8_t u8NumTotalRecord = STO_u8GetNumOfRecords();

    if (u8NumTotalRecord > u8Length)
    {
        return;
    }
    trace("STO_voEraseMemory before\r\n");
    STO_voEraseMemory(u32Address);
    trace("STO_voEraseMemory after\r\n");

    /*Save each record into flash*/
    for (uint8_t u8Index = 0; u8Index < u8NumTotalRecord; u8Index++)
    {
        trace("STO_voSaveOneRecord cnt %d\r\n", u8Index);
        STO_voSaveOneRecord(u32Address, &stStorage[u8Index]);
        u32Address += FLASH_WORD;
    }
    __enable_irq();
}

/* Export functions definition   --------------------------------------------------------*/
void STO_voInit(void)
{
    STO_voGetRecordsFromMemory(stStorage, MAX_NUM_OF_RECORDS, ADDRESS_STORAGE_MEMORY);
}

void STO_voMainFunction(void)
{
}

uint8_t STO_u8GetNumOfRecords(void)
{
    return u8NumOfRecord;
}

/* Record and save the new measured value to storage */
void STO_voSaveRecord(tstStorage stNewRecord)
{
    if (u8NumOfRecord < MAX_NUM_OF_RECORDS)
    {
        stStorage[u8NumOfRecord] = stNewRecord;
        u8NumOfRecord++;
    }
    else if (u8NumOfRecord == MAX_NUM_OF_RECORDS)
    {
        trace("%s u8NumOfRecord: %d\r\n", __func__, u8NumOfRecord);
        for (uint8_t u8Index = 1; u8Index < MAX_NUM_OF_RECORDS; u8Index++)
        {
            stStorage[u8Index - 1] = stStorage[u8Index];
        }
        stStorage[u8NumOfRecord - 1] = stNewRecord;
    }
    else
    {
        /* The code should not be here */
        return;
    }

    /* Save record to memory */
    STO_voSaveRecordsToMemory(stStorage, MAX_NUM_OF_RECORDS, ADDRESS_STORAGE_MEMORY);
}

/* Get all of the record value in storage */
static void STO_voGetRecordsFromMemory(tstStorage stStorage[], uint8_t u8Length, uint32_t u32Address)
{
    tstStorage stTemp;
    u8NumOfRecord = 0;

    /*Each FLASH_WORD area is a record*/
    for (uint8_t u8Index = 0; u8Index < u8Length; u8Index++)
    {
        stTemp = STO_tstGetOneRecordFromMemory(u32Address);
        u32Address += FLASH_WORD;
        if (stTemp.stRecordTime.u8Day != ERASE_DATA)
        {
            stStorage[u8Index] = stTemp;
            u8NumOfRecord      = u8Index + 1;
        }
        else
        {
            break;
        }
    }
}

/* Get all of record to flash*/
uint8_t STO_voGetRecords(tstStorage arrStorage[], uint8_t u8NumRequestRecord)
{
    uint8_t u8ActualNumRecord = 0;

    uint8_t u8NumTotalRecord = STO_u8GetNumOfRecords();

    /*Callback value from STO_voGetRecordsFromMemory*/
    STO_voGetRecordsFromMemory(arrStorage, MAX_NUM_OF_RECORDS, ADDRESS_STORAGE_MEMORY);

    if (u8NumRequestRecord <= u8NumTotalRecord)
    {
        u8ActualNumRecord = u8NumRequestRecord;
    }
    else
    {
        u8ActualNumRecord = u8NumTotalRecord;
    }

    /*Assign value from stStorage to arrStorage*/
    for (uint8_t u8Index = 0; u8Index < u8ActualNumRecord; u8Index++)
    {
        arrStorage[u8Index] = stStorage[u8Index];
    }
    return u8ActualNumRecord;
}

/* Delete all of record in memory */
void STO_DeleteAllRecord(void)
{
    /*
     * Disable interrupt to ensure the erase
     * progress is not being interrupted by anything
     */
    __disable_irq();
    STO_voEraseMemory(ADDRESS_STORAGE_MEMORY);
    __enable_irq();

    /* Get up-to-date the buffer data (stStorage, u8NumOfRecord) from memory */
    STO_voGetRecordsFromMemory(stStorage, MAX_NUM_OF_RECORDS, ADDRESS_STORAGE_MEMORY);
}