#include "systick.h"
#include "rcc.h"

static volatile uint32_t systick_tick = 0U;

void SysTick_Init(void)
{
    uint32_t clock_hz;
    uint32_t reload;

    clock_hz = RCC_GetAHBClock();

    /*
     * 1 ms SysTick period.
     * Reload = HCLK / 1000 - 1
     */
    if (clock_hz < 1000U)
    {
        SysTick->CTRL = 0U;
        SysTick->LOAD = 0U;
        SysTick->VAL  = 0U;
        return;
    }

    reload = (clock_hz / 1000U) - 1U;

    /*
     * SysTick LOAD is 24 bits.
     */
    if (reload > 0x00FFFFFFUL)
    {
        SysTick->CTRL = 0U;
        SysTick->LOAD = 0U;
        SysTick->VAL  = 0U;
        return;
    }

    SysTick->LOAD = reload;
    SysTick->VAL  = 0U;

    /*
     * CLKSOURCE = processor clock
     * TICKINT   = enable interrupt
     * ENABLE    = start counter
     */
    SysTick->CTRL =
        SYSTICK_CTRL_CLKSOURCE |
        SYSTICK_CTRL_TICKINT |
        SYSTICK_CTRL_ENABLE;
}

void SysTick_Handler(void)
{
    systick_tick++;
}

uint32_t SysTick_GetTick(void)
{
    return systick_tick;
}

void SysTick_DelayMs(uint32_t milliseconds)
{
    uint32_t start;

    start = systick_tick;

    while ((uint32_t)(systick_tick - start) < milliseconds)
    {
        /*
         * Wait for SysTick interrupt.
         */
    }
}

void SysTick_DelayUs(uint32_t microseconds)
{
    uint32_t clock_hz;
    uint32_t cycles;
    uint32_t start;
    uint32_t elapsed;

    if (microseconds == 0U)
    {
        return;
    }

    clock_hz = RCC_GetAHBClock();

    /*
     * Convert microseconds to CPU cycles without using
     * floating-point arithmetic.
     */
    cycles = (clock_hz / 1000000U) * microseconds;

    /*
     * If the CPU clock is not an exact multiple of 1 MHz,
     * add the fractional remainder.
     */
    if ((clock_hz % 1000000U) != 0U)
    {
        cycles += ((clock_hz % 1000000U) * microseconds) / 1000000U;
    }

    if (cycles == 0U)
    {
        return;
    }

    /*
     * Use the free-running SysTick counter.
     *
     * LOAD contains the reload value, therefore the counter
     * period is LOAD + 1 cycles.
     */
    start = SysTick->VAL;

    do
    {
        uint32_t current;

        current = SysTick->VAL;

        if (start >= current)
        {
            elapsed = start - current;
        }
        else
        {
            elapsed = start + (SysTick->LOAD + 1U) - current;
        }

    } while (elapsed < cycles);
}
