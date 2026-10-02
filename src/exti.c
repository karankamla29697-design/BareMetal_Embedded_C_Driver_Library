#include "exti.h"
#include "nvic.h"
#include "rcc.h"

static EXTI_Callback_t exti_callbacks[16];
static uint8_t exti_status = EXTI_OK;

static uint8_t EXTI_IsValidPin(uint8_t pin)
{
    return (pin < 16U) ? 1U : 0U;
}

static uint8_t EXTI_GetIRQNumber(uint8_t pin)
{
    if (pin <= 4U)
    {
        return (uint8_t)(6U + pin);
    }

    if (pin <= 9U)
    {
        return 23U;
    }

    return 40U;
}

static uint8_t EXTI_GetPortCode(GPIO_Port_t port)
{
    switch (port)
    {
        case GPIO_PORT_A:
            return 0U;

        case GPIO_PORT_B:
            return 1U;

        case GPIO_PORT_C:
            return 2U;

        default:
            return 0xFFU;
    }
}

uint8_t EXTI_Init(
    GPIO_Port_t port,
    uint8_t pin,
    EXTI_Trigger_t trigger,
    EXTI_Callback_t callback
)
{
    uint8_t port_code;
    uint8_t irq;

    if (!EXTI_IsValidPin(pin))
    {
        exti_status = EXTI_ERROR;
        return EXTI_ERROR;
    }

    if (trigger > EXTI_TRIGGER_RISING_FALLING)
    {
        exti_status = EXTI_ERROR;
        return EXTI_ERROR;
    }

    port_code = EXTI_GetPortCode(port);

    if (port_code == 0xFFU)
    {
        exti_status = EXTI_ERROR;
        return EXTI_ERROR;
    }

    /*
     * AFIO controls EXTI line-to-port mapping.
     */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /*
     * Each EXTICR register contains four 4-bit fields.
     */
    {
        uint8_t register_index = pin / 4U;
        uint8_t field_shift = (pin % 4U) * 4U;

        AFIO->EXTICR[register_index] &=
            ~(0xFU << field_shift);

        AFIO->EXTICR[register_index] |=
            ((uint32_t)port_code << field_shift);
    }

    /*
     * Configure trigger.
     */
    switch (trigger)
    {
        case EXTI_TRIGGER_RISING:
            EXTI->RTSR |= (1UL << pin);
            EXTI->FTSR &= ~(1UL << pin);
            break;

        case EXTI_TRIGGER_FALLING:
            EXTI->RTSR &= ~(1UL << pin);
            EXTI->FTSR |= (1UL << pin);
            break;

        case EXTI_TRIGGER_RISING_FALLING:
            EXTI->RTSR |= (1UL << pin);
            EXTI->FTSR |= (1UL << pin);
            break;

        default:
            exti_status = EXTI_ERROR;
            return EXTI_ERROR;
    }

    /*
     * Store callback.
     */
    exti_callbacks[pin] = callback;

    /*
     * Clear stale pending interrupt before enabling.
     */
    EXTI->PR = (1UL << pin);

    /*
     * Unmask interrupt.
     */
    EXTI->IMR |= (1UL << pin);

    /*
     * Enable matching NVIC IRQ.
     */
    irq = EXTI_GetIRQNumber(pin);
    NVIC_EnableIRQ(irq);

    exti_status = EXTI_OK;

    return EXTI_OK;
}

uint8_t EXTI_Enable(uint8_t pin)
{
    if (!EXTI_IsValidPin(pin))
    {
        exti_status = EXTI_ERROR;
        return EXTI_ERROR;
    }

    EXTI->IMR |= (1UL << pin);

    NVIC_EnableIRQ(EXTI_GetIRQNumber(pin));

    exti_status = EXTI_OK;
    return EXTI_OK;
}

uint8_t EXTI_Disable(uint8_t pin)
{
    if (!EXTI_IsValidPin(pin))
    {
        exti_status = EXTI_ERROR;
        return EXTI_ERROR;
    }

    EXTI->IMR &= ~(1UL << pin);

    NVIC_DisableIRQ(EXTI_GetIRQNumber(pin));

    exti_status = EXTI_OK;
    return EXTI_OK;
}

uint8_t EXTI_ClearPending(uint8_t pin)
{
    if (!EXTI_IsValidPin(pin))
    {
        exti_status = EXTI_ERROR;
        return EXTI_ERROR;
    }

    /*
     * EXTI pending register is cleared by writing 1.
     */
    EXTI->PR = (1UL << pin);

    exti_status = EXTI_OK;
    return EXTI_OK;
}

uint8_t EXTI_GetPending(uint8_t pin)
{
    if (!EXTI_IsValidPin(pin))
    {
        exti_status = EXTI_ERROR;
        return 0U;
    }

    return ((EXTI->PR & (1UL << pin)) != 0U) ? 1U : 0U;
}

uint8_t EXTI_GetStatus(void)
{
    return exti_status;
}

/*
 * Shared interrupt handlers.
 */
void EXTI0_IRQHandler(void)
{
    if ((EXTI->PR & (1UL << 0)) != 0U)
    {
        EXTI->PR = (1UL << 0);

        if (exti_callbacks[0] != 0)
        {
            exti_callbacks[0]();
        }
    }
}

void EXTI1_IRQHandler(void)
{
    if ((EXTI->PR & (1UL << 1)) != 0U)
    {
        EXTI->PR = (1UL << 1);

        if (exti_callbacks[1] != 0)
        {
            exti_callbacks[1]();
        }
    }
}

void EXTI2_IRQHandler(void)
{
    if ((EXTI->PR & (1UL << 2)) != 0U)
    {
        EXTI->PR = (1UL << 2);

        if (exti_callbacks[2] != 0)
        {
            exti_callbacks[2]();
        }
    }
}

void EXTI3_IRQHandler(void)
{
    if ((EXTI->PR & (1UL << 3)) != 0U)
    {
        EXTI->PR = (1UL << 3);

        if (exti_callbacks[3] != 0)
        {
            exti_callbacks[3]();
        }
    }
}

void EXTI4_IRQHandler(void)
{
    if ((EXTI->PR & (1UL << 4)) != 0U)
    {
        EXTI->PR = (1UL << 4);

        if (exti_callbacks[4] != 0)
        {
            exti_callbacks[4]();
        }
    }
}

void EXTI9_5_IRQHandler(void)
{
    uint8_t pin;

    for (pin = 5U; pin <= 9U; pin++)
    {
        if ((EXTI->PR & (1UL << pin)) != 0U)
        {
            EXTI->PR = (1UL << pin);

            if (exti_callbacks[pin] != 0)
            {
                exti_callbacks[pin]();
            }
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    uint8_t pin;

    for (pin = 10U; pin <= 15U; pin++)
    {
        if ((EXTI->PR & (1UL << pin)) != 0U)
        {
            EXTI->PR = (1UL << pin);

            if (exti_callbacks[pin] != 0)
            {
                exti_callbacks[pin]();
            }
        }
    }
}
