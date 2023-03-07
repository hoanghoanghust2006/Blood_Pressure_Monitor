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
#include "menu.h"

/* Private define constants -------------------------------------------------------------*/
#define APP_TASK_DELAY_TIME_MS 10
#define PROCESS_TIMEOUT        700
#define DISPLAY_TIMEOUT        1000
#define MAX_VALUE              150
#define MIN_VALUE              0

/* Private macros -----------------------------------------------------------------------*/
static tstMenu  stMenuStart;
static tstMenu  stMenu1, stMenu2, stMenu3;
static tstMenu *stCurrentMenu = &stMenu1;

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

static void MENU_voNext(tstMenu **stCurrentMenu);
static void MENU_voBack(tstMenu **stCurrentMenu);
static void MENU_voUp(tstMenu **stCurrentMenu);
static void MENU_voDown(tstMenu **stCurrentMenu);

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
// static tenAppState enAppState = eMENU;

static tstValueMeasurement stValueMeasurement = {.u8Pressure  = 0,
                                                 .u8Systolic  = 0,
                                                 .u8Diastolic = 0,
                                                 .u8HeartBeat = 88};

/* Private functions definition   -------------------------------------------------------*/
static void APP_voTask(void *pvoArgument)
{
    MENU_enCreate(&stMenuStart, "START MENU", 0);
    MENU_enCreate(&stMenu1, "Blood pressure", 0);
    MENU_enCreate(&stMenu2, "Set time", DPL_enDisplaySetupTime);
    MENU_enCreate(&stMenu3, "History record", DPL_enDisplayRecordHistory);
    MENU_enAddLink(&stMenuStart, &stMenu1);
    MENU_enAddLink(&stMenuStart, &stMenu2);
    MENU_enAddLink(&stMenuStart, &stMenu3);
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
        // APP_voMenuStateHandler();

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
        DPL_enDisplayMenu(stCurrentMenu);
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
            DPL_enDisplayMenu(stCurrentMenu);
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
    // TODO: Hoang Hoang

    if (BTN_voGetState(eBUTTON_SELECT) == ePRESSED)
    {
        if (stCurrentMenu == &stMenu1)
        {
            enAppState = eIN_PROCESS;
        }
        else
        {
            stCurrentMenu->pvoDoWork();
        }
    }

    if (BTN_voGetState(eBUTTON_MENU) == ePRESSED)
    {
        MENU_voNext(&stCurrentMenu);
        DPL_enDisplayMenu(stCurrentMenu);
    }

    if (BTN_voGetState(eBUTTON_BACK) == ePRESSED)
    {
        MENU_voBack(&stCurrentMenu);
        DPL_enDisplayMenu(stCurrentMenu);
    }

    if (BTN_voGetState(eBUTTON_UP) == ePRESSED)
    {
        MENU_voUp(&stCurrentMenu);
        DPL_enDisplayMenu(stCurrentMenu);
    }

    if (BTN_voGetState(eBUTTON_DOWN) == ePRESSED)
    {
        MENU_voDown(&stCurrentMenu);
        DPL_enDisplayMenu(stCurrentMenu);
    }
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

static void MENU_voNext(tstMenu **stCurrentMenu)
{
    if ((*stCurrentMenu)->apstMenuList[0] != NULL)
    {
        *stCurrentMenu = (*stCurrentMenu)->apstMenuList[0];
    }
}

static void MENU_voBack(tstMenu **stCurrentMenu)
{
    if ((*stCurrentMenu)->pstParent != &stMenuStart)
    {
        (*stCurrentMenu)->pstParent->u8CurrentIndex = 0;
        *stCurrentMenu                              = (*stCurrentMenu)->pstParent;
    }
}

static void MENU_voUp(tstMenu **stCurrentMenu)
{
    *stCurrentMenu = (*stCurrentMenu)->pstParent;
    if ((*stCurrentMenu)->u8CurrentIndex == 0)
    {
        (*stCurrentMenu)->u8CurrentIndex = ((*stCurrentMenu)->u8Size);
    }
    (*stCurrentMenu)->u8CurrentIndex--;
    *stCurrentMenu = (*stCurrentMenu)->apstMenuList[(*stCurrentMenu)->u8CurrentIndex];
}

static void MENU_voDown(tstMenu **stCurrentMenu)
{
    *stCurrentMenu = (*stCurrentMenu)->pstParent;
    (*stCurrentMenu)->u8CurrentIndex++;
    if ((*stCurrentMenu)->u8CurrentIndex >= (*stCurrentMenu)->u8Size)
    {
        (*stCurrentMenu)->u8CurrentIndex = 0;
    }
    *stCurrentMenu = (*stCurrentMenu)->apstMenuList[(*stCurrentMenu)->u8CurrentIndex];
}