/*
 * Title : display.c
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date :  Mar 06,2023
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/
#include <string.h>

/* Local Include ------------------------------------------------------------------------*/
#include "trace.h"
#include "image.h"
#include "st7789.h"
#include "display.h"

/* Private define constants -------------------------------------------------------------*/
#define SIZE_OF_BUFFER 20

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/
static tenStatus DPL_enClearScreen(void);
static tenStatus DPL_enHomePage(void);
static tenStatus DPL_enTimeFormat(const tstTime* stTime, char* acBuffer, uint16_t u16BufferSize);
static tenStatus DPL_enDateFormat(const tstTime* stTime, char* acBuffer, uint16_t u16BufferSize);

/* Private functions definition   -------------------------------------------------------*/
/* Clear the screen */
static tenStatus DPL_enClearScreen(void)
{
    ST7789_voDrawImage(0, 0, 240, 240, (uint16_t*)au8Clear240x240);
    return eSUCCESS;
}

/* Display the home page*/
static tenStatus DPL_enHomePage(void)
{
    ST7789_voWriteString(30, 50, "SYS:", stFont16x26, BLACK, WHITE);
    ST7789_voWriteString(30, 90, "DIA:", stFont16x26, BLACK, WHITE);
    ST7789_voWriteString(30, 130, "PUL:", stFont16x26, BLACK, WHITE);
    ST7789_voWriteString(165, 56, "(mmHg)", stFont11x18, BLACK, WHITE);
    ST7789_voWriteString(165, 96, "(mmHg)", stFont11x18, BLACK, WHITE);

    return eSUCCESS;
}

/* Private functions definition   -------------------------------------------------------*/
/**
 * Write time to string.
 */
static tenStatus DPL_enTimeFormat(const tstTime* stTime, char* acBuffer, uint16_t u16BufferSize)
{
    snprintf(acBuffer, u16BufferSize, "%02d:%02d:%02d", stTime->u8Hour, stTime->u8Minute, stTime->u8Second);
    return eSUCCESS;
}

/**
 * Write date to string.
 */
static tenStatus DPL_enDateFormat(const tstTime* stTime, char* acBuffer, uint16_t u16BufferSize)
{
    snprintf(acBuffer, u16BufferSize, "%02d-%02d-%04d", stTime->u8Day, stTime->u8Month, stTime->u16Year);
    return eSUCCESS;
}

/* Export functions definition   --------------------------------------------------------*/
/**
 * Display process Measurement
 * u8AirPressure -> Systolic blood pressure
 * enPressureState -> pump and discharge status
 * bRefreshAll -> clear value or entire screen
 */
tenStatus DPL_enDisplayProcessMeasurement(const tstTime*   stTime,
                                          uint8_t          u8AirPressure,
                                          tenPressureState enPressureState,
                                          bool             bRefreshAll)
{
    /* Array to stores the air pressure value */
    char acBuffer[SIZE_OF_BUFFER];

    /* Check refesh state */
    if (bRefreshAll)
    {
        /* Clear entire screen */
        DPL_enClearScreen();

        /* Redisplay the home page */
        DPL_enHomePage();
    }
    DPL_enHomePage();

    /* Display time */
    DPL_enTimeFormat(stTime, acBuffer, SIZE_OF_BUFFER);
    ST7789_voWriteString(150, 1, acBuffer, stFont11x18, BLACK, WHITE);

    /* Display date */
    DPL_enDateFormat(stTime, acBuffer, SIZE_OF_BUFFER);
    ST7789_voWriteString(1, 1, acBuffer, stFont11x18, BLACK, WHITE);

    /* Stores the air pressure value into array */
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", u8AirPressure);

    /* Check the Pressure State */
    if (enPressureState == eINFLATE)
    {
        ST7789_voFill(100, 50, 160, 76, WHITE);
        ST7789_voWriteString(100, 50, acBuffer, stFont16x26, BLACK, WHITE);
        ST7789_voDrawImage(40, 170, 50, 50, (uint16_t*)au8UpIcon50x50);
    }
    else
    {
        ST7789_voFill(100, 50, 160, 76, WHITE);
        ST7789_voWriteString(100, 50, acBuffer, stFont16x26, BLACK, WHITE);
        ST7789_voDrawImage(40, 170, 50, 50, (uint16_t*)au8DownIcon50x50);
        ST7789_voDrawImage(150, 170, 50, 50, (uint16_t*)au8PulseBmp);
    }
    return eSUCCESS;
}

/**
 * Display Blood Pressure Result
 * stResult -> heart rate measurements
 */
