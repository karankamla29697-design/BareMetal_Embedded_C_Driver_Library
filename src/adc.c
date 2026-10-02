#include "adc.h"
#include "rcc.h"

#define ADC_TIMEOUT_COUNT       1000000UL
#define ADC_REFERENCE_MV        3300UL
#define ADC_MAX_VALUE           4095UL

static uint8_t adc_status = ADC_OK;

static uint32_t ADC_DivideU32(
    uint32_t dividend,
    uint32_t divisor,
    uint32_t *remainder
)
{
    uint32_t quotient = 0U;
    uint32_t rem = 0U;
    int bit;

    if (divisor == 0U)
    {
        if (remainder != 0)
        {
            *remainder = 0U;
        }

        return 0U;
    }

    for (bit = 31; bit >= 0; bit--)
    {
        rem <<= 1U;
        rem |= (dividend >> bit) & 1U;

        if (rem >= divisor)
        {
            rem -= divisor;
            quotient |= (1UL << bit);
        }
    }

    if (remainder != 0)
    {
        *remainder = rem;
    }

    return quotient;
}

static uint8_t ADC_WaitForClear(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = ADC_TIMEOUT_COUNT;

    while ((*reg & mask) != 0U)
    {
        if (timeout-- == 0U)
        {
            return ADC_TIMEOUT;
        }
    }

    return ADC_OK;
}

static uint8_t ADC_WaitForSet(volatile uint32_t *reg, uint32_t mask)
{
    uint32_t timeout = ADC_TIMEOUT_COUNT;

    while ((*reg & mask) == 0U)
    {
        if (timeout-- == 0U)
        {
            return ADC_TIMEOUT;
        }
    }

    return ADC_OK;
}

static uint8_t ADC_ConfigurePin(ADC_Channel_t channel)
{
    if (channel <= ADC_CHANNEL_7)
    {
        GPIOA->CRL &= ~(0xFU << (channel * 4U));
    }
    else if (channel <= ADC_CHANNEL_9)
    {
        uint8_t pin = (uint8_t)channel - 8U;

        GPIOB->CRL &= ~(0xFU << (pin * 4U));
    }
    else
    {
        return ADC_ERROR;
    }

    return ADC_OK;
}

static void ADC_SetSampleTime(ADC_Channel_t channel)
{
    /*
     * 55.5 ADC cycles:
     * SMP = 101
     *
     * Channels 0..9  -> SMPR2
     * Channels 10..17 -> SMPR1
     *
     * This driver exposes only external channels 0..9.
     */
    uint32_t shift = ((uint32_t)channel * 3U);

    ADC1->SMPR2 &= ~(7U << shift);
    ADC1->SMPR2 |=  (5U << shift);
}

