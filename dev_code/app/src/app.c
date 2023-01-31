/*
 * Title : app.c
 * Copyright : HCL
 * Author : nguyen_trung
 * Creation Date : 31/01/2023
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/
#include <stdio.h>

/* Local Include ------------------------------------------------------------------------*/
#include "trace.h"
#include "FreeRTOS.h"
#include "driver_task.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/

/* Export functions definition   --------------------------------------------------------*/
void APP_voInit(void)
{
    printf("\033\143");
    printf("\033[3J");
    printf("**************** Firmware started ****************\r\n");

    /* Create driver task */
    DRIV_voTaskInit();

    /* Create test tasks */
#ifdef TEST_STO
    printf("Unit test for storage component\r\n");
    STO_voTaskTestInit();
#endif

#ifdef TEST_RTC
    printf("Unit test for RTC component\r\n");
    RTC_voTaskTestInit();
#endif

#ifdef TEST_BLOOD_PRESSURE
    printf("Unit test for blood pressure component\r\n");
    PRE_voTaskTestInit();
#endif

    /* Add heap size check for freeRTOS */
    // trace("Heap remain size: %d\r\n", xPortGetFreeHeapSize());
}
