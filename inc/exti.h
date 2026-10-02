#ifndef EXTI_H
#define EXTI_H

#include "stm32f103xx.h"
#include "gpio.h"

#define EXTI_OK        0U
#define EXTI_ERROR     1U

typedef enum
{
    EXTI_TRIGGER_RISING = 0U,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_RISING_FALLING
} EXTI_Trigger_t;

typedef void (*EXTI_Callback_t)(void);

uint8_t EXTI_Init(
    GPIO_Port_t port,
    uint8_t pin,
    EXTI_Trigger_t trigger,
    EXTI_Callback_t callback
);

uint8_t EXTI_Enable(uint8_t pin);
uint8_t EXTI_Disable(uint8_t pin);
uint8_t EXTI_ClearPending(uint8_t pin);
uint8_t EXTI_GetPending(uint8_t pin);

uint8_t EXTI_GetStatus(void);

#endif
