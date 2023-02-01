/*
 * Title : led.h
 * Copyright : HCL
 * Author : Nam Ngo
 * Creation Date : Sep 9, 2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#ifndef _LED_H_
#define _LED_H_

/* Include --------------------------------------------------------------------------------*/

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
void LED_voInit(void);

/*
 * Name : LED_voMainFunction
 * Description : This function use into main loop to blink led
 * Parameters: void
 * Returns : None
 */
void LED_voMainFunction(void);

#endif /* _LED_H_ */