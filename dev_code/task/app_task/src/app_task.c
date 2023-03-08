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
#include "rtc.h"

/* Private define constants -------------------------------------------------------------*/
#define APP_TASK_DELAY_TIME_MS 10
#define PROCESS_TIMEOUT        700
#define DISPLAY_TIMEOUT        1000
#define MAX_VALUE              150
#define MIN_VALUE              0

#define FIRST_DAY   1
#define LAST_DAY    31
#define FIRST_MONTH 1
#define LAST_MONTH  12
#define FIRST_YEAR  0
#define LAST_YEAR   4000

#define FIRST_HOUR   0
#define LAST_HOUR    23
#define FIRST_MINUTE 0
#define LAST_MINUTE  59
#define FIRST_SECOND 0
#define LAST_SECOND  59

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

typedef enum
{
    eINCREASE,
    eDECREASE
} tenOperator;

/* Private function prototypes declarations   -------------------------------------------*/
static void APP_voTask(void *pvoArgument);
static void APP_voIdleStateHandler(void);
static void APP_voInProcessStateHandler(void);
static void APP_voFinishStateHandler(void);
static void APP_voMenuStateHandler(void);
static void APP_voMenuSetDate(void);
static void APP_voMenuSetTime(void);

static tenStatus enAdjustValueU16(uint16_t *pu16Value, uint8_t u8Max, uint8_t u8Min, tenOperator enOperator);
static tenStatus enAdjustValueU8(uint8_t *pu8Value, uint8_t u8Max, uint8_t u8Min, tenOperator enOperator);

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

