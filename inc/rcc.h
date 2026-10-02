#ifndef RCC_H
#define RCC_H

#include "stm32f103xx.h"

/* STM32F103 default oscillator frequencies */
#define RCC_HSI_FREQUENCY_HZ 8000000UL
#define RCC_HSE_FREQUENCY_HZ 8000000UL

typedef enum
{
    RCC_GPIOA,
    RCC_GPIOB,
    RCC_GPIOC
} RCC_Peripheral_t;

typedef enum
{
    RCC_CLOCK_HSI,
    RCC_CLOCK_HSE,
    RCC_CLOCK_PLL
} RCC_ClockSource_t;

/*
 * Peripheral clock control.
 */
void RCC_EnablePeripheralClock(RCC_Peripheral_t peripheral);
void RCC_DisablePeripheralClock(RCC_Peripheral_t peripheral);

/*
 * Clock-tree queries.
 */
uint32_t RCC_GetSystemClock(void);
uint32_t RCC_GetAHBClock(void);
uint32_t RCC_GetAPB1Clock(void);
uint32_t RCC_GetAPB2Clock(void);

uint32_t RCC_GetAPB1TimerClock(void);
uint32_t RCC_GetAPB2TimerClock(void);

/*
 * Return the currently selected SYSCLK source.
 */
RCC_ClockSource_t RCC_GetSystemClockSource(void);

#endif
