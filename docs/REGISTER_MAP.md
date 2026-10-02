# STM32F103C8T6 Register Map

This document records the main peripheral register blocks used by the Bare-Metal Embedded C Driver Library.

## Peripheral Base Addresses

| Peripheral | Base Address |
|---|---:|
| GPIOA | 0x40010800 |
| GPIOB | 0x40010C00 |
| GPIOC | 0x40011000 |
| AFIO | 0x40010000 |
| EXTI | 0x40010400 |
| RCC | 0x40021000 |
| USART1 | 0x40013800 |
| SPI1 | 0x40013000 |
| I2C1 | 0x40005400 |
| TIM2 | 0x40000000 |
| ADC1 | 0x40012400 |
| IWDG | 0x40003000 |
| SysTick | Cortex-M3 System Control Space |

## GPIO

GPIO registers used by the driver:

- CRL
- CRH
- IDR
- ODR
- BSRR
- BRR

## RCC

RCC registers used by the driver:

- CR
- CFGR
- APB2ENR
- APB1ENR

## USART1

USART registers used:

- SR
- DR
- BRR
- CR1
- CR2
- CR3

## SPI1

SPI registers used:

- CR1
- CR2
- SR
- DR

## I2C1

I2C registers used:

- CR1
- CR2
- OAR1
- OAR2
- DR
- SR1
- SR2
- CCR
- TRISE

## TIM2

Timer registers used:

- CR1
- DIER
- SR
- EGR
- CCMR1
- CCER
- CNT
- PSC
- ARR
- CCR1

## ADC1

ADC registers used:

- SR
- CR1
- CR2
- SMPR1
- SMPR2
- SQR1
- SQR2
- SQR3
- DR

## EXTI

EXTI registers used:

- IMR
- EMR
- RTSR
- FTSR
- SWIER
- PR

## IWDG

Independent watchdog registers used:

- KR
- PR
- RLR
- SR

This document is intended as a project-level register reference. The STM32F103 reference manual remains the authoritative source for complete register definitions and electrical behavior.
