/*
 * Title : display.h
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Mar 06,2023
 * ------- ---------- --------
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/* Include --------------------------------------------------------------------------------*/
#include "common.h"
#include "menu.h"
#include "storage.h"

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
typedef enum
{
    eINFLATE,
    eDEFLATE
} tenPressureState;

typedef struct
{
    tenStatus enStatus;
    uint8_t   u8Systolic;
    uint8_t   u8HeartBeat;
    uint8_t   u8Diastolic;
} tstBloodPressureResult;

typedef enum
{
    eDAY,
    eMONTH,
    eYEAR
} tenDateSetupState;

typedef enum
{
    eHOUR,
    eMINUTE,
    eSECOND
} tenTimeSetupState;

/* Export Function Declarations -----------------------------------------------------------*/
tenStatus DPL_enInit(void);
void      DPL_voMainFunction(void);
tenStatus DPL_enDisplayProcessMeasurement(const tstTime*   stTime,
                                          uint8_t          u8AirPressure,
                                          tenPressureState enPressureState,
                                          bool             bRefreshAll);
tenStatus DPL_enDisplayResults(const tstBloodPressureResult* stResult);
tenStatus DPL_enDisplayMenu(const tstMenu* stMenu);
tenStatus DPL_enDisplayRecordHistory(const tstStorage* stStorage, uint8_t u8Index);
tenStatus DPL_enDisplaySetupDate(const tstTime* stTime, tenDateSetupState enDateSetupState);
tenStatus DPL_enDisplaySetupTime(const tstTime* stTime, tenTimeSetupState enTimeSetupState);
tenStatus DPL_enSetupDateSuccess(void);
tenStatus DPL_enSetupTimeSuccess(void);
#endif