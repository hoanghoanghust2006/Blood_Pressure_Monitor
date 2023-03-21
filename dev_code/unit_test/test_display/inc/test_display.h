/*
 * Title : test_display.h
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Mar 6, 2023
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : Version 1.0 by Nam Ngo
 * ------- ---------- --------
 */

#ifndef _TEST_DISPLAY_H_
#define _TEST_DISPLAY_H_

/* Include --------------------------------------------------------------------------------*/

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
/*
 * Name : DISPLAY_voTaskTestInit
 * Description : Initialize task FreeRTOS to test DISPLAY
 * Parameters: void
 * Returns : None
 */
void DISPLAY_voTaskTestInit(void);

#endif /* _TEST_DISPLAY_H_ */
