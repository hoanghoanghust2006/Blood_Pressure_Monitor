#include "storage.h"
#include "trace.h"

#define MAX_NUM_OF_RECORDS 90
#define RECORDS            (sizeof(stStorage) / sizeof(tstStorage))

static tstStorage stStorage[] = {
    {{8, 3, 2023, 16, 7, 4}, 100, 100, 100},
    {{16, 4, 2023, 28, 45, 12}, 150, 90, 70},
    {{7, 5, 2023, 35, 12, 3}, 110, 10, 150},
    {{31, 6, 2023, 8, 9, 12}, 190, 60, 100},
    {{12, 7, 2023, 18, 19, 2}, 123, 12, 140}};

static uint8_t u8NumOfRecords = RECORDS;

void STO_voSaveRecord(tstStorage stNewRecord)
{
    trace("Done\r\n");
}

uint8_t STO_u8GetNumOfRecords(void)
{
    return u8NumOfRecords;
}

uint8_t STO_voGetRecords(tstStorage astStorage[], uint8_t u8NumRequestRecord)
{
    uint8_t u8ActualNumRecord = 0;

    uint8_t u8NumTotalRecord = STO_u8GetNumOfRecords();

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
        astStorage[u8Index] = stStorage[u8Index];
    }
    return u8ActualNumRecord;
}

void STO_DeleteAllRecord(void)
{
    return;
}
