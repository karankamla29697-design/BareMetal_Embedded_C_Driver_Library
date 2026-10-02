#ifndef NVIC_H
#define NVIC_H

#include "stm32f103xx.h"

void NVIC_EnableIRQ(uint8_t irq_number);
void NVIC_DisableIRQ(uint8_t irq_number);
void NVIC_SetPriority(uint8_t irq_number, uint8_t priority);

#endif /* NVIC_H */
