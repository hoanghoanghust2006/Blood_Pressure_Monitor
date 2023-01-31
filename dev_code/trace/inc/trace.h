/*
 * Title : trace.h
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

#ifndef _TRACE_H_
#define _TRACE_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/
#include <stdio.h>

/* Define constants -----------------------------------------------------------------------*/
#define TRACE_LEVEL_DEBUG 0
#define TRACE_LEVEL_INFO  1
#define TRACE_LEVEL       TRACE_LEVEL_DEBUG

#define trace(...)                            \
    {                                         \
        if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG) \
        {                                     \
            printf("[DEBUG]" __VA_ARGS__);    \
        }                                     \
    }

#define trace_line()                                                     \
    {                                                                    \
        if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)                            \
        {                                                                \
            printf("[LINE] Func: %s, line: %d\r\n", __func__, __LINE__); \
        }                                                                \
    }

#define trace_hex(str, arr, len)               \
    {                                          \
        if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)  \
        {                                      \
            uint8_t* pu8Arr = (uint8_t*)arr;   \
            printf("[HEX] %s: ", str);         \
            for (uint32_t i = 0; i < len; i++) \
            {                                  \
                printf("%02X ", pu8Arr[i]);    \
            }                                  \
            printf("\r\n");                    \
        }                                      \
    }

#define trace_info(...)                      \
    {                                        \
        if (TRACE_LEVEL >= TRACE_LEVEL_INFO) \
        {                                    \
            printf(__VA_ARGS__);             \
        }                                    \
    }

#define trace_error()                                                 \
    {                                                                 \
        printf("[ERROR] Func: %s, line: %d\r\n", __func__, __LINE__); \
    }                                                                 \

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif
