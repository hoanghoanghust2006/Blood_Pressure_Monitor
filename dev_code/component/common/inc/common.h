/*
 * Title : common.h
 * Copyright : HCL
 * Author : nguyen_trung
 * Creation Date : 07/09/2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
typedef struct
{
    uint8_t  u8Day;
    uint8_t  u8Month;
    uint16_t u16Year;
    uint8_t  u8Second;
    uint8_t  u8Minute;
    uint8_t  u8Hour;
} tstTime;

typedef enum
{
    eSUCCESS = 0,
    eFAIL,
} tenStatus;

/* Export Function Declarations -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif