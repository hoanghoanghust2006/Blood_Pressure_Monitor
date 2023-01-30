/*
 * Title : blood_pressure.c
 * Copyright : HCL
 * Author : Nam Ngo
 * Creation Date : Sep 24, 2022
 * Description : < Briefly describe the purpose of the file. >
 * Limitations : < Any limitations. >
 * Dependencies : < H/W, S/W( Operating System, Compiler) >
 * Modifications History : Version 1.0 by Nam Ngo
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "blood_pressure.h"
#include "cmsis_os.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/
#if defined(STM32H735xx)
#define MOTOR_PORT  GPIOC
#define MOTOR_PIN   GPIO_PIN_0
#define VALVES_PORT GPIOH
#define VALVES_PIN  GPIO_PIN_2
#elif defined(STM32H7B3xxQ)
#define MOTOR_PORT  GPIOA
#define MOTOR_PIN   GPIO_PIN_4
#define VALVES_PORT GPIOC
#define VALVES_PIN  GPIO_PIN_4
#endif

#define ON             GPIO_PIN_SET
#define OFF            GPIO_PIN_RESET
#define PRE_MAXIMUM    140
#define PRE_ESSENTIAL  60
#define TIME_WAITE_CMM 2000
#define MMHG_CONVERT   12001
#define DC_GAIN        120.0
#define MEAS_REPEAT    12
#define SYS_THRESHOLD  1.74
#define HB_THRESHOLD   1.74
#define DIA_THRESHOLD  1.718
#define CHANNEL_DC     0
#define CHANNEL_AC     1
#define SYS_DETECT     2
#define HB_DETECT      3
#define DIA_DETECT     4
#define SYS_CAL        5
#define HB_CAL         6
#define DIA_CAL        7
#define WAITE_DETECT   8
#define DETECTED       9

/* Private macros -----------------------------------------------------------------------*/
static float    fPrevVoltage             = 0.0;
static float    fPulsePeriod             = 0.0;
static float    fVoltageChannelAC        = 0.0;
static uint8_t  u8ModeMeasure            = 0;
static uint8_t  u8CountVariablesCmm      = 0;
static uint8_t  u8CatchFirstPoint        = 0;
static uint32_t u32TimeConfirm           = 0;
static uint32_t u32PrevTimeDetect        = 0;
static uint32_t u32CumulativePulsePeriod = 0;

/* Private type definitions  ------------------------------------------------------------*/
typedef void (*PRE_voRunProcess)();
typedef enum
{
    eNOT_STA_MO, /* The status isn't machine of the monitor  */
    eINFLATE,    /* The status during inflate air */
    eDEFLATE     /* The status during deflate air  */
} tenStatusMonitor;
typedef struct
{
    tenStatusMonitor enStatus;
    PRE_voRunProcess fpvoRunProcess;
} tstProcessMachine;
typedef enum
{
    eSYS_HANDLE,
    eHB_HANDLE,
    eDIA_HANDLE,
    eEND_HANDLE
} tenStatusHandle;

static tenStatusMonitor    enStatusMonitor    = eNOT_STA_MO;
static tenStatusHandle     enStatusHandle     = eSYS_HANDLE;
static tenNotifyResponse   enNotifyResponse   = eNOT_NOTI_RES;
static tstValueMeasurement stValueMeasurement = {.u8Pressure  = 0,
                                                 .u8Systolic  = 0,
                                                 .u8Diastolic = 0,
                                                 .u8HeartBeat = 0};

/* Private file-local global variables   ------------------------------------------------*/
static ADC_HandleTypeDef hadc1;

/* Private function prototypes declarations   -------------------------------------------*/
static void PRE_voMotor(uint8_t u8Mode);
static void PRE_voAirValves(uint8_t u8Mode);
static void PRE_voInflateProcess(void);
static void PRE_voDeflateProcess(void);
static void PRE_voADCInitialize(uint8_t u8Channel);
static void PRE_voReadADC(void);
static void PRE_voSystolicHandle(void);
static void PRE_voDiastolicHandle(void);
static void PRE_voHeartBeatHandle(void);

/* Private lookup table    --------------------------------------------------------------*/
static tstProcessMachine stProcessMachine[] = {{eINFLATE, PRE_voInflateProcess},
                                               {eDEFLATE, PRE_voDeflateProcess}};
static void (*PRE_voMeasurementHandle[])()  = {PRE_voSystolicHandle,
                                               PRE_voHeartBeatHandle,
                                               PRE_voDiastolicHandle};

/* Private functions definition   -------------------------------------------------------*/
static void PRE_voMotor(uint8_t u8Mode)
{
    HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, u8Mode);
}

