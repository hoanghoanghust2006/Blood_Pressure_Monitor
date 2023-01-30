/*
 * Title : < Name of the file. >
 * Copyright : < Company Name of the file owner. >
 * Author : < Name of the author. >
 * Creation Date : < Date in DD/MMM/YY format >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/
#include <stdio.h>

/* Local Include ------------------------------------------------------------------------*/
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
#ifdef TEST_BAT
    printf("Unit test for battery component\r\n");
    BAT_voTaskTestInit();
#endif

#ifdef TEST_STO
    printf("Unit test for storage component\r\n");
    STO_voTaskTestInit();
#endif

#ifdef TEST_RTC
    printf("Unit test for RTC component\r\n");
    RTC_voTaskTestInit();
#endif

#ifdef TEST_AUDI
    printf("Unit test for Audio component\r\n");
    printf("Press User button to play audio\r\n");
    AUDI_voTaskTestInit();
#endif

#ifdef TEST_BLOOD_PRESSURE
    printf("Unit test for blood pressure component\r\n");
    PRE_voTaskTestInit();
#endif
}
