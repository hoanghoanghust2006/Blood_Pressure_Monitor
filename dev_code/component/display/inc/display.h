/*
 * Title : display.h
 * Copyright : HCL
 * Author : < Name of the author. >
 * Creation Date : < Date in DD/MMM/YY format >
 * ------- ---------- --------
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/* Include --------------------------------------------------------------------------------*/
#include "common.h"

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

/* Export Function Declarations -----------------------------------------------------------*/
tenStatus DPL_enInit(void);
void      DPL_voMainFunction(void);
tenStatus DPL_enDisplayProcessMeasurement(uint8_t          u8AirPressure,
                                          tenPressureState enPressureState,
                                          bool             bRefreshAll);
tenStatus DPL_enDisplayResults(tstBloodPressureResult stResult);
tenStatus DPL_enDisplayMenu();
tenStatus DPL_enDisplayRecordHistory();
tenStatus DPL_enDisplaySetupDate();
tenStatus DPL_enDisplaySetupTime();
#endif