static void PRE_voAirValves(uint8_t u8Mode)
{
    HAL_GPIO_WritePin(VALVES_PORT, VALVES_PIN, u8Mode);
}

static void PRE_voInflateProcess(void)
{
    /* Read ADC */
    PRE_voReadADC();

    /* Check stop pupmping air into the cuff */
    if (stValueMeasurement.u8Pressure >= PRE_MAXIMUM)
    {
        trace("SYS Measuring...\r\n");

        /* Turn off motor */
        PRE_voMotor(OFF);

        /* Set handle the frist measurement */
        enStatusHandle = eSYS_HANDLE;

        /* Set next status of the process */
        enStatusMonitor = eDEFLATE;
    }
    else
    {
        // FIXME check error
        u32TimeConfirm = xTaskGetTickCount() - u32PrevTimeDetect;

        if ((u32TimeConfirm >= TIME_WAITE_CMM) && (stValueMeasurement.u8Pressure < PRE_ESSENTIAL))
        {
            trace("Failed\r\n");

            /* Turn off both motor and air valves */
            PRE_voMotor(OFF);
            PRE_voAirValves(OFF);

            /* Return the status error */
            enStatusMonitor  = eNOT_STA_MO;
            enNotifyResponse = eFAILED;

            /* Stop ADC and interrupt timer*/
            HAL_ADC_Stop(&hadc1);
        }
        else
        {
            // Not Todo
        }
    }
}

static void PRE_voDeflateProcess(void)
{
    if ((enStatusHandle == eEND_HANDLE) || (stValueMeasurement.u8Pressure < 60))
    {
        /* Turn off air valves */
        PRE_voAirValves(OFF);

        /* Stop ADC and interrupt timer*/
        HAL_ADC_Stop(&hadc1);
        trace("Successful\r\n");

        /* Return the status successful */
        stValueMeasurement.u8Diastolic = stValueMeasurement.u8Pressure;
        enStatusMonitor                = eNOT_STA_MO;
        enNotifyResponse               = eSUCCESSFUL;
    }
    else
    {
        /* Read ADC */
        PRE_voReadADC();

        /* Program to measure */
        (*PRE_voMeasurementHandle[enStatusHandle])();
    }
}

static void PRE_voADCInitialize(uint8_t u8Channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    HAL_ADC_Stop(&hadc1);

    if (u8Channel == CHANNEL_DC)
    {
        /* Configure for the selected ADC channel 0 */
        sConfig.Channel                = ADC_CHANNEL_0;
        sConfig.Rank                   = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime           = ADC_SAMPLETIME_64CYCLES_5;
        sConfig.SingleDiff             = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber           = ADC_OFFSET_NONE;
        sConfig.Offset                 = 0;
        sConfig.OffsetSignedSaturation = DISABLE;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
        }
    }
    else if (u8Channel == CHANNEL_AC)
    {
        /* Configure for the selected ADC channel 1 */
        sConfig.Channel                = ADC_CHANNEL_1;
        sConfig.Rank                   = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime           = ADC_SAMPLETIME_64CYCLES_5;
        sConfig.SingleDiff             = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber           = ADC_OFFSET_NONE;
        sConfig.Offset                 = 0;
        sConfig.OffsetSignedSaturation = DISABLE;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
        }
    }
    else
    {
        // Not Todo
    }

    HAL_ADC_Start(&hadc1);
}

static void PRE_voReadADC(void)
{
    static uint16_t fCumulativeDC     = 0;
    static uint16_t fCumulativeAC     = 0;
    static uint8_t  u8MeasureRepeat   = 0;
    static float    fVoltageChannelDC = 0.0;

    /* Start read ADC values */
    if (u8MeasureRepeat != MEAS_REPEAT)
    {
        u8MeasureRepeat++;
        PRE_voADCInitialize(CHANNEL_DC);
        HAL_ADC_PollForConversion(&hadc1, 50);
        fCumulativeDC += HAL_ADC_GetValue(&hadc1);

        PRE_voADCInitialize(CHANNEL_AC);
        HAL_ADC_PollForConversion(&hadc1, 50);
        fCumulativeAC += HAL_ADC_GetValue(&hadc1);
    }
    else
    {
        /* Convert digital value to analogue value */
        fVoltageChannelDC             = (float)((float)(fCumulativeDC / MEAS_REPEAT) * 3.3) / 4095;
        fVoltageChannelAC             = (float)((float)(fCumulativeAC / MEAS_REPEAT) * 3.3) / 4095;
        stValueMeasurement.u8Pressure = (uint8_t)((fVoltageChannelDC / DC_GAIN) * MMHG_CONVERT);

        // trace("<  DC: %.2f - mmHg: %d   >  <   AC: %.2f   >\r\n", fVoltageChannelDC, stValueMeasurement.u8Pressure, fVoltageChannelAC);
        // trace("%.3f\r\n", (fVoltageChannelAC));

        fCumulativeDC   = 0;
        fCumulativeAC   = 0;
        u8MeasureRepeat = 0;
    }
}

static void PRE_voSystolicHandle(void)
{
    switch (u8ModeMeasure)
    {
        case SYS_DETECT:
            if (u8CountVariablesCmm < 4) /* Take 4 time to verify that it's the start point of the systolic */
            {
                if ((fPrevVoltage <= SYS_THRESHOLD) && (fVoltageChannelAC > SYS_THRESHOLD))
                {
                    u8CountVariablesCmm++;
                }
                fPrevVoltage = fVoltageChannelAC; /* Load new fPrevVoltage value  */
            }
            else
            {
                u8ModeMeasure = SYS_CAL;
            }
            break;
        case SYS_CAL:
            /* Save the systolic value */
            stValueMeasurement.u8Systolic = stValueMeasurement.u8Pressure;
            trace("Sys -> %d\r\n", stValueMeasurement.u8Systolic);
            trace("HB Measurement...\r\n");

            u8CountVariablesCmm = 0; /* Use this variable again for next task */
            fPrevVoltage        = HB_THRESHOLD - 0.1;

            /* Move to next task */
            u32CumulativePulsePeriod = 0;
            fPulsePeriod             = 0.0;
            u8ModeMeasure            = HB_DETECT;
            u8CatchFirstPoint        = WAITE_DETECT;
            enStatusHandle           = eHB_HANDLE;
            break;
        default:
            // Not Todo
            break;
    }
}

static void PRE_voHeartBeatHandle(void)
{
    static uint32_t u32PrevPulsePeriod = 0;
    // trace("Func Heart Beat\r\n");
    switch (u8ModeMeasure)
    {
        case HB_DETECT:
            /* Count 5 time measure averange the pulse period of the heart beat */
            if (u8CountVariablesCmm < 5)
            {
                if ((fPrevVoltage < HB_THRESHOLD) && (fVoltageChannelAC > HB_THRESHOLD) && (u8CatchFirstPoint == WAITE_DETECT))
                {
                    u8CatchFirstPoint = DETECTED;
                    fPrevVoltage      = fVoltageChannelAC;

                    /* Get the current time and start counting new heart pulse period */
                    u32PrevPulsePeriod = xTaskGetTickCount();
                }

                if ((fPrevVoltage < HB_THRESHOLD) && (fVoltageChannelAC > HB_THRESHOLD) && (u8CatchFirstPoint == DETECTED))
                {
                    u32CumulativePulsePeriod += (xTaskGetTickCount() - u32PrevPulsePeriod);

                    /* Upload new time catch new point */
                    u32PrevPulsePeriod = xTaskGetTickCount();
                    u8CountVariablesCmm++;
                }
                fPrevVoltage = fVoltageChannelAC;
            }
            else
            {
                u8ModeMeasure = HB_CAL;
            }
            break;
        case HB_CAL:
            /* Calculate the elasticity period of the heart beat period after 5 cycles of the heart beat    */
            fPulsePeriod = u32CumulativePulsePeriod / 5000.0;

            /** Calculate the heart beat per miniute
             * Explane: 72bpm < 72 heart beat per minute >
             * <=> 1/72mpb = 0.0138 < minute per beat >
             * <=> 60/72spb = 0.833 < second per beat >
             */
            stValueMeasurement.u8HeartBeat = (uint8_t)(60 / fPulsePeriod);
            trace("HB -> %d <%.4f>\r\n", stValueMeasurement.u8HeartBeat, fPulsePeriod);
            trace("DIA Measurement...\r\n");

            /* Move to next task */
            u32TimeConfirm    = 0;
            u32PrevTimeDetect = 0;
            u8ModeMeasure     = DIA_DETECT;
            u8CatchFirstPoint = WAITE_DETECT;
            enStatusHandle    = eDIA_HANDLE;
            break;
        default:
            // Not Todo
            break;
    }
}

