#ifndef I2C_H
#define I2C_H

#include "stm32f103xx.h"

#define I2C_OK       0U
#define I2C_ERROR    1U
#define I2C_TIMEOUT  2U
#define I2C_NACK     3U

typedef enum
{
    I2C_SPEED_STANDARD_100KHZ = 100000U,
    I2C_SPEED_FAST_400KHZ     = 400000U
} I2C_Speed_t;

uint8_t I2C_Init(uint32_t speed_hz);

uint8_t I2C_Start(void);
uint8_t I2C_Stop(void);

uint8_t I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(uint8_t *data, uint8_t acknowledge);

uint8_t I2C_Write(
    uint8_t address,
    const uint8_t *data,
    uint32_t length
);

uint8_t I2C_Read(
    uint8_t address,
    uint8_t *data,
    uint32_t length
);

uint8_t I2C_WriteRegister(
    uint8_t address,
    uint8_t reg,
    const uint8_t *data,
    uint32_t length
);

uint8_t I2C_ReadRegister(
    uint8_t address,
    uint8_t reg,
    uint8_t *data,
    uint32_t length
);

uint8_t I2C_GetStatus(void);

#endif
