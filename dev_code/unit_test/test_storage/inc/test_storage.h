/*
 * Title : test_storage.h
 * Copyright : HCL
 * Author : hoang.le
 * Creation Date : 31/08/2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#ifndef _TEST_STORAGE_H_
#define _TEST_STORAGE_H_

/* Include --------------------------------------------------------------------------------*/
#include <stdint.h>
/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
typedef void (*STO_voTestHandle)();
typedef struct
{
    uint8_t          u8Cmd;
    STO_voTestHandle fnPtrTestHandle;
} tstStorageHandle;
/* Export Function Declarations -----------------------------------------------------------*/
void STO_voTaskTestInit(void);

#endif
