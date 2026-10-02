#ifndef PWM_H
#define PWM_H

#include "stm32f103xx.h"

#define PWM_OK       0U
#define PWM_ERROR    1U

/*
 * STM32F103:
 * TIM2_CH1 -> PA0
 */
#define PWM_OUTPUT_PORT   GPIO_PORT_A
#define PWM_OUTPUT_PIN    0U

uint8_t PWM_Init(uint32_t frequency_hz);

void PWM_Start(void);
void PWM_Stop(void);

uint8_t PWM_SetDutyCycle(uint8_t duty_cycle);
uint8_t PWM_GetDutyCycle(void);

uint32_t PWM_GetFrequency(void);
uint8_t PWM_GetStatus(void);

#endif
