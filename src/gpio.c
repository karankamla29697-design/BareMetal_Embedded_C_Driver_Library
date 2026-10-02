#include "gpio.h"
#include "rcc.h"

static GPIO_TypeDef *GPIO_GetPort(GPIO_Port_t port)
{
    switch (port)
    {
        case GPIO_PORT_A:
            return GPIOA;

        case GPIO_PORT_B:
            return GPIOB;

        case GPIO_PORT_C:
            return GPIOC;

        default:
            return (GPIO_TypeDef *)0;
    }
}

static uint8_t GPIO_IsValidPin(uint8_t pin)
{
    return (pin < 16U) ? 1U : 0U;
}

static uint8_t GPIO_IsValidMode(uint8_t mode)
{
    return (mode <= GPIO_MODE_OUTPUT_50MHZ) ? 1U : 0U;
}

static uint8_t GPIO_IsValidConfig(uint8_t mode, uint8_t config)
{
    if (mode == GPIO_MODE_INPUT)
    {
        return (config == GPIO_INPUT_ANALOG) ||
               (config == GPIO_INPUT_FLOATING) ||
               (config == GPIO_INPUT_PULL);
    }

    return (config == GPIO_OUTPUT_PP) ||
           (config == GPIO_OUTPUT_OD) ||
           (config == GPIO_OUTPUT_AF_PP) ||
           (config == GPIO_OUTPUT_AF_OD);
}

void GPIO_Init(
    GPIO_Port_t port,
    uint8_t pin,
    uint8_t mode,
    uint8_t config)
{
    GPIO_TypeDef *gpio;
    volatile uint32_t *config_register;
    uint8_t shift;
    uint32_t value;

    if (!GPIO_IsValidPin(pin) ||
        !GPIO_IsValidMode(mode) ||
        !GPIO_IsValidConfig(mode, config))
    {
        return;
    }

    gpio = GPIO_GetPort(port);

    if (gpio == (GPIO_TypeDef *)0)
    {
        return;
    }

    RCC_EnablePeripheralClock((RCC_Peripheral_t)port);

    /*
     * STM32F103 GPIO configuration:
     *
     * MODE[1:0]:
     *   00 = input
     *   01 = output 10 MHz
     *   10 = output 2 MHz
     *   11 = output 50 MHz
     *
     * CNF[1:0]:
     *   Input:
     *     00 = analog
     *     01 = floating
     *     10 = input pull-up/pull-down
     *
     *   Output:
     *     00 = push-pull
     *     01 = open-drain
     *     10 = alternate-function push-pull
     *     11 = alternate-function open-drain
     */

    if (pin < 8U)
    {
        config_register = &gpio->CRL;
        shift = (uint8_t)(pin * 4U);
    }
    else
    {
        config_register = &gpio->CRH;
        shift = (uint8_t)((pin - 8U) * 4U);
    }

    value = *config_register;

    /* Clear existing 4-bit configuration. */
    value &= ~(0x0FU << shift);

    /* MODE occupies bits [1:0]. */
    value |= ((uint32_t)mode << shift);

    /* CNF occupies bits [3:2]. */
    value |= ((uint32_t)(config & 0x0CU) << shift);

    *config_register = value;

    /*
     * For input pull-up/pull-down:
     * ODR = 1 -> pull-up
     * ODR = 0 -> pull-down
     *
     * Default to pull-up when GPIO_INPUT_PULL is selected.
     * GPIO_WritePin() can subsequently change the ODR bit.
     */
    if ((mode == GPIO_MODE_INPUT) &&
        (config == GPIO_INPUT_PULL))
    {
        gpio->BSRR = (1U << pin);
    }
}

void GPIO_WritePin(
    GPIO_Port_t port,
    uint8_t pin,
    uint8_t state)
{
    GPIO_TypeDef *gpio;

    if (!GPIO_IsValidPin(pin))
    {
        return;
    }

    gpio = GPIO_GetPort(port);

    if (gpio == (GPIO_TypeDef *)0)
    {
        return;
    }

    if (state == GPIO_PIN_HIGH)
    {
        gpio->BSRR = (1U << pin);
    }
    else
    {
        gpio->BSRR = (1U << (pin + 16U));
    }
}

void GPIO_ResetPin(
    GPIO_Port_t port,
    uint8_t pin)
{
    GPIO_TypeDef *gpio;

    if (!GPIO_IsValidPin(pin))
    {
        return;
    }

    gpio = GPIO_GetPort(port);

    if (gpio == (GPIO_TypeDef *)0)
    {
        return;
    }

    gpio->BSRR = (1U << (pin + 16U));
}

void GPIO_SetPin(
    GPIO_Port_t port,
    uint8_t pin)
{
    GPIO_TypeDef *gpio;

    if (!GPIO_IsValidPin(pin))
    {
        return;
    }

    gpio = GPIO_GetPort(port);

    if (gpio == (GPIO_TypeDef *)0)
    {
        return;
    }

    gpio->BSRR = (1U << pin);
}

void GPIO_TogglePin(
    GPIO_Port_t port,
    uint8_t pin)
{
    GPIO_TypeDef *gpio;
    uint32_t odr;

    if (!GPIO_IsValidPin(pin))
    {
        return;
    }

    gpio = GPIO_GetPort(port);

    if (gpio == (GPIO_TypeDef *)0)
    {
        return;
    }

    odr = gpio->ODR;

    if ((odr & (1U << pin)) != 0U)
    {
        gpio->BSRR = (1U << (pin + 16U));
    }
    else
    {
        gpio->BSRR = (1U << pin);
    }
}

uint8_t GPIO_ReadPin(
    GPIO_Port_t port,
    uint8_t pin)
{
    GPIO_TypeDef *gpio;

    if (!GPIO_IsValidPin(pin))
    {
        return GPIO_PIN_LOW;
    }

    gpio = GPIO_GetPort(port);

    if (gpio == (GPIO_TypeDef *)0)
    {
        return GPIO_PIN_LOW;
    }

    return ((gpio->IDR & (1U << pin)) != 0U)
               ? GPIO_PIN_HIGH
               : GPIO_PIN_LOW;
}