static tstValueMeasurement stValueMeasurement = {.u8Pressure  = 0,
                                                 .u8Systolic  = 0,
                                                 .u8Diastolic = 0,
                                                 .u8HeartBeat = 88};

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
    static uint16_t         u16InProcessCount = 0;
    static tenPressureState enPressureState   = eINFLATE;
    static bool             bRefreshAll       = false;
    tstStorage              stNewRecord;
    tstBloodPressureResult  stResult;

    u16InProcessCount++;

    /* Check process timeout */
    if (u16InProcessCount <= PROCESS_TIMEOUT)
    {
        tenNotifyResponse enResponse = PRE_enGetStatusProcess(&stValueMeasurement);

        /* Check processing status */
        if (enResponse == eSUCCESSFUL)
        {
            stResult.enStatus    = eSUCCESS;
            stResult.u8Systolic  = stValueMeasurement.u8Systolic;
            stResult.u8Diastolic = stValueMeasurement.u8Diastolic;
            stResult.u8HeartBeat = stValueMeasurement.u8HeartBeat;

            tenStatus enResultDateTime = RTC_enGetDateTime(&(stNewRecord.stRecordTime));
            if (enResultDateTime == eFAIL)
            {
                // TO DO:
            }
            else
            {
                /* Do nothing */
            }

            stNewRecord.u8Sys       = stResult.u8Systolic;
            stNewRecord.u8Dia       = stResult.u8Diastolic;
            stNewRecord.u8HeartRate = stResult.u8HeartBeat;
            STO_voSaveRecord(stNewRecord);

            DPL_enDisplayResults(&stResult);
            u16InProcessCount = 0;
            bRefreshAll       = true;
            enAppState  = eFINISH;
        }
        else if (enResponse == eFAILED)
        {
            trace("Error\r\n");
            u16InProcessCount = 0;
            bRefreshAll       = true;
            enAppState = eFINISH;
        }
        else if (enResponse == eBUSY)
        {
            /* Event when button select is pressed */
            if (BTN_voGetState(eBUTTON_SELECT) == ePRESSED)
            {
                PRE_voRequestCancelProcess();
                u16InProcessCount = 0;
                bRefreshAll       = true;
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

            if (stValueMeasurement.u8Pressure == 150)
            {
                bRefreshAll = true;
            }
            else
            {
                bRefreshAll = false;
            }

            /* Display process measurement */
            if (stValueMeasurement.u8Pressure % 10 == 0)
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
        bRefreshAll       = true;
        enAppState        = eFINISH;
    }
}

static void APP_voFinishStateHandler(void)
{
    static uint16_t u16DisplayCount = 0;

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
    static bool              bFlagGetDate = true;
    static tstTime           stSetDate;
    static tenDateSetupState enState = eDAY;
    tenOperator              enOperator;
    tenButtonState           enUpBtnState     = BTN_voGetState(eBUTTON_UP);
    tenButtonState           enDownBtnState   = BTN_voGetState(eBUTTON_DOWN);
    tenButtonState           enSelectBtnState = BTN_voGetState(eBUTTON_SELECT);

    /* Get current date */
    if (bFlagGetDate == true)
    {
        RTC_enGetDateTime(&stSetDate);
        trace("%d %d %d\r\n", stSetDate.u8Day, stSetDate.u8Month, stSetDate.u16Year);
        bFlagGetDate = false;
    }

    /* Event when button up is pressed or held */
    if (enUpBtnState != eNONE)
    {
        enOperator = eINCREASE;

        /* Increase day value */
        if (enState == eDAY)
        {
            enAdjustValueU8(&(stSetDate.u8Day), LAST_DAY, FIRST_DAY, enOperator);
        }

        /* Increase month value */
        if (enState == eMONTH)
        {
            enAdjustValueU8(&(stSetDate.u8Month), LAST_MONTH, FIRST_MONTH, enOperator);
        }

        /* Increase year value */
        if (enState == eYEAR)
        {
            enAdjustValueU16(&(stSetDate.u16Year), LAST_YEAR, FIRST_YEAR, enOperator);
        }
    }

    /* Event when button down is pressed or held */
    if (enDownBtnState != eNONE)
    {
        enOperator = eDECREASE;

        /* Decrease day value */
        if (enState == eDAY)
        {
            enAdjustValueU8(&(stSetDate.u8Day), LAST_DAY, FIRST_DAY, enOperator);
        }

        /* Decrease month value */
        if (enState == eMONTH)
        {
            enAdjustValueU8(&(stSetDate.u8Month), LAST_MONTH, FIRST_MONTH, enOperator);
        }

        /* Decrease year value */
        if (enState == eYEAR)
        {
            enAdjustValueU16(&(stSetDate.u16Year), LAST_YEAR, FIRST_YEAR, enOperator);
        }
    }

    /* Display setup date */
    if ((enUpBtnState != eNONE) || (enDownBtnState != eNONE) || (enSelectBtnState != eNONE))
    {
        DPL_enDisplaySetupDate(&stSetDate, enState);
    }

    /* Even when button select is pressed */
    if (enSelectBtnState == ePRESSED)
    {
        printf("\033\143");
        printf("\033[3J");

        /* Check condition to save date */
        if (enState < eYEAR)
        {
            /* Move to next setup state */
            enState++;
            trace("%d %d %d\r\n", stSetDate.u8Day, stSetDate.u8Month, stSetDate.u16Year);
        }
        else
        {
            printf("\033\143");
            printf("\033[3J");
            RTC_enSetDateTime(&stSetDate);
            trace("%d %d %d\r\n", stSetDate.u8Day, stSetDate.u8Month, stSetDate.u16Year);
            trace("Save time: Done \r\n");
            enState      = eDAY;
            bFlagGetDate = true;
        }
    }
}

static void APP_voMenuSetTime(void)
{
    static bool              bFlagGetTime = true;
    static tstTime           stSetTime;
    static tenTimeSetupState enState = eHOUR;
    tenOperator              enOperator;
    tenButtonState           enUpBtnState     = BTN_voGetState(eBUTTON_UP);
    tenButtonState           enDownBtnState   = BTN_voGetState(eBUTTON_DOWN);
    tenButtonState           enSelectBtnState = BTN_voGetState(eBUTTON_SELECT);

    /* Get current time */
    if (bFlagGetTime == true)
    {
        RTC_enGetDateTime(&stSetTime);
        trace("%d %d %d\r\n", stSetTime.u8Hour, stSetTime.u8Minute, stSetTime.u8Second);
        bFlagGetTime = false;
    }

    /* Event when button up is pressed or held */
    if (enUpBtnState != eNONE)
    {
        enOperator = eINCREASE;

        /* Increase hour value */
        if (enState == eHOUR)
        {
            enAdjustValueU8(&(stSetTime.u8Hour), LAST_HOUR, FIRST_HOUR, enOperator);
        }

        /* Increase minute value */
        if (enState == eMINUTE)
        {
            enAdjustValueU8(&(stSetTime.u8Minute), LAST_MINUTE, FIRST_MINUTE, enOperator);
        }

        /* Increase second value */
        if (enState == eSECOND)
        {
            enAdjustValueU8(&(stSetTime.u8Second), LAST_SECOND, FIRST_SECOND, enOperator);
        }
    }

    /* Event when button down is pressed or held */
    if (enDownBtnState != eNONE)
    {
        enOperator = eDECREASE;

        /* Decrease hour value */
        if (enState == eHOUR)
        {
            enAdjustValueU8(&(stSetTime.u8Hour), LAST_HOUR, FIRST_HOUR, enOperator);
        }

        /* Decrease minute value */
        if (enState == eMINUTE)
        {
            enAdjustValueU8(&(stSetTime.u8Minute), LAST_MINUTE, FIRST_MINUTE, enOperator);
        }

        /* Decrease second value */
        if (enState == eSECOND)
        {
            enAdjustValueU8(&(stSetTime.u8Second), LAST_SECOND, FIRST_SECOND, enOperator);
        }
    }

    /* Display setup time */
    if ((enUpBtnState != eNONE) || (enDownBtnState != eNONE) || (enSelectBtnState != eNONE))
    {
        DPL_enDisplaySetupTime(&stSetTime, enState);
    }

    /* Even when button select is pressed */
    if (enSelectBtnState == ePRESSED)
    {
        /* Check condition to save time */
        if (enState < eSECOND)
        {
            /* Move to next setup state */
            enState++;
            trace("%d %d %d\r\n", stSetTime.u8Hour, stSetTime.u8Minute, stSetTime.u8Second);
        }
        else
        {
            printf("\033\143");
            printf("\033[3J");
            RTC_enSetDateTime(&stSetTime);
            trace("%d %d %d\r\n", stSetTime.u8Hour, stSetTime.u8Minute, stSetTime.u8Second);
            trace("Save time: Done \r\n");
            enState      = eHOUR;
            bFlagGetTime = true;
        }
    }
}

static tenStatus enAdjustValueU16(uint16_t *pu16Value, uint8_t u8Max, uint8_t u8Min, tenOperator enOperator)
{
    if (enOperator == eINCREASE)
    {
        if (*pu16Value == u8Max)
        {
            *pu16Value = u8Min;
        }
        else
        {
            (*pu16Value)++;
        }
    }

    if (enOperator == eDECREASE)
    {
        if (*pu16Value == u8Min)
        {
            *pu16Value = u8Max;
        }
        else
        {
            (*pu16Value)--;
        }
    }
}

static tenStatus enAdjustValueU8(uint8_t *pu8Value, uint8_t u8Max, uint8_t u8Min, tenOperator enOperator)
{
    if (enOperator == eINCREASE)
    {
        trace("%d\r\n", *pu8Value);
        if (*pu8Value == u8Max)
        {
            *pu8Value = u8Min;
        }
        else
        {
            (*pu8Value)++;
        }
    }

    if (enOperator == eDECREASE)
    {
        if (*pu8Value == u8Min)
        {
            *pu8Value = u8Max;
        }
        else
        {
            (*pu8Value)--;
        }
    }
}