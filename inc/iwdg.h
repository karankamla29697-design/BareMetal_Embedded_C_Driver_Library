#ifndef IWDG_H
#define IWDG_H

#include "stm32f103xx.h"

/*
 * STM32F103 internal LSI is nominally around 40 kHz.
 *
 * Actual LSI frequency varies between devices and conditions,
 * so watchdog timeout values are approximate unless the LSI
 * is measured/calibrated.
 */
#define IWDG_LSI_FREQUENCY_HZ 40000UL

typedef enum
{
    IWDG_PRESCALER_4 = 0U,
    IWDG_PRESCALER_8 = 1U,
    IWDG_PRESCALER_16 = 2U,
    IWDG_PRESCALER_32 = 3U,
    IWDG_PRESCALER_64 = 4U,
    IWDG_PRESCALER_128 = 5U,
    IWDG_PRESCALER_256 = 6U
} IWDG_Prescaler_t;

/*
 * Initializes and STARTS the independent watchdog.
 *
 * Important:
 * Once started, the STM32 IWDG cannot be stopped by software
 * during normal operation. The MCU must periodically call
 * IWDG_Refresh() or it will reset.
 *
 * Returns:
 *   0 -> success
 *   1 -> timeout cannot be represented by IWDG
 */
uint8_t IWDG_Init(uint32_t timeout_ms);

/*
 * Reloads the watchdog counter.
 */
void IWDG_Refresh(void);

/*
 * Returns the approximately configured timeout in milliseconds.
 */
uint32_t IWDG_GetTimeoutMs(void);

/*
 * Returns the selected watchdog prescaler.
 */
IWDG_Prescaler_t IWDG_GetPrescaler(void);

#endif
