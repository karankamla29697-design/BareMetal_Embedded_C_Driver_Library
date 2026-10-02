#include "nvic.h"

#define NVIC_ISER_BASE    0xE000E100UL
#define NVIC_ICER_BASE    0xE000E180UL
#define NVIC_IPR_BASE     0xE000E400UL

static volatile uint32_t * const nvic_iser =
    (volatile uint32_t *)NVIC_ISER_BASE;

static volatile uint32_t * const nvic_icer =
    (volatile uint32_t *)NVIC_ICER_BASE;

static volatile uint8_t * const nvic_ipr =
    (volatile uint8_t *)NVIC_IPR_BASE;

void NVIC_EnableIRQ(uint8_t irq_number)
{
    if (irq_number < 60U)
    {
        nvic_iser[irq_number / 32U] =
            (1UL << (irq_number % 32U));
    }
}

void NVIC_DisableIRQ(uint8_t irq_number)
{
    if (irq_number < 60U)
    {
        nvic_icer[irq_number / 32U] =
            (1UL << (irq_number % 32U));
    }
}

void NVIC_SetPriority(uint8_t irq_number, uint8_t priority)
{
    if (irq_number < 60U)
    {
        /*
         * STM32F1 implements the upper priority bits.
         * Shift priority into the implemented bits.
         */
        nvic_ipr[irq_number] = (uint8_t)(priority << 4U);
    }
}
