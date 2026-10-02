#include "uart.h"
#include "gpio.h"
#include "rcc.h"

/*
 * Freestanding unsigned 32-bit division.
 *
 * Returns quotient and optionally remainder without using
 * the C division operator. This avoids compiler-generated
 * __aeabi_uidivmod/__aeabi_uldivmod helpers when linking
 * with -nostdlib.
 */
static uint32_t UART_DivideU32(
    uint32_t dividend,
    uint32_t divisor,
    uint32_t *remainder)
{
    uint32_t quotient = 0U;
    uint32_t rem = 0U;
    uint32_t i;

    if (divisor == 0U)
    {
        if (remainder != 0)
        {
            *remainder = 0U;
        }

        return 0U;
    }

    for (i = 32U; i > 0U; i--)
    {
        rem <<= 1U;

        if ((dividend & 0x80000000UL) != 0U)
        {
            rem |= 1U;
        }

        dividend <<= 1U;
        quotient <<= 1U;

        if (rem >= divisor)
        {
            rem -= divisor;
            quotient |= 1U;
        }
    }

    if (remainder != 0)
    {
        *remainder = rem;
    }

    return quotient;
}

void UART_Init(uint32_t baud_rate)
{
    if (baud_rate == 0U)
    {
        return;
    }

    /*
     * USART1 is connected to APB2.
     */
    uint32_t pclk2 = RCC_GetAPB2Clock();

    if (pclk2 == 0U)
    {
        return;
    }

    /*
     * USART1 pins:
     *
     * PA9  = TX
     * PA10 = RX
     */
    GPIO_Init(
        GPIO_PORT_A,
        9U,
        GPIO_MODE_OUTPUT_50MHZ,
        GPIO_OUTPUT_AF_PP
    );

    GPIO_Init(
        GPIO_PORT_A,
        10U,
        GPIO_MODE_INPUT,
        GPIO_INPUT_FLOATING
    );

    /*
     * STM32F1 USART BRR:
     *
     *     BRR = round(PCLK2 / baud)
     *
     * Calculate quotient and remainder manually.
     */
    uint32_t remainder = 0U;

    uint32_t quotient =
        UART_DivideU32(
            pclk2,
            baud_rate,
            &remainder
        );

    /*
     * Round to nearest integer.
     *
     * remainder / baud_rate >= 0.5
     */
    uint32_t doubled_remainder = remainder << 1U;

    if (doubled_remainder >= baud_rate)
    {
        quotient++;
    }

    /*
     * USART BRR is 16-bit.
     */
    if (quotient > 0xFFFFU)
    {
        quotient = 0xFFFFU;
    }

    USART1->BRR = quotient;

    /*
     * Enable:
     * TE = transmitter
     * RE = receiver
     * UE = USART
     */
    USART1->CR1 =
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_UE;

    /*
     * 8 data bits, no parity, 1 stop bit.
     */
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;
}

void UART_SendChar(char character)
{
    while ((USART1->SR & USART_SR_TXE) == 0U)
    {
    }

    USART1->DR = (uint32_t)(uint8_t)character;

    while ((USART1->SR & USART_SR_TC) == 0U)
    {
    }
}

void UART_SendString(const char *string)
{
    if (string == 0)
    {
        return;
    }

    while (*string != '\0')
    {
        UART_SendChar(*string);
        string++;
    }
}

char UART_ReceiveChar(void)
{
    while ((USART1->SR & USART_SR_RXNE) == 0U)
    {
    }

    return (char)(USART1->DR & 0xFFU);
}