static void PRE_voDiastolicHandle(void)
{
    switch (u8ModeMeasure)
    {
        case DIA_DETECT:
            if (u32TimeConfirm >= TIME_WAITE_CMM)
            {
                /* After 2 seconds move to next task */
                u8ModeMeasure = DIA_CAL;
            }
            else
            {
                if (fVoltageChannelAC <= DIA_THRESHOLD)
                {
                    if (u8CatchFirstPoint == WAITE_DETECT)
                    {
                        u32PrevTimeDetect = xTaskGetTickCount();
                        u8CatchFirstPoint = DETECTED;
                    }
                    else if (u8CatchFirstPoint == DETECTED)
                    {
                        u32TimeConfirm += (xTaskGetTickCount() - u32PrevTimeDetect);
                        u32PrevTimeDetect = xTaskGetTickCount();
                    }
                    else
                    {
                        // Not Todo
                    }
                }
                else
                {
                    u32TimeConfirm    = 0;
                    u8CatchFirstPoint = WAITE_DETECT;
                }
            }
            break;
        case DIA_CAL:
            /* Save the diastolic value */
            stValueMeasurement.u8Diastolic = stValueMeasurement.u8Pressure;
            trace("DIA -> %d\r\n", stValueMeasurement.u8Diastolic);

            /* Turn off air valves */
            PRE_voAirValves(OFF);

            /* Set notify successful and stop process*/
            enNotifyResponse = eSUCCESSFUL;
            enStatusHandle   = eEND_HANDLE;
            break;
        default:
            break;
    }
}

/* Export functions definition   --------------------------------------------------------*/
void PRE_voInint(void)
{
    /* Intialize ADC  */
    ADC_MultiModeTypeDef multimode       = {0};
    GPIO_InitTypeDef     GPIO_InitStruct = {0};

    hadc1.Instance                      = ADC1;
    hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution               = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode             = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait         = DISABLE;
    hadc1.Init.ContinuousConvMode       = ENABLE;
    hadc1.Init.NbrOfConversion          = 2;
    hadc1.Init.DiscontinuousConvMode    = DISABLE;
    hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc1.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
    hadc1.Init.OversamplingMode         = DISABLE;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
    }

    /* Configure the ADC multi-mode */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
    {
    }

#if defined(STM32H735xx)
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_RESET);

    /*Configure GPIO pins : PC0 */
    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PH2 */
    GPIO_InitStruct.Pin   = GPIO_PIN_2;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
#elif defined(STM32H7B3xxQ)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

    /*Configure GPIO pins : PC0 */
    GPIO_InitStruct.Pin   = GPIO_PIN_4;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PH2 */
    GPIO_InitStruct.Pin   = GPIO_PIN_4;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
}

void PRE_voRequestStartProcess(void)
{
    if (enStatusMonitor == eNOT_STA_MO)
    {
        // trace("Start ...\r\n");

        /*  Intialize some of the value */
        stValueMeasurement.u8Pressure  = 0;
        stValueMeasurement.u8Diastolic = 0;
        stValueMeasurement.u8Systolic  = 0;
        stValueMeasurement.u8HeartBeat = 0;

        fPrevVoltage             = SYS_THRESHOLD - 0.01;
        u8ModeMeasure            = SYS_DETECT;
        u8CatchFirstPoint        = WAITE_DETECT;
        u32TimeConfirm           = 0;
        u8CountVariablesCmm      = 0;
        u32CumulativePulsePeriod = 0;
        u32PrevTimeDetect        = xTaskGetTickCount();

        /* Set next step status of the process */
        enStatusMonitor = eINFLATE;

        /* Set status busy of the process */
        enNotifyResponse = eBUSY;

        /* Turn on both moter and air valves */
        PRE_voMotor(ON);
        PRE_voAirValves(ON);
    }
    else
    {
        // Not Todo
    }
}

void PRE_voRequestCancelProcess(void)
{
    if (enNotifyResponse == eBUSY)
    {
        trace("Cancel\r\n");

        /* Turn off both motor and air valves */
        PRE_voMotor(OFF);
        PRE_voAirValves(OFF);

        /* Stop ADC and interrupt timer*/
        HAL_ADC_Stop(&hadc1);

        /* Reset status */
        enStatusMonitor  = eNOT_STA_MO;
        enNotifyResponse = eNOT_NOTI_RES;
    }
    else
    {
        // Not Todo
    }
}

void PRE_voMainFunction(void)
{
    if (enNotifyResponse != eNOT_NOTI_RES) /* Check the status setting */
    {
        for (uint8_t Index = 0; Index < (sizeof(stProcessMachine) / sizeof(tstProcessMachine)); Index++)
        {
            if (enStatusMonitor == stProcessMachine[Index].enStatus) /* Check status current of the process */
            {
                stProcessMachine[Index].fpvoRunProcess();
                break;
            }
        }
    }
    else
    {
        // Not Todo
    }
}

tenNotifyResponse PRE_enGetStatusProcess(tstValueMeasurement *stGetResult)
{
    stGetResult->u8Pressure  = stValueMeasurement.u8Pressure;
    stGetResult->u8Systolic  = stValueMeasurement.u8Systolic;
    stGetResult->u8HeartBeat = stValueMeasurement.u8HeartBeat;
    stGetResult->u8Diastolic = stValueMeasurement.u8Diastolic;

    return enNotifyResponse;
}