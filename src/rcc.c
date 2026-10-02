#include "rcc.h"

static uint32_t RCC_GetAHBPrescaler(void)
{
    static const uint16_t ahb_prescaler[16] =
    {
        1U, 1U, 1U, 1U,
        1U, 1U, 1U, 1U,
        2U, 4U, 8U, 16U,
        64U, 128U, 256U, 512U
    };

    return ahb_prescaler[(RCC->CFGR >> 4U) & 0x0FU];
}

static uint32_t RCC_GetAPBPrescaler(uint32_t shift)
{
    uint32_t encoded;

    encoded = (RCC->CFGR >> shift) & 0x07U;

    switch (encoded)
    {
        case 0U:
        case 1U:
        case 2U:
        case 3U:
            return 1U;

        case 4U:
            return 2U;

        case 5U:
            return 4U;

        case 6U:
            return 8U;

        case 7U:
            return 16U;

        default:
            return 1U;
    }
}

static uint32_t RCC_GetPLLClock(void)
{
    uint32_t pll_input;
    uint32_t pll_multiplier;

    if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0U)
    {
        /*
         * PLL source = HSI / 2
         */
        pll_input = RCC_HSI_FREQUENCY_HZ / 2U;
    }
    else
    {
        /*
         * PLL source = HSE.
         *
         * If PLLXTPRE is set, HSE is divided by 2.
         */
        pll_input = RCC_HSE_FREQUENCY_HZ;

        if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != 0U)
        {
            pll_input /= 2U;
        }
    }

    /*
     * PLLMUL encoding:
     *
     * 0000 -> x2
     * 0001 -> x3
     * ...
     * 1101 -> x15
     * 1110 -> x16
     */
    pll_multiplier = ((RCC->CFGR >> 18U) & 0x0FU) + 2U;

    return pll_input * pll_multiplier;
}

void RCC_EnablePeripheralClock(RCC_Peripheral_t peripheral)
{
    switch (peripheral)
    {
        case RCC_GPIOA:
            RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
            break;

        case RCC_GPIOB:
            RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
            break;

        case RCC_GPIOC:
            RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
            break;

        default:
            break;
    }
}

void RCC_DisablePeripheralClock(RCC_Peripheral_t peripheral)
{
    switch (peripheral)
    {
        case RCC_GPIOA:
            RCC->APB2ENR &= ~RCC_APB2ENR_IOPAEN;
            break;

        case RCC_GPIOB:
            RCC->APB2ENR &= ~RCC_APB2ENR_IOPBEN;
            break;

        case RCC_GPIOC:
            RCC->APB2ENR &= ~RCC_APB2ENR_IOPCEN;
            break;

        default:
            break;
    }
}

uint32_t RCC_GetSystemClock(void)
{
    uint32_t source;

    source = (RCC->CFGR >> 2U) & 0x03U;

    switch (source)
    {
        case 0U:
            return RCC_HSI_FREQUENCY_HZ;

        case 1U:
            return RCC_HSE_FREQUENCY_HZ;

        case 2U:
            return RCC_GetPLLClock();

        default:
            /*
             * Reserved/invalid SWS value.
             * Returning HSI provides a safe fallback.
             */
            return RCC_HSI_FREQUENCY_HZ;
    }
}

uint32_t RCC_GetAHBClock(void)
{
    uint32_t sysclk;

    sysclk = RCC_GetSystemClock();

    return sysclk / RCC_GetAHBPrescaler();
}

uint32_t RCC_GetAPB1Clock(void)
{
    uint32_t hclk;

    hclk = RCC_GetAHBClock();

    return hclk / RCC_GetAPBPrescaler(8U);
}

uint32_t RCC_GetAPB2Clock(void)
{
    uint32_t hclk;

    hclk = RCC_GetAHBClock();

    return hclk / RCC_GetAPBPrescaler(11U);
}

uint32_t RCC_GetAPB1TimerClock(void)
{
    uint32_t hclk;
    uint32_t prescaler;

    hclk = RCC_GetAHBClock();
    prescaler = RCC_GetAPBPrescaler(8U);

    /*
     * STM32F1 timer clock:
     *
     * APB prescaler = 1
     *     timer clock = PCLK
     *
     * APB prescaler != 1
     *     timer clock = 2 x PCLK
     */
    if (prescaler == 1U)
    {
        return hclk;
    }

    return (hclk / prescaler) * 2U;
}

uint32_t RCC_GetAPB2TimerClock(void)
{
    uint32_t hclk;
    uint32_t prescaler;

    hclk = RCC_GetAHBClock();
    prescaler = RCC_GetAPBPrescaler(11U);

    if (prescaler == 1U)
    {
        return hclk;
    }

    return (hclk / prescaler) * 2U;
}

RCC_ClockSource_t RCC_GetSystemClockSource(void)
{
    uint32_t source;

    source = (RCC->CFGR >> 2U) & 0x03U;

    switch (source)
    {
        case 0U:
            return RCC_CLOCK_HSI;

        case 1U:
            return RCC_CLOCK_HSE;

        case 2U:
            return RCC_CLOCK_PLL;

        default:
            return RCC_CLOCK_HSI;
    }
}
