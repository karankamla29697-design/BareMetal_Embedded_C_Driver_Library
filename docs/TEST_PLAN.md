# Driver Test Plan

## Build Tests

- [x] Compile all C source files
- [x] Assemble startup code
- [x] Link ELF
- [x] Generate BIN
- [x] Check undefined symbols
- [x] Check ARM runtime division helpers
- [x] Check ELF size

## GPIO

- [ ] Configure output pin
- [ ] Set output high
- [ ] Set output low
- [ ] Toggle output
- [ ] Read input
- [ ] Test pull-up input

## RCC

- [ ] Verify HSI clock
- [ ] Verify HCLK calculation
- [ ] Verify APB1 calculation
- [ ] Verify APB2 calculation
- [ ] Verify timer clock calculation

## SysTick

- [ ] Verify 1 ms tick
- [ ] Verify millisecond delay
- [ ] Verify microsecond delay
- [ ] Verify tick counter

## UART

- [ ] Initialize USART1
- [ ] Transmit text
- [ ] Receive byte
- [ ] Verify baud rate using serial terminal

## SPI

- [ ] Initialize SPI1
- [ ] Transmit byte
- [ ] Receive byte
- [ ] Verify transfer with SPI peripheral

## I2C

- [ ] Initialize I2C1
- [ ] Generate START
- [ ] Address slave
- [ ] Write byte
- [ ] Read byte
- [ ] Generate STOP
- [ ] Test register access

## Timer

- [ ] Start TIM2
- [ ] Stop TIM2
- [ ] Reset TIM2
- [ ] Read counter
- [ ] Detect update event

## PWM

- [ ] Generate PWM
- [ ] Verify frequency
- [ ] Test 0% duty cycle
- [ ] Test 50% duty cycle
- [ ] Test 100% duty cycle

## ADC

- [ ] Initialize ADC1
- [ ] Read ADC channel
- [ ] Convert ADC reading to millivolts
- [ ] Test multiple channels

## EXTI / NVIC

- [ ] Configure external interrupt
- [ ] Verify trigger edge
- [ ] Verify callback
- [ ] Clear pending interrupt
- [ ] Verify NVIC interrupt enable

## IWDG

- [ ] Initialize watchdog
- [ ] Refresh watchdog
- [ ] Verify timeout behavior
- [ ] Verify reset after timeout

## Hardware Validation

Software build success does not replace physical hardware testing. Each peripheral must be validated on an STM32F103C8T6 board with appropriate external hardware.
