# Examples

This directory contains example applications demonstrating how the bare-metal drivers can be used.

The main firmware currently located in `src/main.c` acts as the integrated driver demonstration.

Future examples can be separated into independent applications such as:

- GPIO LED blink
- UART communication
- SPI sensor interface
- I2C sensor interface
- PWM output
- ADC measurement
- External interrupt
- Watchdog recovery

When creating standalone examples, each example should use the same driver APIs from `inc/`.
