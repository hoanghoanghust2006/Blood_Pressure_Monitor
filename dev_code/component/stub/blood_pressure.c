#include "blood_pressure.h"
#include "trace.h"

static tstValueMeasurement stValueMeasurement = {.u8Pressure  = 0,
                                                 .u8Systolic  = 0,
                                                 .u8Diastolic = 0,
                                                 .u8HeartBeat = 0};

static uint32_t u32Count = 0;

void PRE_voRequestStartProcess(void)
{
    trace("Request to start blood pressure\r\n");
}

void PRE_voRequestCancelProcess(void)
{
    trace("Request to stop blood pressure\r\n");
    u32Count = 0;
}

tenNotifyResponse PRE_enGetStatusProcess(tstValueMeasurement *stGetResult)
{
    uint8_t           u8SimPressure = 0;
    tenNotifyResponse enRespond     = eNOT_NOTI_RES;

    u32Count++;
    if (u32Count <= (300))
    {
        u8SimPressure = u32Count / 2;
        enRespond     = eBUSY;
        stGetResult->u8Diastolic = 0;
    }
    else if ((u32Count > 300) && (u32Count <= 600))
    {
        u8SimPressure = (600 - u32Count) / 2;
        enRespond     = eBUSY;
        stGetResult->u8Diastolic = 1;
    }
    else
    {
        enRespond = eSUCCESSFUL;
        u32Count  = 0;
    }

    stGetResult->u8Pressure = u8SimPressure;

    return enRespond;
}