uint8_t ADC_Init(void)
{
    uint32_t pclk2;
    uint32_t adc_clock_divider;

    adc_status = ADC_OK;

    /*
     * ADC1 uses:
     *   PA0..PA7 for channels 0..7
     *   PB0..PB1 for channels 8..9
     */
    RCC_EnablePeripheralClock(RCC_GPIOA);
    RCC_EnablePeripheralClock(RCC_GPIOB);

    /*
     * ADC1 clock is on APB2.
     */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    pclk2 = RCC_GetAPB2Clock();

    /*
     * STM32F103 ADC clock must not exceed 14 MHz.
     *
     * Use /4 when possible.
     * Otherwise use /6.
     * If PCLK2 is above 84 MHz, /8 would be required,
     * but STM32F103 APB2 normally does not exceed 72 MHz.
     */
    if (pclk2 <= 56000000UL)
    {
        adc_clock_divider = 4U;

        RCC->CFGR &= ~RCC_CFGR_ADCPRE;
        RCC->CFGR |= RCC_CFGR_ADCPRE_DIV4;
    }
    else
    {
        adc_clock_divider = 6U;

        RCC->CFGR &= ~RCC_CFGR_ADCPRE;
        RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
    }

    (void)adc_clock_divider;

    /*
     * Reset ADC configuration.
     */
    ADC1->CR1 = 0U;
    ADC1->CR2 = 0U;

    /*
     * Software trigger:
     * EXTSEL = 000 -> SWSTART
     * EXTTRIG = 1
     */
    ADC1->CR2 = ADC_CR2_EXTTRIG;

    /*
     * One conversion in regular sequence.
     */
    ADC1->SQR1 = 0U;
    ADC1->SQR2 = 0U;
    ADC1->SQR3 = 0U;

    /*
     * Default sample time = 55.5 cycles for all
     * external channels.
     */
    ADC1->SMPR1 = 0xFFFFFFFFUL;
    ADC1->SMPR2 = 0xFFFFFFFFUL;

    /*
     * Enable ADC.
     */
    ADC1->CR2 |= ADC_CR2_ADON;

    /*
     * Reset calibration.
     */
    ADC1->CR2 |= ADC_CR2_RSTCAL;

    if (ADC_WaitForClear(&ADC1->CR2, ADC_CR2_RSTCAL) != ADC_OK)
    {
        adc_status = ADC_TIMEOUT;
        return ADC_TIMEOUT;
    }

    /*
     * Start calibration.
     */
    ADC1->CR2 |= ADC_CR2_CAL;

    if (ADC_WaitForClear(&ADC1->CR2, ADC_CR2_CAL) != ADC_OK)
    {
        adc_status = ADC_TIMEOUT;
        return ADC_TIMEOUT;
    }

    /*
     * ADC is now ready.
     */
    adc_status = ADC_OK;

    return ADC_OK;
}

uint8_t ADC_Read(ADC_Channel_t channel, uint16_t *value)
{
    uint8_t status;

    if (value == 0)
    {
        adc_status = ADC_ERROR;
        return ADC_ERROR;
    }

    /*
     * Only external channels 0..9 are supported.
     */
    if ((uint32_t)channel > (uint32_t)ADC_CHANNEL_9)
    {
        adc_status = ADC_ERROR;
        return ADC_ERROR;
    }

    status = ADC_ConfigurePin(channel);

    if (status != ADC_OK)
    {
        adc_status = status;
        return status;
    }

    ADC_SetSampleTime(channel);

    /*
     * Configure regular sequence:
     * exactly one conversion.
     */
    ADC1->SQR1 = 0U;
    ADC1->SQR2 = 0U;
    ADC1->SQR3 = (uint32_t)channel;

    /*
     * Clear previous EOC state.
     */
    ADC1->SR &= ~ADC_SR_EOC;

    /*
     * Start software conversion.
     */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /*
     * Wait for conversion completion.
     */
    status = ADC_WaitForSet(&ADC1->SR, ADC_SR_EOC);

    if (status != ADC_OK)
    {
        adc_status = ADC_TIMEOUT;
        return ADC_TIMEOUT;
    }

    /*
     * Reading DR clears EOC.
     */
    *value = (uint16_t)(ADC1->DR & 0x0FFFU);

    adc_status = ADC_OK;

    return ADC_OK;
}

uint8_t ADC_ReadVoltage(
    ADC_Channel_t channel,
    uint32_t *millivolts
)
{
    uint16_t raw;
    uint32_t scaled;
    uint32_t remainder;

    if (millivolts == 0)
    {
        adc_status = ADC_ERROR;
        return ADC_ERROR;
    }

    if (ADC_Read(channel, &raw) != ADC_OK)
    {
        return adc_status;
    }

    /*
     * Voltage = raw * 3300 / 4095
     *
     * Rounded to nearest millivolt.
     */
    scaled = (uint32_t)raw * ADC_REFERENCE_MV;
    scaled += (ADC_MAX_VALUE / 2UL);

    *millivolts = ADC_DivideU32(
        scaled,
        ADC_MAX_VALUE,
        &remainder
    );

    (void)remainder;

    adc_status = ADC_OK;

    return ADC_OK;
}

uint8_t ADC_GetStatus(void)
{
    return adc_status;
}
