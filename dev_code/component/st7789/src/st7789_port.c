/**
 * Title : st7789_port.c
 * Copyright : HCL
 * Author : Le Hieu, Nguyen Duc
 * Creation Date : Feb 20, 2022
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include "st7789_port.h"
#include "stm32l4xx_hal.h"
#include "trace.h"

/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/
SPI_HandleTypeDef stHandlerSpi;
#define ST7789_RST_PORT GPIOG
#define ST7789_RST_PIN  GPIO_PIN_15
#define ST7789_DC_PORT  GPIOG
#define ST7789_DC_PIN   GPIO_PIN_6
#define ST7789_CS_PORT  GPIOH
#define ST7789_CS_PIN   GPIO_PIN_13

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/

/* Export functions definition   --------------------------------------------------------*/
void ST7789_voRstClear(void)
{
    HAL_GPIO_WritePin(ST7789_RST_PORT, ST7789_RST_PIN, GPIO_PIN_RESET);
}

void ST7789_voRstSet(void)
{
    HAL_GPIO_WritePin(ST7789_RST_PORT, ST7789_RST_PIN, GPIO_PIN_SET);
}

void ST7789_voDcClear(void)
{
    HAL_GPIO_WritePin(ST7789_DC_PORT, ST7789_DC_PIN, GPIO_PIN_RESET);
}

void ST7789_voDcSet(void)
{
    HAL_GPIO_WritePin(ST7789_DC_PORT, ST7789_DC_PIN, GPIO_PIN_SET);
}

void ST7789_voCsClear(void)
{
    HAL_GPIO_WritePin(ST7789_CS_PORT, ST7789_CS_PIN, GPIO_PIN_RESET);
}

void ST7789_voCsSet(void)
{
    HAL_GPIO_WritePin(ST7789_CS_PORT, ST7789_CS_PIN, GPIO_PIN_SET);
}

tenStatus ST7789_enInitIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    ST7789_voDcSet();

    /*Configure GPIO pin Output Level */
    ST7789_voRstClear();

    /*Configure GPIO pin Output Level */
    ST7789_voCsClear();

    /*Configure GPIO pin : PG6 */
    GPIO_InitStruct.Pin   = ST7789_DC_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ST7789_DC_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : PG15 */
    GPIO_InitStruct.Pin   = ST7789_RST_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ST7789_RST_PORT, &GPIO_InitStruct);

    /*Configure GPIO pin : PH13 */
    GPIO_InitStruct.Pin   = ST7789_CS_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ST7789_CS_PORT, &GPIO_InitStruct);

    stHandlerSpi.Instance               = SPI1;
    stHandlerSpi.Init.Mode              = SPI_MODE_MASTER;
    stHandlerSpi.Init.Direction         = SPI_DIRECTION_2LINES;
    stHandlerSpi.Init.DataSize          = SPI_DATASIZE_8BIT;
    stHandlerSpi.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    stHandlerSpi.Init.CLKPhase          = SPI_PHASE_1EDGE;
    stHandlerSpi.Init.NSS               = SPI_NSS_SOFT;
    stHandlerSpi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    stHandlerSpi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    stHandlerSpi.Init.TIMode            = SPI_TIMODE_DISABLE;
    stHandlerSpi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    stHandlerSpi.Init.CRCPolynomial     = 7;
    stHandlerSpi.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    stHandlerSpi.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&stHandlerSpi) != HAL_OK)
    {
        return eFAIL;
    }
    return eSUCCESS;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (spiHandle->Instance == SPI1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**
         * SPI1 GPIO Configuration
         * PB5     ------> SPI1_MOSI
         * PA5     ------> SPI1_SCK
         */
        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{
    if (spiHandle->Instance == SPI1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**
         * SPI1 GPIO Configuration
         * PB5     ------> SPI1_MOSI
         * PA5     ------> SPI1_SCK
         */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
    }
}

tenStatus ST7789_voTransmit(uint8_t *pu8Data, uint16_t u16Size, uint32_t u32Timeout)
{
    if (HAL_SPI_Transmit(&stHandlerSpi, pu8Data, u16Size, u32Timeout) == HAL_OK)
    {
        return eSUCCESS;
    }
    else
    {
        return eFAIL;
    }
}

/* Export variables definition   --------------------------------------------------------*/