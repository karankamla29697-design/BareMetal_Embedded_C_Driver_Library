#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f103xx.h"

void SysTick_Init(void);
void SysTick_Handler(void);

void SysTick_DelayMs(uint32_t milliseconds);
void SysTick_DelayUs(uint32_t microseconds);

uint32_t SysTick_GetTick(void);

#endif
