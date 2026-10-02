.syntax unified
.cpu cortex-m3
.thumb

/* =========================================================
 * External Symbols
 * ========================================================= */

.extern main

.extern _sidata
.extern _sdata
.extern _edata

.extern _sbss
.extern _ebss
.extern EXTI0_IRQHandler
.extern EXTI1_IRQHandler
.extern EXTI2_IRQHandler
.extern EXTI3_IRQHandler
.extern EXTI4_IRQHandler
.extern EXTI9_5_IRQHandler
.extern EXTI15_10_IRQHandler

/* =========================================================
 * Vector Table
 * ========================================================= */

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler
    .word Default_Handler
    .word 0
    .word Default_Handler
    .word Default_Handler

    /* External interrupt vectors */
    .word Default_Handler        /* IRQ0  WWDG */
    .word Default_Handler        /* IRQ1  PVD */
    .word Default_Handler        /* IRQ2  TAMPER */
    .word Default_Handler        /* IRQ3  RTC */
    .word Default_Handler        /* IRQ4  FLASH */
    .word Default_Handler        /* IRQ5  RCC */
    .word EXTI0_IRQHandler       /* IRQ6  EXTI0 */
    .word EXTI1_IRQHandler       /* IRQ7  EXTI1 */
    .word EXTI2_IRQHandler       /* IRQ8  EXTI2 */
    .word EXTI3_IRQHandler       /* IRQ9  EXTI3 */
    .word EXTI4_IRQHandler       /* IRQ10 EXTI4 */
    .word Default_Handler        /* IRQ11 DMA1 Channel1 */
    .word Default_Handler        /* IRQ12 DMA1 Channel2 */
    .word Default_Handler        /* IRQ13 DMA1 Channel3 */
    .word Default_Handler        /* IRQ14 DMA1 Channel4 */
    .word Default_Handler        /* IRQ15 DMA1 Channel5 */
    .word Default_Handler        /* IRQ16 DMA1 Channel6 */
    .word Default_Handler        /* IRQ17 DMA1 Channel7 */
    .word Default_Handler        /* IRQ18 ADC1_2 */
    .word Default_Handler        /* IRQ19 USB_HP_CAN_TX */
    .word Default_Handler        /* IRQ20 USB_LP_CAN_RX0 */
    .word Default_Handler        /* IRQ21 CAN_RX1 */
    .word Default_Handler        /* IRQ22 CAN_SCE */
    .word EXTI9_5_IRQHandler     /* IRQ23 EXTI9_5 */
    .word Default_Handler        /* IRQ24 TIM1_BRK */
    .word Default_Handler        /* IRQ25 TIM1_UP */
    .word Default_Handler        /* IRQ26 TIM1_TRG_COM */
    .word Default_Handler        /* IRQ27 TIM1_CC */
    .word Default_Handler        /* IRQ28 TIM2 */
    .word Default_Handler        /* IRQ29 TIM3 */
    .word Default_Handler        /* IRQ30 TIM4 */
    .word Default_Handler        /* IRQ31 I2C1_EV */
    .word Default_Handler        /* IRQ32 I2C1_ER */
    .word Default_Handler        /* IRQ33 I2C2_EV */
    .word Default_Handler        /* IRQ34 I2C2_ER */
    .word Default_Handler        /* IRQ35 SPI1 */
    .word Default_Handler        /* IRQ36 SPI2 */
    .word Default_Handler        /* IRQ37 USART1 */
    .word Default_Handler        /* IRQ38 USART2 */
    .word Default_Handler        /* IRQ39 USART3 */
    .word EXTI15_10_IRQHandler   /* IRQ40 EXTI15_10 */
.size g_pfnVectors, . - g_pfnVectors

/* =========================================================
 * Reset Handler
 * ========================================================= */

.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    /* -----------------------------------------------------
     * 1. Copy .data section from Flash to RAM
     *
     * _sidata = source address in Flash
     * _sdata  = destination start in RAM
     * _edata  = destination end in RAM
     * ----------------------------------------------------- */

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bge zero_bss

    ldr r3, [r0]
    str r3, [r1]

    adds r0, r0, #4
    adds r1, r1, #4

    b copy_data

    /* -----------------------------------------------------
     * 2. Clear .bss section
     *
     * _sbss = start of BSS
     * _ebss = end of BSS
     * ----------------------------------------------------- */

zero_bss:

    ldr r1, =_sbss
    ldr r2, =_ebss

    movs r0, #0

clear_bss:
    cmp r1, r2
    bge call_main

    str r0, [r1]

    adds r1, r1, #4

    b clear_bss

    /* -----------------------------------------------------
     * 3. Call main()
     * ----------------------------------------------------- */

call_main:

    bl main

    /* -----------------------------------------------------
     * main() should never return.
     * ----------------------------------------------------- */

infinite_loop:

    b infinite_loop

.size Reset_Handler, . - Reset_Handler

/* =========================================================
 * Default Interrupt Handler
 * ========================================================= */

.section .text.Default_Handler
.weak Default_Handler
.type Default_Handler, %function

Default_Handler:

default_loop:

    b default_loop

.size Default_Handler, . - Default_Handler
