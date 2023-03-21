/*
 * Title : storage.c
 * Copyright : HCL
 * Author : Vu Hieu
 * Creation Date : 01/03/2023
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/
#include <string.h>

/* Local Include ------------------------------------------------------------------------*/
#include "storage.h"
#include "common.h"
#include "trace.h"

#if defined(STM32H735xx) || defined(STM32H7B3xxQ)
#include "stm32h7xx_hal.h"
#elif defined(STM32L496xx)
#include "stm32l4xx_hal.h"
#endif

/* Private define constants -------------------------------------------------------------*/
#if defined(STM32H735xx)
#define ADDRESS_STORAGE_MEMORY 0x080E0000
#define FLASH_BANK_NUM         FLASH_BANK_1
#define TYPEPROGRAM            FLASH_TYPEPROGRAM_FLASHWORD
#elif defined(STM32H7B3xxQ)
#define ADDRESS_STORAGE_MEMORY 0x081E0000
#define FLASH_BANK_NUM         FLASH_BANK_2
#define TYPEPROGRAM            FLASH_TYPEPROGRAM_FLASHWORD
#elif defined(STM32L496xx)
#define ADDRESS_STORAGE_MEMORY 0x0807F800
#define FLASH_BANK_NUM         FLASH_BANK_1
#define TYPEPROGRAM            FLASH_TYPEPROGRAM_DOUBLEWORD
#endif
#define ERASE_DATA 0xFF

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
static tstStorage stStorage[MAX_NUM_OF_RECORDS];
static uint8_t    u8NumOfRecord = 0;

/*
 * Calculate the size of block that stores one record.
 * The size of block has to be mutiple of FLASH_WORD
 */
static const uint8_t u8Check     = ((sizeof(tstStorage) % FLASH_WORD) == 0) ? 0 : 1;
static const uint8_t u8SizeBlock = (sizeof(tstStorage) / FLASH_WORD + u8Check) * FLASH_WORD;

/* Private function prototypes declarations   -------------------------------------------*/
static uint32_t   STO_u32GetSegment(uint32_t u32Address);
static void       STO_voEraseMemory(uint32_t u32address);
static void       STO_voSaveOneRecord(uint32_t u32address, const tstStorage *stStorage);
static void       STO_voSaveRecordsToMemory(tstStorage stStorage[], uint8_t u8Length, uint32_t u32Address);
static tstStorage STO_tstGetOneRecordFromMemory(uint32_t u32address);
static void       STO_voGetRecordsFromMemory(tstStorage stStorage[], uint8_t u8Length, uint32_t u32Address);

/* Private functions definition   -------------------------------------------------------*/

static uint32_t STO_u32GetSegment(uint32_t u32Address)
{
#if defined(STM32H735xx) || defined(STM32H7B3xxQ)
    return (u32Address - FLASH_BASE) / FLASH_SECTOR_SIZE;
#elif defined(STM32L496xx)
    return (u32Address - FLASH_BASE) / FLASH_PAGE_SIZE;
#endif
}
/* Clear a address area in flash */
static void STO_voEraseMemory(uint32_t u32address)
{
    uint32_t u32FirstSegment = 0;
    uint32_t u32SegmentError = 0;
    u32FirstSegment          = STO_u32GetSegment(u32address);

    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    /* Fill EraseInit structure*/
#if defined(STM32H735xx)
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks        = FLASH_BANK_NUM;
    EraseInitStruct.Sector       = u32FirstSegment;
    EraseInitStruct.NbSectors    = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_4;
#elif defined(STM32H7B3xxQ)
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks        = FLASH_BANK_NUM;
    EraseInitStruct.Sector       = u32FirstSegment;
    EraseInitStruct.NbSectors    = 1;
    EraseInitStruct.VoltageRange = 0;
#elif defined(STM32L496xx)
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks     = FLASH_BANK_NUM;
    EraseInitStruct.Page      = u32FirstSegment;
    EraseInitStruct.NbPages   = 1;
#endif

    /* Start Delete Segment */
    HAL_FLASHEx_Erase(&EraseInitStruct, &u32SegmentError);

#if defined(STM32H735xx) || defined(STM32H7B3xxQ)
    /* TODO: Fake write 0xFF to memory */
    uint8_t arrErase[FLASH_WORD];
    memset(arrErase, ERASE_DATA, FLASH_WORD);
    for (uint8_t i = 0; i < MAX_NUM_OF_RECORDS; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, u32address, (uint32_t)arrErase);
        u32address += FLASH_WORD;
    }
#endif

    /* Lock the Flash to disable the flash control register access */
    HAL_FLASH_Lock();
}

/* Write data to storage */
static void STO_voSaveOneRecord(uint32_t u32address, const tstStorage *stStorage)
{
    /* Unlock the Flash */
    HAL_FLASH_Unlock();

#if defined(STM32H735xx) || defined(STM32H7B3xxQ)
    uint32_t *ptr = (uint32_t *)stStorage;
    for (uint8_t u8index = 0; u8index < (sizeof(tstStorage) / FLASH_WORD); u8index++)
    {
        HAL_FLASH_Program(TYPEPROGRAM, u32address, (uint32_t)(ptr + u8index));
    }
#elif defined(STM32L496xx)
    uint64_t *ptr                = (uint64_t *)stStorage;
    for (uint8_t u8index = 0; u8index < (sizeof(tstStorage) / FLASH_WORD); u8index++)
    {
        HAL_FLASH_Program(TYPEPROGRAM, u32address + 8 * u8index, *(ptr + u8index));
    }
#endif

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
    STO_voEraseMemory(u32Address);

    /*Save each record into flash*/
    for (uint8_t u8Index = 0; u8Index < u8NumTotalRecord; u8Index++)
    {
        STO_voSaveOneRecord(u32Address, &stStorage[u8Index]);
        u32Address += u8SizeBlock;
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
void STO_voSaveRecord(const tstStorage *stNewRecord)
{
    if (u8NumOfRecord < MAX_NUM_OF_RECORDS)
    {
        stStorage[u8NumOfRecord] = *stNewRecord;
        u8NumOfRecord++;
    }
    else if (u8NumOfRecord == MAX_NUM_OF_RECORDS)
    {
        for (uint8_t u8Index = 1; u8Index < MAX_NUM_OF_RECORDS; u8Index++)
        {
            stStorage[u8Index - 1] = stStorage[u8Index];
        }
        stStorage[u8NumOfRecord - 1] = *stNewRecord;
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
        u32Address += u8SizeBlock;
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
    STO_voGetRecordsFromMemory(stStorage, MAX_NUM_OF_RECORDS, ADDRESS_STORAGE_MEMORY);

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