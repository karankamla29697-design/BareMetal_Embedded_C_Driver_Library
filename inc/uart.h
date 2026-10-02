#ifndef UART_H
#define UART_H

#include "stm32f103xx.h"

/* =========================================================
 * UART Configuration
 * ========================================================= */

typedef struct
{
    uint32_t baud_rate;

} UART_Config_t;

/* =========================================================
 * UART Driver
 * ========================================================= */

void UART_Init(uint32_t baud_rate);

void UART_SendChar(char character);

void UART_SendString(const char *string);

char UART_ReceiveChar(void);

#endif /* UART_H */
