/*
 * Title : < Name of the file. >
 * Copyright : HCL
 * Author : < Name of the author. >
 * Creation Date : < Date in DD/MMM/YY format >
 * ------- ---------- --------
 */

#ifndef _GLCD_H_
#define _GLCD_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Include --------------------------------------------------------------------------------*/

/* Define constants -----------------------------------------------------------------------*/

/* Type definitions (Typedef, enum, struct) -----------------------------------------------*/

/* Export Function Declarations -----------------------------------------------------------*/
tenStatus      GLCD_enInit(void);
void           GLCD_voMainFunction(uint32_t u32LoopTime);

#ifdef __cplusplus
}
#endif

#endif