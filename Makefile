TARGET = baremetal

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = cortex-m3

CFLAGS = -mcpu=$(CPU) \
         -mthumb \
         -mfloat-abi=soft \
         -ffreestanding \
         -fdata-sections \
         -ffunction-sections \
         -Wall \
         -Wextra \
         -g3 \
         -O0

ASFLAGS = -mcpu=$(CPU) \
          -mthumb \
          -g3

LDFLAGS = -T linker/stm32f103c8.ld \
          -mcpu=$(CPU) \
          -mthumb \
          -mfloat-abi=soft \
          -nostdlib \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map

INCLUDES = -Iinc

C_SOURCES = \
    src/main.c \
    src/gpio.c \
    src/rcc.c \
    src/systick.c \
    src/uart.c \
    src/spi.c \
    src/i2c.c \
    src/timer.c \
    src/pwm.c \
    src/adc.c \
    src/nvic.c \
    src/exti.c \
    src/iwdg.c

ASM_SOURCES = \
    startup/startup_stm32f103xb.s

C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

all: $(TARGET).elf $(TARGET).bin
	@echo "Build completed successfully."
	$(SIZE) $(TARGET).elf

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

startup/%.o: startup/%.s
	$(AS) $(ASFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(C_OBJECTS)
	rm -f $(ASM_OBJECTS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
	rm -f $(TARGET).map

.PHONY: all clean
