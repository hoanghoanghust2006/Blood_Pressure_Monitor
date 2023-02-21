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
    // TODO: Do Trang
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
}

tenStatus DPL_enDisplaySetupTime(const tstTime* stTime, tenTimeSetupState enTimeSetupState)
{
}
