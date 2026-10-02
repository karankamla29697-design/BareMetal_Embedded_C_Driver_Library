#ifndef GPIO_H
#define GPIO_H

#include "stm32f103xx.h"

/* =========================================================
 * GPIO Port Definition
 * ========================================================= */

typedef enum
{
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C

} GPIO_Port_t;

/* =========================================================
 * GPIO Pin Modes
 * ========================================================= */

#define GPIO_MODE_INPUT          0x00U
#define GPIO_MODE_OUTPUT_10MHZ   0x01U
#define GPIO_MODE_OUTPUT_2MHZ    0x02U
#define GPIO_MODE_OUTPUT_50MHZ   0x03U
#define GPIO_INPUT_ANALOG        0x00U
#define GPIO_INPUT_FLOATING      0x04U
#define GPIO_INPUT_PULL          0x08U


/* =========================================================
 * GPIO Output Configuration
 * ========================================================= */

#define GPIO_OUTPUT_PP           0x00U
#define GPIO_OUTPUT_OD           0x04U
#define GPIO_OUTPUT_AF_PP        0x08U
#define GPIO_OUTPUT_AF_OD        0x0CU

/* =========================================================
 * GPIO Pin State
 * ========================================================= */

#define GPIO_PIN_LOW             0U
#define GPIO_PIN_HIGH            1U

/* =========================================================
 * GPIO Driver Functions
 * ========================================================= */

void GPIO_Init(GPIO_Port_t port,
               uint8_t pin,
               uint8_t mode,
               uint8_t config);

void GPIO_WritePin(GPIO_Port_t port,
                   uint8_t pin,
                   uint8_t state);

void GPIO_ResetPin(GPIO_Port_t port,
                   uint8_t pin);

void GPIO_SetPin(GPIO_Port_t port,
                 uint8_t pin);

void GPIO_TogglePin(GPIO_Port_t port,
                    uint8_t pin);

uint8_t GPIO_ReadPin(GPIO_Port_t port,
                     uint8_t pin);

#endif /* GPIO_H */