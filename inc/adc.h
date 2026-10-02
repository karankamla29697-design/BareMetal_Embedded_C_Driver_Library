#ifndef ADC_H
#define ADC_H

#include "stm32f103xx.h"

#define ADC_OK       0U
#define ADC_ERROR    1U
#define ADC_TIMEOUT  2U

typedef enum
{
    ADC_CHANNEL_0  = 0U,
    ADC_CHANNEL_1  = 1U,
    ADC_CHANNEL_2  = 2U,
    ADC_CHANNEL_3  = 3U,
    ADC_CHANNEL_4  = 4U,
    ADC_CHANNEL_5  = 5U,
    ADC_CHANNEL_6  = 6U,
    ADC_CHANNEL_7  = 7U,
    ADC_CHANNEL_8  = 8U,
    ADC_CHANNEL_9  = 9U,
    ADC_CHANNEL_10 = 10U,
    ADC_CHANNEL_11 = 11U,
    ADC_CHANNEL_12 = 12U,
    ADC_CHANNEL_13 = 13U,
    ADC_CHANNEL_14 = 14U,
    ADC_CHANNEL_15 = 15U
} ADC_Channel_t;

uint8_t ADC_Init(void);

uint8_t ADC_Read(
    ADC_Channel_t channel,
    uint16_t *value
);

uint8_t ADC_ReadVoltage(
    ADC_Channel_t channel,
    uint32_t *millivolts
);

uint8_t ADC_GetStatus(void);

#endif
