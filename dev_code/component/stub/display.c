#include "display.h"
#include "trace.h"

tenStatus DPL_enDisplayProcessMeasurement(uint8_t          u8AirPressure,
                                          tenPressureState enPressureState,
                                          bool             bRefreshAll)
{
    printf("\033\143");
    printf("\033[3J");
    trace("Air Pressure: %d\r\n", u8AirPressure);
    trace("enPressureState: %d\r\n", enPressureState);
    trace("bRefreshAll: %d\r\n", bRefreshAll);
    return eSUCCESS;
}

tenStatus DPL_enDisplayResults(const tstBloodPressureResult* stResult)
{
    if (stResult->enStatus == eSUCCESS)
    {
        printf("\033\143");
        printf("\033[3J");
        trace("Systolic: %d\r\n", stResult->u8Systolic);
        trace("HeartBeat: %d\r\n", stResult->u8HeartBeat);
        trace("Diastolic: %d\r\n", stResult->u8Diastolic);
    }
    else
    {
        trace("Error\r\n");
    }
    return eSUCCESS;
}

tenStatus DPL_enDisplayMenu(const tstMenu* stMenu)
{
    // TODO: Hoang Hoang
    return eSUCCESS;
}

tenStatus DPL_enDisplayRecordHistory(const tstStorage* stStorage, uint8_t u8Index)
{
    return eSUCCESS;
}

tenStatus DPL_enDisplaySetupDate(const tstTime* stTime, tenDateSetupState enDateSetupState)
{
    return eSUCCESS;
}

tenStatus DPL_enDisplaySetupTime(const tstTime* stTime, tenTimeSetupState enTimeSetupState)
{
    return eSUCCESS;
}
