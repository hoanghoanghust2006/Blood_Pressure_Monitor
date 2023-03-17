/*
 * Title : test_display_2.h
 * Copyright : HCL
 * Author : Nguyen Duc
 * Creation Date : Feb 20, 2022
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */
#ifdef TEST_DISPLAY_2

/* System Include -----------------------------------------------------------------------*/
#include <string.h>

/* Local Include ------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "common.h"
#include "trace.h"
#include "st7789.h"
#include "image.h"
#include "display.h"

/* Private define constants -------------------------------------------------------------*/
#define MAX_MENU_LIST 5

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/
/* Initialize time and date values */
tstTime stTime = {.u8Day    = 6,
                  .u8Month  = 3,
                  .u16Year  = 2023,
                  .u8Second = 01,
                  .u8Minute = 44,
                  .u8Hour   = 2};

/* Initialize measured values */
tstBloodPressureResult astResult = {.u8Systolic  = 136,
                                    .u8HeartBeat = 157,
                                    .u8Diastolic = 178};

/* Initialize main menu */
tstMenu astMenu;

/* Initialize sub menus */
tstMenu astMenu1;
tstMenu astMenu2;
tstMenu astMenu3;
tstMenu astMenu4;
tstMenu astMenu5;

/* Initialize array stores sub menus */
tstMenu* apstSubMenuList[MAX_MENU_LIST] = {&astMenu1, &astMenu2, &astMenu3, &astMenu4, &astMenu5};

osThreadId_t         DISPLAY_2_pvoTaskHandle;
const osThreadAttr_t stDISPLAY2TaskTest = {
    .name       = "StTaskTest",
    .stack_size = 1024 * 4,
    .priority   = (osPriority_t)osPriorityLow,
};

/* Private function prototypes declarations   -------------------------------------------*/
static void DISPLAY_2_voTaskTest(void* pvoArgument);
static void DISPLAY_2_voTest(void);

/* Private functions definition   -------------------------------------------------------*/
static void DISPLAY_2_voTest(void)
{
    /* Initialize menu values */
    sprintf(astMenu.cName, "Main Menu");
    astMenu.pstParent = NULL;
    astMenu.u8Size    = 3;

    /* Assign array values into sub menu array */
    for (uint8_t u8Index = 0; u8Index < 5; u8Index++)
    {
        astMenu.apstMenuList[u8Index] = apstSubMenuList[u8Index];
    }
    sprintf(astMenu1.cName, "History of Records");
    sprintf(astMenu2.cName, "Set Date");
    sprintf(astMenu3.cName, "Set Time");

    /* Display measured air pressures when increasing */
    DPL_enDisplayProcessMeasurement(&stTime, 138, eINFLATE, true);
    for (uint8_t u8Pressure = 139; u8Pressure <= 144; u8Pressure++)
    {
        stTime.u8Second++;
        DPL_enDisplayProcessMeasurement(&stTime, u8Pressure, eINFLATE, false);
        osDelay(1000);
    }

    /* Display measured air pressures when decreasing */
    for (uint8_t u8Pressure = 143; u8Pressure >= 140; u8Pressure--)
    {
        stTime.u8Second++;
        DPL_enDisplayProcessMeasurement(&stTime, u8Pressure, eDEFLATE, false);
        osDelay(1000);
    }

    /* Display measured value on the screen */
    DPL_enDisplayResults(&astResult);
    osDelay(3000);

    /* Assign other current index */
    astMenu.u8CurrentIndex = 0;
    DPL_enDisplayMenu(&astMenu);
    osDelay(1000);
    astMenu.u8CurrentIndex = 1;
    DPL_enDisplayMenu(&astMenu);
    osDelay(1000);
    astMenu.u8CurrentIndex = 2;
    DPL_enDisplayMenu(&astMenu);
    osDelay(1000);
}

static void DISPLAY_2_voTaskTest(void* pvoArgument)
{
    ST7789_enInit();
    DISPLAY_2_voTest();
    for (;;)
    {
        osDelay(500);
    }
}

/* Export functions definition   --------------------------------------------------------*/
void DISPLAY_2_voTaskTestInit(void)
{
    DISPLAY_2_pvoTaskHandle = osThreadNew(DISPLAY_2_voTaskTest, NULL, &stDISPLAY2TaskTest);
}

#endif /*TEST_DISPLAY_2*/
