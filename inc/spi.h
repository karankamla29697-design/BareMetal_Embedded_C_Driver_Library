#ifndef SPI_H
#define SPI_H

#include "stm32f103xx.h"

typedef enum
{
    SPI_BAUD_DIV2   = (0U << 3),
    SPI_BAUD_DIV4   = (1U << 3),
    SPI_BAUD_DIV8   = (2U << 3),
    SPI_BAUD_DIV16  = (3U << 3),
    SPI_BAUD_DIV32  = (4U << 3),
    SPI_BAUD_DIV64  = (5U << 3),
    SPI_BAUD_DIV128 = (6U << 3),
    SPI_BAUD_DIV256 = (7U << 3)
} SPI_BaudRate_t;

typedef struct
{
    SPI_BaudRate_t baud_rate;
    uint8_t cpol;
    uint8_t cpha;
    uint8_t lsb_first;
} SPI_Config_t;

#define SPI_CPOL_LOW       0U
#define SPI_CPOL_HIGH      1U

#define SPI_CPHA_FIRST     0U
#define SPI_CPHA_SECOND    1U

#define SPI_MSB_FIRST      0U
#define SPI_LSB_FIRST      1U

#define SPI_OK             0U
#define SPI_ERROR          1U
#define SPI_TIMEOUT        2U

void SPI_Init(SPI_BaudRate_t baud_rate);
void SPI_InitAdvanced(const SPI_Config_t *config);

uint8_t SPI_Transfer(uint8_t data);
uint8_t SPI_SendByte(uint8_t data);
uint8_t SPI_ReceiveByte(uint8_t *data);

uint8_t SPI_GetStatus(void);

#endif
