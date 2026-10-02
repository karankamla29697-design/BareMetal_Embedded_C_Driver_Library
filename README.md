# Bare-Metal Embedded C Driver Library

A from-scratch bare-metal peripheral driver library for the STM32F103C8T6 (Blue Pill), written in Embedded C without STM32 HAL or vendor peripheral driver libraries.

## Target

- MCU: STM32F103C8T6
- Core: ARM Cortex-M3
- Architecture: ARMv7-M
- Flash: 64 KB
- SRAM: 20 KB
- Toolchain: arm-none-eabi-gcc
- Build system: GNU Make
- Host development: VS Code + WSL Ubuntu

## Drivers

- GPIO
- RCC / Clock Tree
- SysTick
- UART / USART1
- SPI1
- I2C1
- TIM2
- PWM / TIM2 CH1
- ADC1
- EXTI
- NVIC
- Independent Watchdog (IWDG)

## Design Approach

This project implements peripheral drivers directly through STM32F103 registers.

The library avoids STM32 HAL, STM32 LL drivers, vendor peripheral libraries, dynamic memory allocation, and standard C runtime dependencies.

The goal is to understand MCU hardware at register level and provide a reusable Embedded C driver foundation.

## Build

Run:

    make clean
    make

The build generates baremetal.elf, baremetal.bin, and baremetal.map.

## Hardware

Target development board: STM32F103C8T6 Blue Pill.

Recommended tools: ST-Link V2, USB-to-UART adapter, breadboard, jumper wires, and 3.3 V compatible peripherals.

## Project Status

Implemented drivers: GPIO, RCC, SysTick, UART, SPI, I2C, Timer, PWM, ADC, EXTI, NVIC, and IWDG.

The firmware builds as a bare-metal STM32F103C8T6 ELF and binary image.

Hardware validation requires flashing the firmware to a physical STM32F103C8T6 board and testing each peripheral with appropriate external hardware.

## License

This project is intended for educational and portfolio purposes.
