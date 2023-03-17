/*
 * Title : Storage.h
 * Copyright : HCL
 * Author : Vu Hieu
 * Creation Date : 01/03/2023
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#ifndef _STORAGE_H_
#define _STORAGE_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/
#include <stdint.h>
#include "common.h"

/* Define constants -----------------------------------------------------------------------*/
#define MAX_NUM_OF_RECORDS 90

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef struct
    {
        tstTime stRecordTime;
        uint8_t u8Sys;
        uint8_t u8Dia;
        uint8_t u8HeartRate;
    } __attribute__((packed, aligned(8))) tstStorage;

    /* Export Function Declarations -----------------------------------------------------------*/
    void    STO_voInit(void);
    void    STO_voMainFunction(void);
    void    STO_voSaveRecord(const tstStorage *stNewRecord);
    uint8_t STO_u8GetNumOfRecords(void);
    uint8_t STO_voGetRecords(tstStorage arrStorage[], uint8_t u8NumRequestRecord);
    void    STO_DeleteAllRecord(void);

#ifdef __cplusplus
}
#endif
#endif /* _STORAGE_H_ */
