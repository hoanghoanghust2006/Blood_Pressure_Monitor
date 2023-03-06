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
}

tenStatus DPL_enDisplayMenu(const tstMenu* stMenu)
{
    // TODO: Hoang Hoang
}

tenStatus DPL_enDisplayRecordHistory(const tstStorage* stStorage, uint8_t u8Index)
{
}

tenStatus DPL_enDisplaySetupDate(const tstTime* stTime, tenDateSetupState enDateSetupState)
{
    printf("\033\143");
    printf("\033[3J");

    switch (enDateSetupState)
    {
        case eDAY:
            trace("Day: %d\r\n", stTime->u8Day);
            break;
        case eMONTH:
            trace("Month: %d\r\n", stTime->u8Month);
            break;
        case eYEAR:
            trace("Year: %d\r\n", stTime->u16Year);
            break;
    }
}

tenStatus DPL_enDisplaySetupTime(const tstTime* stTime, tenTimeSetupState enTimeSetupState)
{
    printf("\033\143");
    printf("\033[3J");

    switch (enTimeSetupState)
    {
        case eHOUR:
            trace("Hour: %d\r\n", stTime->u8Hour);
            break;
        case eMINUTE:
            trace("Minute: %d\r\n", stTime->u8Minute);
            break;
        case eSECOND:
            trace("Second: %d\r\n", stTime->u8Second);
            break;
    }
}