tenStatus DPL_enDisplayResults(const tstBloodPressureResult* stResult)
{
    /* Array to stores the measured values */
    char acBuffer[SIZE_OF_BUFFER];

    /* Clear the up down icons */
    ST7789_voFill(40, 170, 90, 220, WHITE);

    /* Store and display the value on the screen */
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", stResult->u8Diastolic);
    ST7789_voWriteString(100, 90, acBuffer, stFont16x26, BLACK, WHITE);
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", stResult->u8Systolic);
    ST7789_voWriteString(100, 50, acBuffer, stFont16x26, BLACK, WHITE);
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", stResult->u8HeartBeat);
    ST7789_voWriteString(100, 130, acBuffer, stFont16x26, BLACK, WHITE);

    return eSUCCESS;
}

/**
 * Display menu
 * stMenu -> menu structure
 */
tenStatus DPL_enDisplayMenu(const tstMenu* stMenu)
{
    /* Variable to stores the previous index */
    static uint8_t u8PreIndex = 0;

    /* Clear entire screen */
    DPL_enClearScreen();

    /* Display main menu name */
    ST7789_voWriteString(45, 35, stMenu->cName, stFont16x26, BLACK, WHITE);

    /* Display sub menu name and setting icons on the screen */
    for (uint8_t u8Index = 0; u8Index < stMenu->u8Size; u8Index++)
    {
        ST7789_voWriteString(36, 35 + (42 * (u8Index + 1)), stMenu->apstMenuList[u8Index]->cName, stFont11x18, BLACK, WHITE);
        ST7789_voDrawImage(3, 32 + (40 * (u8Index + 1)), 30, 30, (uint16_t*)au8SettingIcon30x30);
    }

    /* Clear the unselected options */
    ST7789_voDrawRectangle(2, 22 + (42 * u8PreIndex), 237, 22 + (42 * u8PreIndex) + 40, WHITE);

    /* Display selected option */
    ST7789_voDrawRectangle(2, 22 + (42 * (stMenu->u8CurrentIndex + 1)), 237, 22 + (42 * (stMenu->u8CurrentIndex + 1)) + 40, BROWN);

    /* Save current index */
    u8PreIndex = stMenu->u8CurrentIndex + 1;

    return eSUCCESS;
}

/**
 * Display history of records.
 */
tenStatus DPL_enDisplayRecordHistory(const tstStorage* stStorage, uint8_t u8Index)
{
    char acBuffer[SIZE_OF_BUFFER];

    /* Clear screen */
    ST7789_voDrawImage(0, 0, 240, 240, (uint16_t*)au8Clear240x240);

    /* Display History feature */
    ST7789_voFill(5, 15, 235, 37, BLACK);
    ST7789_voWriteString(20, 17, "History of Records", stFont11x18, WHITE, BLACK);

    /* Display SYS index */
    ST7789_voWriteString(30, 50, "SYS:", stFont16x26, BLACK, WHITE);
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", stStorage->u8Sys);
    ST7789_voWriteString(100, 50, acBuffer, stFont16x26, RED, WHITE);
    ST7789_voWriteString(154, 56, "(mmHg)", stFont11x18, BLACK, WHITE);

    /* Display DIA index */
    ST7789_voWriteString(30, 90, "DIA:", stFont16x26, BLACK, WHITE);
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", stStorage->u8Dia);
    ST7789_voWriteString(100, 90, acBuffer, stFont16x26, RED, WHITE);
    ST7789_voWriteString(154, 96, "(mmHg)", stFont11x18, BLACK, WHITE);

    /* Display Pulse index */
    ST7789_voWriteString(30, 130, "Pul:", stFont16x26, BLACK, WHITE);
    snprintf(acBuffer, SIZE_OF_BUFFER, "%d", stStorage->u8HeartRate);
    ST7789_voWriteString(100, 130, acBuffer, stFont16x26, RED, WHITE);

    /* Display index of record in the history */
    ST7789_voWriteString(70, 163, "Index:", stFont11x18, BLACK, WHITE);
    snprintf(acBuffer, SIZE_OF_BUFFER, "%02d", u8Index);
    ST7789_voWriteString(140, 163, acBuffer, stFont11x18, BLUE, WHITE);

    /* Display time of record */
    DPL_enTimeFormat(&(stStorage->stRecordTime), acBuffer, SIZE_OF_BUFFER);
    ST7789_voWriteString(40, 188, "Time:", stFont11x18, BLACK, WHITE);
    ST7789_voWriteString(100, 188, acBuffer, stFont11x18, BLACK, WHITE);

    /* Display date of record */
    DPL_enDateFormat(&(stStorage->stRecordTime), acBuffer, SIZE_OF_BUFFER);
    ST7789_voWriteString(30, 212, "Date:", stFont11x18, BLACK, WHITE);
    ST7789_voWriteString(90, 212, acBuffer, stFont11x18, BLACK, WHITE);
    return eSUCCESS;
}

