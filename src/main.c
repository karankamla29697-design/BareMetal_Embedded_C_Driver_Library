#include "gpio.h"
#include "rcc.h"
#include "systick.h"
#include "uart.h"
#include "spi.h"
#include "i2c.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "exti.h"
#include "iwdg.h"

static volatile uint32_t button_interrupt_count = 0U;

static void Button_Callback(void)
{
    button_interrupt_count++;

    /*
     * Blue Pill LED on PC13 is normally active-low.
     */
    GPIO_TogglePin(GPIO_PORT_C, 13U);
}



static void UART_SendNumber(uint32_t value)
{
    char buffer[11];
    uint32_t index = 0U;

    if (value == 0U)
    {
        UART_SendChar('0');
        return;
    }

    while (value > 0U)
    {
        buffer[index++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (index > 0U)
    {
        UART_SendChar(buffer[--index]);
    }
}

static void UART_SendClock(const char *name, uint32_t value)
{
    UART_SendString(name);
    UART_SendNumber(value);
    UART_SendString(" Hz\r\n");
}

int main(void)
{
    SysTick_Init();

    UART_Init(115200U);

    UART_SendClock("SYSCLK = ", RCC_GetSystemClock());
    UART_SendClock("HCLK   = ", RCC_GetAHBClock());
    UART_SendClock("PCLK1  = ", RCC_GetAPB1Clock());
    UART_SendClock("PCLK2  = ", RCC_GetAPB2Clock());
    UART_SendClock("TIM1/8 domain = ", RCC_GetAPB2TimerClock());
    UART_SendClock("TIM2/3/4 domain = ", RCC_GetAPB1TimerClock());

    SPI_Init(SPI_BAUD_DIV16);

    I2C_Init(100000U);

    TIMER2_Init(7U, 999U);

    PWM_Init(1000U);
    PWM_SetDutyCycle(50U);
    PWM_Start();

    ADC_Init();

    /*
     * Start the independent watchdog with an approximately
     * 2 second timeout.
     *
     * IMPORTANT:
     * Once started, IWDG cannot be stopped by software.
     * The main loop must periodically refresh it.
     */
    if (IWDG_Init(2000U) != 0U)
    {
        /*
         * Watchdog configuration failed.
         * Stay here instead of continuing.
         */
        while (1)
        {
        }
    }

    /*
     * Blue Pill LED.
     */
    GPIO_Init(GPIO_PORT_C,
              13U,
              GPIO_MODE_OUTPUT_2MHZ,
              GPIO_OUTPUT_PP);

    /*
     * PA0 as input for EXTI0.
     *
     * External button:
     *
     * PA0 ---- button ---- GND
     *
     * External pull-up resistor is recommended.
     */
    GPIO_Init(GPIO_PORT_A,
              0U,
              GPIO_MODE_INPUT,
              GPIO_INPUT_PULL);

    /*
     * Pull-up:
     * ODR = 1 -> input pull-up.
     */
    GPIO_SetPin(GPIO_PORT_A, 0U);

    EXTI_Init(
        GPIO_PORT_A,
        0U,
        EXTI_TRIGGER_FALLING,
        Button_Callback
    );

    UART_SendString("Bare-Metal Driver Library\r\n");
    UART_SendString("USART1 initialized successfully.\r\n");
    UART_SendString("SPI1 initialized successfully.\r\n");
    UART_SendString("I2C1 initialized successfully.\r\n");
    UART_SendString("TIM2 initialized successfully.\r\n");
    UART_SendString("PWM initialized successfully.\r\n");
    UART_SendString("ADC1 initialized successfully.\r\n");
    UART_SendString("EXTI0 + NVIC initialized successfully.\r\n");
    UART_SendString("Press button on PA0 to trigger interrupt.\r\n");

    while (1)
    {
        UART_SendString("Interrupt count = ");

        UART_SendNumber(button_interrupt_count);

        UART_SendString("\r\n");

        /*
         * Refresh watchdog before the approximately 2 second
         * timeout expires.
         */
        IWDG_Refresh();

        SysTick_DelayMs(1000U);
    }
}
