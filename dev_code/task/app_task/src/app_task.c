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
tstMenu  stMenuStart;
tstMenu  stMenu1, stMenu2, stMenu3;
tstMenu  stMenu1_1, stMenu1_2, stMenu1_3, stMenu2_1, stMenu2_2, stMenu2_3;
tstMenu  stMenu1_1_1, stMenu1_1_2, stMenu1_1_3;
tstMenu *stCurrentMenu;

typedef struct
{
    tstMenu *pstMenuVal;
    char     cName[MAX_CHARACTER_LENGTH];
    void (*pvoDoWork)(void);
} tstPreMenu;

typedef struct
{
    tenButtonType enButton;
    void (*pvoDoWork)(tstMenu **);
} tstScanMenu;
static tstPreMenu astPreMenu[] =
    {
        {&stMenuStart, "MenuStart", 0},
        {&stMenu1, "Menu1", 0},
        {&stMenu2, "Menu2", 0},
        {&stMenu3, "Menu3", 0},
        {&stMenu1_1, "Menu1_1", 0},
        {&stMenu1_2, "Menu1_2", 0},
        {&stMenu1_3, "Menu1_3", 0},
        {&stMenu2_1, "Menu2_1", 0},
        {&stMenu2_2, "Menu2_2", 0},
        {&stMenu2_3, "Menu2_3", 0},
        {&stMenu1_1_1, "Menu1_1_1", 0},
        {&stMenu1_1_2, "Menu1_1_2", 0},
        {&stMenu1_1_3, "Menu1_1_3", 0}};

static tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1] =
    {
        {&stMenuStart, &stMenu1, &stMenu2, &stMenu3, NULL},
        {&stMenu1, &stMenu1_1, &stMenu1_2, &stMenu1_3},
        {&stMenu1_1, &stMenu1_1_1, &stMenu1_1_2, &stMenu1_1_3},
        {&stMenu2, &stMenu2_1, &stMenu2_2, &stMenu2_3}};
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

static void MENU_voCreateAll(tstPreMenu *pastPreMenu);
static void MENU_voAddLinks(tstMenu **pastMenu);
static void MENU_voAddAllLinks(tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1]);

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

// static tenAppState enAppState = eIDLE;
static tenAppState enAppState = eMENU;

static tstValueMeasurement stValueMeasurement = {.u8Pressure  = 0,
                                                 .u8Systolic  = 0,
                                                 .u8Diastolic = 0,
                                                 .u8HeartBeat = 88};
tstScanMenu                astScanMenu[]      = {
    {eBUTTON_MENU, MENU_voNext},
    {eBUTTON_BACK, MENU_voBack},
    {eBUTTON_UP, MENU_voUp},
    {eBUTTON_DOWN, MENU_voDown},
};

/* Private functions definition   -------------------------------------------------------*/
static void APP_voTask(void *pvoArgument)
{
    for (;;)
    {
        uint32_t u32AppTaskStartTick = osKernelGetTickCount();

        /* Search in state handler lookup table for current state */
        // for (uint8_t i = 0; i < sizeof(stAppStateHandler) / sizeof(tstAppStateHandler); i++)
        // {
        //     if (enAppState == stAppStateHandler[i].enState)
        //     {
        //         /* If found, run state handler function */
        //         stAppStateHandler[i].voStateHandler();
        //         break;
        //     }
        // }
        // APP_voMenuStateHandler();
        // printf("hello\r\n");
        MENU_voCreateAll(astPreMenu);
        MENU_voAddAllLinks(apstAllMenuLink);
        stCurrentMenu = &stMenu1;
        DPL_enDisplayMenu(stCurrentMenu);

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
    // trace("menu\r\n");
    // if (BTN_voGetState(eBUTTON_BACK) == ePRESSED)
    // {
    //     enAppState = eIDLE;
    // }
    // TODO: Hoang Hoang
    MENU_voCreateAll(astPreMenu);
    MENU_voAddAllLinks(apstAllMenuLink);
    stCurrentMenu = &stMenu1;
    DPL_enDisplayMenu(stCurrentMenu);
    while (1)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            if (BTN_voGetState(astScanMenu[i].enButton) == ePRESSED)
            {
                astScanMenu[i].pvoDoWork(&stCurrentMenu);
                DPL_enDisplayMenu(stCurrentMenu);
            }
        }
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
static void MENU_voCreateAll(tstPreMenu *pastPreMenu)
{
    uint8_t i = 0;
    while ((pastPreMenu + i)->pstMenuVal != NULL)
    {
        MENU_enCreate((pastPreMenu + i)->pstMenuVal, (pastPreMenu + i)->cName, (pastPreMenu + i)->pvoDoWork);
        i++;
    }
}

/* Linking a parent option to child options*/
static void MENU_voAddLinks(tstMenu **pastMenu)
{
    uint8_t i = 1;
    while (*(pastMenu + i) != NULL)
    {
        MENU_enAddLink(*pastMenu, *(pastMenu + i));
        i++;
    }
}

/* Linking all parent options to their child options*/
static void MENU_voAddAllLinks(tstMenu *apstAllMenuLink[][MAX_MENU_LIST + 1])
{
    uint8_t i = 0;
    while ((apstAllMenuLink[i][MAX_MENU_LIST + 1]) != NULL)
    {
        MENU_voAddLinks(apstAllMenuLink[i]);
        i++;
    }
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