/**
 * Display setup Date feature.
 */
tenStatus DPL_enDisplaySetupDate(const tstTime* stTime, tenDateSetupState enDateSetupState)
{
    char acBuffer[SIZE_OF_BUFFER];

    /* Clear screen */
    ST7789_voDrawImage(0, 0, 240, 240, (uint16_t*)au8Clear240x240);

    /* Display Set date feature */
    ST7789_voWriteString(55, 40, "Set Date", stFont16x26, RED, WHITE);
    DPL_enDateFormat(stTime, acBuffer, SIZE_OF_BUFFER);
    ST7789_voWriteString(45, 110, acBuffer, stFont16x26, BLACK, WHITE);

    switch (enDateSetupState)
    {
        case eDAY:
            /* Fill black box on day index */
            ST7789_voFill(43, 104, 79, 138, BLACK);
            snprintf(acBuffer, SIZE_OF_BUFFER, "%02d", stTime->u8Day);
            ST7789_voWriteString(45, 110, acBuffer, stFont16x26, WHITE, BLACK);
            break;
        case eMONTH:
            /* Fill black box on Month index */
            ST7789_voFill(91, 104, 127, 138, BLACK);
            snprintf(acBuffer, SIZE_OF_BUFFER, "%02d", stTime->u8Month);
            ST7789_voWriteString(93, 110, acBuffer, stFont16x26, WHITE, BLACK);
            break;
        case eYEAR:
            /* Fill black box on Year index */
            ST7789_voFill(139, 104, 207, 138, BLACK);
            snprintf(acBuffer, SIZE_OF_BUFFER, "%04d", stTime->u16Year);
            ST7789_voWriteString(141, 110, acBuffer, stFont16x26, WHITE, BLACK);
            break;
        default:
            break;
    }
    return eSUCCESS;
}

/**
 * Display setup Time feature.
 */
tenStatus DPL_enDisplaySetupTime(const tstTime* stTime, tenTimeSetupState enTimeSetupState)
{
    char acBuffer[SIZE_OF_BUFFER];

    /* Clear screen */
    ST7789_voDrawImage(0, 0, 240, 240, (uint16_t*)au8Clear240x240);

    /* Display Set time feature */
    ST7789_voWriteString(55, 40, "Set Time", stFont16x26, RED, WHITE);
    DPL_enTimeFormat(stTime, acBuffer, SIZE_OF_BUFFER);
    ST7789_voWriteString(60, 110, acBuffer, stFont16x26, BLACK, WHITE);
    switch (enTimeSetupState)
    {
        case eHOUR:
            /* Fill black box on Hour index */
            ST7789_voFill(58, 104, 94, 138, BLACK);
            snprintf(acBuffer, SIZE_OF_BUFFER, "%02d", stTime->u8Hour);
            ST7789_voWriteString(60, 110, acBuffer, stFont16x26, WHITE, BLACK);
            break;
        case eMINUTE:
            /* Fill black box on Minute index */
            ST7789_voFill(106, 104, 142, 138, BLACK);
            snprintf(acBuffer, SIZE_OF_BUFFER, "%02d", stTime->u8Minute);
            ST7789_voWriteString(108, 110, acBuffer, stFont16x26, WHITE, BLACK);
            break;
        case eSECOND:
            /* Fill black box on Second index */
            ST7789_voFill(154, 104, 190, 138, BLACK);
            snprintf(acBuffer, SIZE_OF_BUFFER, "%02d", stTime->u8Second);
            ST7789_voWriteString(156, 110, acBuffer, stFont16x26, WHITE, BLACK);
            break;
        default:
            break;
    }
    return eSUCCESS;
}

/**
 * Display setup Date successfully.
 */
tenStatus DPL_enSetupDateSuccess(void)
{
    ST7789_voDrawImage(0, 0, 240, 240, (uint16_t*)au8Clear240x240);
    ST7789_voWriteString(55, 40, "Set Date", stFont16x26, RED, WHITE);
    ST7789_voWriteString(25, 110, "Successfully", stFont16x26, BLACK, WHITE);
    return eSUCCESS;
}

/**
 * Display setup Time successfully.
 */
tenStatus DPL_enSetupTimeSuccess(void)
{
    ST7789_voDrawImage(0, 0, 240, 240, (uint16_t*)au8Clear240x240);
    ST7789_voWriteString(55, 40, "Set Time", stFont16x26, RED, WHITE);
    ST7789_voWriteString(25, 110, "Successfully", stFont16x26, BLACK, WHITE);
    return eSUCCESS;
}
