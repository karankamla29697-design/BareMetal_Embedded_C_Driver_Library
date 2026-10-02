#include "spi.h"
#include "gpio.h"

#define SPI_TIMEOUT_COUNT 1000000UL

static uint8_t spi_status = SPI_OK;

static uint8_t SPI_WaitForFlag(uint32_t flag)
{
    uint32_t timeout = SPI_TIMEOUT_COUNT;

    while ((SPI1->SR & flag) == 0U)
    {
        if (timeout == 0U)
        {
            spi_status = SPI_TIMEOUT;
            return 0U;
        }

        timeout--;
    }

    return 1U;
}

void SPI_Init(SPI_BaudRate_t baud_rate)
{
    SPI_Config_t config;

    config.baud_rate = baud_rate;
    config.cpol = SPI_CPOL_LOW;
    config.cpha = SPI_CPHA_FIRST;
    config.lsb_first = SPI_MSB_FIRST;

    SPI_InitAdvanced(&config);
}

void SPI_InitAdvanced(const SPI_Config_t *config)
{
    uint32_t cr1;

    if (config == 0)
    {
        spi_status = SPI_ERROR;
        return;
    }

    /*
     * SPI1:
     *
     * PA5 = SCK
     * PA6 = MISO
     * PA7 = MOSI
     */
    GPIO_Init(
        GPIO_PORT_A,
        5U,
        GPIO_MODE_OUTPUT_50MHZ,
        GPIO_OUTPUT_AF_PP
    );

    GPIO_Init(
        GPIO_PORT_A,
        6U,
        GPIO_MODE_INPUT,
        GPIO_INPUT_FLOATING
    );

    GPIO_Init(
        GPIO_PORT_A,
        7U,
        GPIO_MODE_OUTPUT_50MHZ,
        GPIO_OUTPUT_AF_PP
    );

    /*
     * Enable SPI1 clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    /*
     * Disable SPI before configuration.
     */
    SPI1->CR1 = 0U;
    SPI1->CR2 = 0U;

    /*
     * Master mode.
     *
     * SSM + SSI:
     * Software-managed NSS.
     */
    cr1 =
        SPI_CR1_MSTR |
        SPI_CR1_SSM  |
        SPI_CR1_SSI;

    /*
     * Baud-rate prescaler.
     */
    cr1 |= ((uint32_t)config->baud_rate & SPI_CR1_BR_MASK);

    /*
     * Clock polarity.
     */
    if (config->cpol != 0U)
    {
        cr1 |= SPI_CR1_CPOL;
    }

    /*
     * Clock phase.
     */
    if (config->cpha != 0U)
    {
        cr1 |= SPI_CR1_CPHA;
    }

    /*
     * Bit order.
     */
    if (config->lsb_first != 0U)
    {
        cr1 |= SPI_CR1_LSBFIRST;
    }

    /*
     * Enable SPI.
     */
    cr1 |= SPI_CR1_SPE;

    SPI1->CR1 = cr1;

    spi_status = SPI_OK;
}

uint8_t SPI_Transfer(uint8_t data)
{
    uint32_t timeout;
    uint8_t received;

    /*
     * Wait until transmit data register is empty.
     */
    if (SPI_WaitForFlag(SPI_SR_TXE) == 0U)
    {
        return 0U;
    }

    SPI1->DR = data;

    /*
     * Wait until receive data becomes available.
     */
    if (SPI_WaitForFlag(SPI_SR_RXNE) == 0U)
    {
        return 0U;
    }

    received = (uint8_t)(SPI1->DR & 0xFFU);

    /*
     * Wait until SPI is no longer busy.
     */
    timeout = SPI_TIMEOUT_COUNT;

    while ((SPI1->SR & SPI_SR_BSY) != 0U)
    {
        if (timeout == 0U)
        {
            spi_status = SPI_TIMEOUT;
            return received;
        }

        timeout--;
    }

    spi_status = SPI_OK;

    return received;
}

uint8_t SPI_SendByte(uint8_t data)
{
    /*
     * Transfer generates the required clock cycles.
     * The received byte is intentionally ignored.
     */
    (void)SPI_Transfer(data);

    return spi_status;
}

uint8_t SPI_ReceiveByte(uint8_t *data)
{
    if (data == 0)
    {
        spi_status = SPI_ERROR;
        return SPI_ERROR;
    }

    /*
     * Sending 0xFF generates clock pulses for reception.
     */
    *data = SPI_Transfer(0xFFU);

    return spi_status;
}

uint8_t SPI_GetStatus(void)
{
    return spi_status;
}
