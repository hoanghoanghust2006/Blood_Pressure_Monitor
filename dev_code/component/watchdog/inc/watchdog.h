/*
 * Title :  watchdog.h
 * Copyright : HCL
 * Author :  minh.thach
 * Creation Date :  01/09/2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : VERSION WHO WHEN WHY
 * ------- ---------- --------
 */

#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
/*
 * Name : WDG_voInit
 * Description : Init watchdog
 * Parameters: void
 * Return: void
 */
void WDG_voInit(void);

/*
 * Name : WDG_voMainFunction
 * Description : Auto reset IWDG
 * Parameters: void
 * Return: void
 */
void WDG_voMainFunction(void);

#ifdef __cplusplus
}
#endif
#endif
