#ifndef TIMER_H
#define TIMER_H

#include "stm32f103xx.h"

#define TIMER_OK       0U
#define TIMER_ERROR    1U
#define TIMER_TIMEOUT  2U

uint8_t TIMER2_Init(uint16_t prescaler, uint16_t auto_reload);

void TIMER2_Start(void);
void TIMER2_Stop(void);
void TIMER2_Reset(void);

uint16_t TIMER2_GetCounter(void);

uint8_t TIMER2_GetUpdateFlag(void);
void TIMER2_ClearUpdateFlag(void);

uint8_t TIMER2_DelayMs(uint32_t milliseconds);

uint8_t TIMER2_GetStatus(void);

#endif
