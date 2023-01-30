/*
 * Title : blood_pressure.h
 * Copyright : HCL
 * Author : Nam Ngo
 * Creation Date : Sep 24, 2022
 * Requirements : < Which other header files needs be included to include this header file. >
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : Version 1.0 by Nam Ngo
 * ------- ---------- --------
 */

#ifndef _BLOOD_PRESSURE_H_
#define _BLOOD_PRESSURE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Include --------------------------------------------------------------------------------*/
#include <stdint.h>

    /* Define constants -----------------------------------------------------------------------*/

    /* Type definitions (Typedef, enum, struct) -----------------------------------------------*/
    typedef enum
    {
        eNOT_NOTI_RES, /* Don't have any notifies */
        eFAILED,       /* Return this status when the process occurs error*/
        eBUSY,         /* Return this status when the process is running*/
        eSUCCESSFUL    /* Return this status when the process doesn't have any errors*/
    } tenNotifyResponse;

    typedef struct
    {
        uint8_t u8Pressure;  /* Save the value of the pressure during the actual measurement */
        uint8_t u8Systolic;  /* Save the value of the systolic when the measurement is successful */
        uint8_t u8HeartBeat; /* Save the value of the heart beat when the measurement is successful */
        uint8_t u8Diastolic; /* Save the value of the diastolic when the measurement is successful */
    } tstValueMeasurement;

    /* Export Function Declarations -----------------------------------------------------------*/
    /*
     * Name : PRE_voInint
     * Description : Intialize all the peripheral essential use to blood pressure
     * Parameters: void
     * Returns : void
     */
    void PRE_voInint(void);

    /*
     * Name : PRE_voMainFunction
     * Description : Infinity loop to run main process
     * Parameters: void
     * Returns : void
     */
    void PRE_voMainFunction(void);

    /*
     * Name : PRE_voRequestStartProcess
     * Description : Start run process and measurement
     * Parameters: void
     * Returns : void
     */
    void PRE_voRequestStartProcess(void);

    /*
     * Name : PRE_voRequestCancelProcess
     * Description : Immediately cancel and stop process.
     * Parameters: void
     * Returns : void
     */
    void PRE_voRequestCancelProcess(void);

    /*
     * Name : PRE_enGetStatusProcess
     * Description : Return the status of the process when the process is end or ocurrs error
     * Parameters: void
     * Returns : tenNotifyResponse
     */
    tenNotifyResponse PRE_enGetStatusProcess(tstValueMeasurement *stGetResult);

#ifdef __cplusplus
}
#endif

#endif /* _BLOOD_PRESSURE_H_ */
