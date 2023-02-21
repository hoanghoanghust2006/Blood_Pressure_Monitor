#include "blood_pressure.h"
#include "trace.h"

void PRE_voRequestStartProcess(void)
{
    trace("Request to start blood pressure\r\n");
}

void PRE_voRequestCancelProcess(void)
{
    trace("Request to stop blood pressure\r\n");
}

tenNotifyResponse PRE_enGetStatusProcess(tstValueMeasurement *stGetResult)
{
    static uint32_t   u32Count      = 0;
    uint8_t           u8SimPressure = 0;
    tenNotifyResponse enRespond     = eNOT_NOTI_RES;

    u32Count++;
    if (u32Count <= (300))
    {
        u8SimPressure = u32Count / 2;
        enRespond     = eBUSY;
    }
    else if ((u32Count > 300) && (u32Count <= 600))
    {
        u8SimPressure = (600 - u32Count) / 2;
        enRespond     = eBUSY;
    }
    else
    {
        enRespond = eSUCCESSFUL;
    }

    stGetResult->u8Pressure = u8SimPressure;

    return enRespond;
}