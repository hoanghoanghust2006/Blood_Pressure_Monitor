/*
 * Title : app_task.c
 * Copyright : HCL
 * Author : Nguyen Trung
 * Creation Date : 15/02/2023
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "common.h"
#include "trace.h"
#include "cmsis_os.h"
#include "button.h"
#include "display.h"
#include "blood_pressure.h"
#include "storage.h"

/* Private define constants -------------------------------------------------------------*/
#define APP_TASK_DELAY_TIME_MS 10
#define PROCESS_TIMEOUT        700
#define DISPLAY_TIMEOUT        1000
#define MAX_VALUE              150
#define MIN_VALUE              0

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/
typedef enum
{
    eIDLE,
    eIN_PROCESS,
    eFINISH,
    eMENU
} tenAppState;

typedef struct
{
    tenAppState enState;
    void (*voStateHandler)(void);
} tstAppStateHandler;

/* Private function prototypes declarations   -------------------------------------------*/
static void APP_voTask(void *pvoArgument);
static void APP_voIdleStateHandler(void);
static void APP_voInProcessStateHandler(void);
static void APP_voFinishStateHandler(void);
static void APP_voMenuStateHandler(void);
static void APP_voMenuSetDate(void);
static void APP_voMenuSetTime(void);

/* Private file-local global variables   ------------------------------------------------*/
static osThreadId_t         APP_pvoTaskHandle;
static const osThreadAttr_t stAppTask = {
    .name       = "AppTask",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

tstAppStateHandler stAppStateHandler[] = {
    {eIDLE, APP_voIdleStateHandler},
    {eIN_PROCESS, APP_voInProcessStateHandler},
    {eFINISH, APP_voFinishStateHandler},
    {eMENU, APP_voMenuStateHandler},
};

static tenAppState enAppState = eIDLE;

tstBloodPressureResult stResult = {eSUCCESS, 100, 100, 100};

static tstValueMeasurement stValueMeasurement = {.u8Pressure  = 0,
                                                 .u8Systolic  = 0,
                                                 .u8Diastolic = 0,
                                                 .u8HeartBeat = 88};

static tenPressureState enPressureState = eINFLATE;
static bool             bRefreshAll     = false;

/* Private functions definition   -------------------------------------------------------*/
static void APP_voTask(void *pvoArgument)
{
    for (;;)
    {
        uint32_t u32AppTaskStartTick = osKernelGetTickCount();

        /* Search in state handler lookup table for current state */
        for (uint8_t i = 0; i < sizeof(stAppStateHandler) / sizeof(tstAppStateHandler); i++)
        {
            if (enAppState == stAppStateHandler[i].enState)
            {
                /* If found, run state handler function */
                stAppStateHandler[i].voStateHandler();
                break;
            }
        }

        osDelayUntil(u32AppTaskStartTick + APP_TASK_DELAY_TIME_MS);
    }
}

/* Export functions definition   --------------------------------------------------------*/
tenStatus APP_enInitTask(void)
{
    APP_pvoTaskHandle = osThreadNew(APP_voTask, NULL, &stAppTask);
    return eSUCCESS;
}

static void APP_voIdleStateHandler(void)
{
    printf("\033\143");
    printf("\033[3J");
    bRefreshAll       = false;

    /* Event when button select is pressed */
    if (BTN_voGetState(eBUTTON_SELECT) == ePRESSED)
    {
        PRE_voRequestStartProcess();
        enAppState = eIN_PROCESS;
    }

    /* Event when button menu is pressed */
    if (BTN_voGetState(eBUTTON_MENU) == ePRESSED)
    {
        enAppState = eMENU;
    }
}

static void APP_voInProcessStateHandler(void)
{
    static uint16_t u16InProcessCount = 0;

    u16InProcessCount++;

    /* Check process timeout */
    if (u16InProcessCount <= PROCESS_TIMEOUT)
    {
        tenNotifyResponse enResponse = PRE_enGetStatusProcess(&stValueMeasurement);

        /* Check processing status */
        if (enResponse == eSUCCESSFUL)
        {
            tstStorage stNewRecord;
            stValueMeasurement.u8Pressure = 0;

            stNewRecord.u8Sys       = stResult.u8Systolic;
            stNewRecord.u8Dia       = stResult.u8Diastolic;
            stNewRecord.u8HeartRate = stResult.u8HeartBeat;
            STO_voSaveRecord(stNewRecord);

            DPL_enDisplayResults(&stResult);
            bRefreshAll = true;
            u16InProcessCount = 0;
            enAppState  = eFINISH;
        }
        else if (enResponse == eFAILED)
        {
            trace("Error\r\n");
            u16InProcessCount = 0;
            enAppState = eFINISH;
        }
        else if (enResponse == eBUSY)
        {
            /* Event when button select is pressed */
            if (BTN_voGetState(eBUTTON_SELECT) == ePRESSED)
            {
                PRE_voRequestCancelProcess();
                u16InProcessCount = 0;
                enAppState = eIDLE;
            }

            /* Check motor status */
            if (stValueMeasurement.u8Pressure == MIN_VALUE)
            {
                enPressureState = eINFLATE;
            }
            else if (stValueMeasurement.u8Pressure == MAX_VALUE)
            {
                enPressureState = eDEFLATE;
            }
            else
            {
                /* Do nothing*/
            }

            /* Display process measurement */
            if (stValueMeasurement.u8Pressure % 10 == 0 && bRefreshAll == false)
            {
                DPL_enDisplayProcessMeasurement(stValueMeasurement.u8Pressure, enPressureState, bRefreshAll);
            }
        }
        else
        {
            /*Do nothing*/
        }
    }
    else
    {
        printf("\033\143");
        printf("\033[3J");
        trace("Process timeout\r\n");
        u16InProcessCount = 0;
        enAppState        = eFINISH;
    }
}

static void APP_voFinishStateHandler(void)
{
    static uint16_t u16DisplayCount = 0;
    bRefreshAll                     = false;

    u16DisplayCount++;

    /* Check display timeout */
    if (u16DisplayCount <= DISPLAY_TIMEOUT)
    {
        /* Event when button select is pressed */
        if (BTN_voGetState(eBUTTON_SELECT) == ePRESSED)
        {
            trace("Select is pressed\r\n");
            u16DisplayCount = 0;
            enAppState      = eIDLE;
        }

        /* Event when button menu is pressed */
        if (BTN_voGetState(eBUTTON_MENU) == ePRESSED)
        {
            u16DisplayCount = 0;
            enAppState      = eMENU;
        }
    }
    else
    {
        trace("back to idle mode\r\n");
        u16DisplayCount = 0;
        enAppState = eIDLE;
    }
}

static void APP_voMenuStateHandler(void)
{
    trace("menu\r\n");
    if (BTN_voGetState(eBUTTON_BACK) == ePRESSED)
    {
        enAppState = eIDLE;
    }
    // TODO: Hoang Hoang
}

static void APP_voMenuHistory(void)
{
    // TODO:
}

static void APP_voMenuSetDate(void)
{
    // TODO:
}

static void APP_voMenuSetTime(void)
{
    // TODO:
}