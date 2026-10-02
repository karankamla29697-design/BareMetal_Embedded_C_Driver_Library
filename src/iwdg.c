#include "iwdg.h"

/*
 * IWDG timeout is derived from:
 *
 * timeout = (reload + 1) * prescaler / LSI
 *
 * Maximum:
 *
 * 4096 * 256 / 40000 ~= 26.2 seconds
 */

static uint32_t iwdg_timeout_ms = 0U;
static IWDG_Prescaler_t iwdg_prescaler = IWDG_PRESCALER_4;

static const uint16_t iwdg_prescaler_dividers[7] =
{
    4U,
    8U,
    16U,
    32U,
    64U,
    128U,
    256U
};

static uint8_t IWDG_WaitForFlagClear(uint32_t flag)
{
    uint32_t timeout = 1000000U;

    while ((IWDG->SR & flag) != 0U)
    {
        if (timeout == 0U)
        {
            return 0U;
        }

        timeout--;
    }

    return 1U;
}

uint8_t IWDG_Init(uint32_t timeout_ms)
{
    uint32_t ticks;
    uint32_t reload;
    uint32_t divider;
    uint8_t selected = 0U;
    uint8_t index;

    if (timeout_ms == 0U)
    {
        return 1U;
    }

    /*
     * Find the smallest prescaler capable of representing
     * the requested timeout.
     */
    for (index = 0U; index < 7U; index++)
    {
        divider = iwdg_prescaler_dividers[index];

        /*
         * Ceiling division:
         *
         * ticks = ceil(timeout_ms * LSI / (divider * 1000))
         */
        ticks =
            ((timeout_ms * IWDG_LSI_FREQUENCY_HZ) +
             (divider * 1000UL) - 1UL) /
            (divider * 1000UL);

        if (ticks == 0U)
        {
            ticks = 1U;
        }

        if (ticks <= 4096U)
        {
            reload = ticks - 1U;
            selected = 1U;
            break;
        }
    }

    if (selected == 0U)
    {
        /*
         * Requested timeout is larger than the maximum
         * approximately 26.2 second watchdog period.
         */
        return 1U;
    }

    iwdg_prescaler = (IWDG_Prescaler_t)index;

    /*
     * Enable write access to PR and RLR.
     */
    IWDG->KR = IWDG_KR_WRITE_ACCESS;

    /*
     * Configure prescaler.
     */
    IWDG->PR = index;

    if (IWDG_WaitForFlagClear(IWDG_SR_PVU) == 0U)
    {
        return 1U;
    }

    /*
     * Configure reload value.
     */
    IWDG->RLR = reload;

    if (IWDG_WaitForFlagClear(IWDG_SR_RVU) == 0U)
    {
        return 1U;
    }

    /*
     * Load the counter before starting.
     */
    IWDG->KR = IWDG_KR_RELOAD;

    /*
     * Start IWDG.
     *
     * Once started, it cannot be stopped by software.
     */
    IWDG->KR = IWDG_KR_ENABLE;

    /*
     * Store the actual nominal timeout represented by
     * the selected reload/prescaler.
     */
    iwdg_timeout_ms =
        ((reload + 1U) * divider * 1000UL) /
        IWDG_LSI_FREQUENCY_HZ;

    return 0U;
}

void IWDG_Refresh(void)
{
    IWDG->KR = IWDG_KR_RELOAD;
}

uint32_t IWDG_GetTimeoutMs(void)
{
    return iwdg_timeout_ms;
}

IWDG_Prescaler_t IWDG_GetPrescaler(void)
{
    return iwdg_prescaler;
}
