#ifndef STM32F103XX_H
#define STM32F103XX_H

#include <stdint.h>

/* =========================================================
 * STM32F103 Memory Map
 * ========================================================= */

#define PERIPH_BASE          0x40000000UL

#define APB1PERIPH_BASE      (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE      (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE       (PERIPH_BASE + 0x00020000UL)

/* =========================================================
 * RCC - Reset and Clock Control
 * ========================================================= */

#define RCC_BASE             (AHBPERIPH_BASE + 0x00001000UL)

/* =========================================================
 * RCC Register Structure
 * ========================================================= */

typedef struct
{
    volatile uint32_t CR;          /* 0x00 - Clock control */
    volatile uint32_t CFGR;        /* 0x04 - Clock configuration */
    volatile uint32_t CIR;         /* 0x08 - Clock interrupt */
    volatile uint32_t APB2RSTR;    /* 0x0C - APB2 peripheral reset */
    volatile uint32_t APB1RSTR;    /* 0x10 - APB1 peripheral reset */
    volatile uint32_t AHBENR;      /* 0x14 - AHB peripheral clock enable */
    volatile uint32_t APB2ENR;     /* 0x18 - APB2 peripheral clock enable */
    volatile uint32_t APB1ENR;     /* 0x1C - APB1 peripheral clock enable */
    volatile uint32_t BDCR;        /* 0x20 - Backup domain control */
    volatile uint32_t CSR;         /* 0x24 - Control/status */
} RCC_TypeDef;

/* =========================================================
 * RCC Peripheral Instance
 * ========================================================= */

#define RCC                  ((RCC_TypeDef *)RCC_BASE)

/* =========================================================
 * RCC Clock Control Register - CR
 * ========================================================= */

#define RCC_CR_HSION         (1U << 0)
#define RCC_CR_HSIRDY        (1U << 1)

#define RCC_CR_HSITRIM_MASK  (0x1FU << 3)

#define RCC_CR_HSEON         (1U << 16)
#define RCC_CR_HSERDY        (1U << 17)
#define RCC_CR_HSEBYP        (1U << 18)

#define RCC_CR_CSSON         (1U << 19)

#define RCC_CR_PLLON         (1U << 24)
#define RCC_CR_PLLRDY        (1U << 25)

/* =========================================================
 * RCC Clock Configuration Register - CFGR
 * ========================================================= */

/* System clock switch */

#define RCC_CFGR_SW_MASK     (0x03U << 0)
#define RCC_CFGR_SW_HSI      (0x00U << 0)
#define RCC_CFGR_SW_HSE      (0x01U << 0)
#define RCC_CFGR_SW_PLL      (0x02U << 0)

/* System clock status */

#define RCC_CFGR_SWS_MASK    (0x03U << 2)
#define RCC_CFGR_SWS_HSI     (0x00U << 2)
#define RCC_CFGR_SWS_HSE     (0x01U << 2)
#define RCC_CFGR_SWS_PLL     (0x02U << 2)

/* AHB prescaler */

#define RCC_CFGR_HPRE_MASK   (0x0FU << 4)

/* APB1 prescaler */

#define RCC_CFGR_PPRE1_MASK  (0x07U << 8)

/* APB2 prescaler */

#define RCC_CFGR_PPRE2_MASK  (0x07U << 11)

/* PLL source */

#define RCC_CFGR_PLLSRC      (1U << 16)
#define RCC_CFGR_PLLXTPRE       (1U << 17)

#define RCC_CFGR_ADCPRE         (3U << 14)
#define RCC_CFGR_ADCPRE_DIV2    (0U << 14)
#define RCC_CFGR_ADCPRE_DIV4    (1U << 14)
#define RCC_CFGR_ADCPRE_DIV6    (2U << 14)
#define RCC_CFGR_ADCPRE_DIV8    (3U << 14)

/* PLL multiplication */

#define RCC_CFGR_PLLMUL_MASK (0x0FU << 18)

/* =========================================================
 * RCC Peripheral Clock Enable Bits
 * ========================================================= */

/* AHB */

#define RCC_AHBENR_DMA1EN    (1U << 0)
#define RCC_AHBENR_SRAMEN    (1U << 2)
#define RCC_AHBENR_FLITFEN   (1U << 4)
#define RCC_AHBENR_CRCEN     (1U << 6)

/* APB2 */

#define RCC_APB2ENR_AFIOEN   (1U << 0)
#define RCC_APB2ENR_SPI1EN  (1U << 12)
#define RCC_APB2ENR_IOPAEN   (1U << 2)
#define RCC_APB2ENR_IOPBEN   (1U << 3)
#define RCC_APB2ENR_IOPCEN   (1U << 4)
#define RCC_APB2ENR_IOPDEN   (1U << 5)
#define RCC_APB2ENR_IOPEEN   (1U << 6)
#define RCC_APB2ENR_ADC1EN   (1U << 9)
#define RCC_APB2ENR_ADC2EN   (1U << 10)
#define RCC_APB2ENR_USART1EN (1U << 14)

/* APB1 */

#define RCC_APB1ENR_TIM2EN   (1U << 0)
#define RCC_APB1ENR_TIM3EN   (1U << 1)
#define RCC_APB1ENR_TIM4EN   (1U << 2)
#define RCC_APB1ENR_SPI2EN   (1U << 14)
#define RCC_APB1ENR_USART2EN (1U << 17)
#define RCC_APB1ENR_USART3EN (1U << 18)
#define RCC_APB1ENR_I2C1EN   (1U << 21)
#define RCC_APB1ENR_I2C2EN   (1U << 22)

/* =========================================================
 * GPIO Base Addresses
 * ========================================================= */

#define GPIOA_BASE           (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE           (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE           (APB2PERIPH_BASE + 0x1000UL)

/* =========================================================
 * GPIO Register Structure
 * ========================================================= */

typedef struct
{
    volatile uint32_t CRL;      /* 0x00 - Port configuration low */
    volatile uint32_t CRH;      /* 0x04 - Port configuration high */
    volatile uint32_t IDR;      /* 0x08 - Input data register */
    volatile uint32_t ODR;      /* 0x0C - Output data register */
    volatile uint32_t BSRR;     /* 0x10 - Bit set/reset register */
    volatile uint32_t BRR;      /* 0x14 - Bit reset register */
    volatile uint32_t LCKR;     /* 0x18 - Configuration lock */
} GPIO_TypeDef;

/* =========================================================
 * GPIO Peripheral Instances
 * ========================================================= */

#define GPIOA                 ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *)GPIOC_BASE)

/* =========================================================
 * GPIO Pin Definitions
 * ========================================================= */

#define GPIO_PIN_0            (1U << 0)
#define GPIO_PIN_1            (1U << 1)
#define GPIO_PIN_2            (1U << 2)
#define GPIO_PIN_3            (1U << 3)
#define GPIO_PIN_4            (1U << 4)
#define GPIO_PIN_5            (1U << 5)
#define GPIO_PIN_6            (1U << 6)
#define GPIO_PIN_7            (1U << 7)
#define GPIO_PIN_8            (1U << 8)
#define GPIO_PIN_9            (1U << 9)
#define GPIO_PIN_10           (1U << 10)
#define GPIO_PIN_11           (1U << 11)
#define GPIO_PIN_12           (1U << 12)
#define GPIO_PIN_13           (1U << 13)
#define GPIO_PIN_14           (1U << 14)
#define GPIO_PIN_15           (1U << 15)


/* =========================================================
 * Cortex-M3 SysTick
 * ========================================================= */

#define SYSTICK_BASE          0xE000E010UL

typedef struct
{
    volatile uint32_t CTRL;       /* 0x00 - Control and status */
    volatile uint32_t LOAD;       /* 0x04 - Reload value */
    volatile uint32_t VAL;        /* 0x08 - Current value */
    volatile uint32_t CALIB;      /* 0x0C - Calibration */
} SysTick_TypeDef;

#define SysTick               ((SysTick_TypeDef *)SYSTICK_BASE)

/* SysTick CTRL bits */

#define SYSTICK_CTRL_ENABLE    (1U << 0)
#define SYSTICK_CTRL_TICKINT   (1U << 1)
#define SYSTICK_CTRL_CLKSOURCE (1U << 2)
#define SYSTICK_CTRL_COUNTFLAG (1U << 16)


/* =========================================================
 * USART1
 * ========================================================= */

#define USART1_BASE           (APB2PERIPH_BASE + 0x380UL)

typedef struct
{
    volatile uint32_t SR;         /* 0x00 - Status */
    volatile uint32_t DR;         /* 0x04 - Data */
    volatile uint32_t BRR;        /* 0x08 - Baud rate */
    volatile uint32_t CR1;        /* 0x0C - Control 1 */
    volatile uint32_t CR2;        /* 0x10 - Control 2 */
    volatile uint32_t CR3;        /* 0x14 - Control 3 */
    volatile uint32_t GTPR;       /* 0x18 - Guard time/prescaler */
} USART_TypeDef;

#define USART1                ((USART_TypeDef *)USART1_BASE)

/* =========================================================
 * USART Status Register Bits
 * ========================================================= */

#define USART_SR_PE           (1U << 0)
#define USART_SR_FE           (1U << 1)
#define USART_SR_NE           (1U << 2)
#define USART_SR_ORE          (1U << 3)
#define USART_SR_IDLE         (1U << 4)
#define USART_SR_RXNE         (1U << 5)
#define USART_SR_TC           (1U << 6)
#define USART_SR_TXE          (1U << 7)

/* =========================================================
 * USART Control Register 1 Bits
 * ========================================================= */

#define USART_CR1_RE          (1U << 2)
#define USART_CR1_TE          (1U << 3)
#define USART_CR1_RXNEIE      (1U << 5)
#define USART_CR1_TCIE        (1U << 6)
#define USART_CR1_TXEIE       (1U << 7)
#define USART_CR1_UE          (1U << 13)

/* =========================================================
 * USART Control Register 2 Bits
 * ========================================================= */

#define USART_CR2_STOP_MASK   (3U << 12)


/* =========================================================
 * SPI1
 * ========================================================= */

#define SPI1_BASE             (APB2PERIPH_BASE + 0x300UL)

typedef struct
{
    volatile uint32_t CR1;        /* 0x00 - Control 1 */
    volatile uint32_t CR2;        /* 0x04 - Control 2 */
    volatile uint32_t SR;         /* 0x08 - Status */
    volatile uint32_t DR;         /* 0x0C - Data */
    volatile uint32_t CRCPR;      /* 0x10 - CRC polynomial */
    volatile uint32_t RXCRCR;     /* 0x14 - RX CRC */
    volatile uint32_t TXCRCR;     /* 0x18 - TX CRC */
    volatile uint32_t I2SCFGR;    /* 0x1C - I2S configuration */
    volatile uint32_t I2SPR;      /* 0x20 - I2S prescaler */
} SPI_TypeDef;

#define SPI1                  ((SPI_TypeDef *)SPI1_BASE)

/* =========================================================
 * SPI CR1 Bits
 * ========================================================= */

#define SPI_CR1_CPHA          (1U << 0)
#define SPI_CR1_CPOL          (1U << 1)
#define SPI_CR1_MSTR          (1U << 2)

#define SPI_CR1_BR_MASK       (7U << 3)

#define SPI_CR1_SPE           (1U << 6)
#define SPI_CR1_LSBFIRST      (1U << 7)
#define SPI_CR1_SSI           (1U << 8)
#define SPI_CR1_SSM           (1U << 9)

/* =========================================================
 * SPI Status Register Bits
 * ========================================================= */

#define SPI_SR_RXNE           (1U << 0)
#define SPI_SR_TXE            (1U << 1)
#define SPI_SR_BSY            (1U << 7)


/* =========================================================
 * I2C1
 * ========================================================= */

#define I2C1_BASE             (APB1PERIPH_BASE + 0x540UL)

typedef struct
{
    volatile uint32_t CR1;        /* 0x00 */
    volatile uint32_t CR2;        /* 0x04 */
    volatile uint32_t OAR1;       /* 0x08 */
    volatile uint32_t OAR2;       /* 0x0C */
    volatile uint32_t DR;         /* 0x10 */
    volatile uint32_t SR1;        /* 0x14 */
    volatile uint32_t SR2;        /* 0x18 */
    volatile uint32_t CCR;        /* 0x1C */
    volatile uint32_t TRISE;      /* 0x20 */
} I2C_TypeDef;

#define I2C1                  ((I2C_TypeDef *)I2C1_BASE)

/* CR1 */
#define I2C_CR1_PE            (1U << 0)
#define I2C_CR1_START         (1U << 8)
#define I2C_CR1_STOP          (1U << 9)
#define I2C_CR1_SWRST       (1U << 15)
#define I2C_CR1_ACK           (1U << 10)

/* SR1 */
#define I2C_SR1_SB            (1U << 0)
#define I2C_SR1_ADDR          (1U << 1)
#define I2C_SR1_BTF           (1U << 2)
#define I2C_SR1_RXNE          (1U << 6)
#define I2C_SR1_TXE           (1U << 7)

/* SR2 */
#define I2C_SR2_MSL           (1U << 0)
#define I2C_SR2_BUSY          (1U << 1)
#define I2C_SR2_TRA           (1U << 2)

/* CCR */
#define I2C_CCR_FS            (1U << 15)
#define I2C_CCR_DUTY          (1U << 14)


/* =========================================================
 * TIM2
 * ========================================================= */

#define TIM2_BASE             (APB1PERIPH_BASE + 0x000UL)

typedef struct
{
    volatile uint32_t CR1;        /* 0x00 */
    volatile uint32_t CR2;        /* 0x04 */
    volatile uint32_t SMCR;       /* 0x08 */
    volatile uint32_t DIER;       /* 0x0C */
    volatile uint32_t SR;         /* 0x10 */
    volatile uint32_t EGR;        /* 0x14 */
    volatile uint32_t CCMR1;      /* 0x18 */
    volatile uint32_t CCMR2;      /* 0x1C */
    volatile uint32_t CCER;       /* 0x20 */
    volatile uint32_t CNT;        /* 0x24 */
    volatile uint32_t PSC;        /* 0x28 */
    volatile uint32_t ARR;        /* 0x2C */
    volatile uint32_t RESERVED1;  /* 0x30 */
    volatile uint32_t CCR1;       /* 0x34 */
    volatile uint32_t CCR2;       /* 0x38 */
    volatile uint32_t CCR3;       /* 0x3C */
    volatile uint32_t CCR4;       /* 0x40 */
    volatile uint32_t RESERVED2;  /* 0x44 */
    volatile uint32_t DCR;        /* 0x48 */
    volatile uint32_t DMAR;       /* 0x4C */

} TIM_TypeDef;

#define TIM2                  ((TIM_TypeDef *)TIM2_BASE)

/* TIM2 CR1 */

#define TIM_CR1_CEN           (1U << 0)
#define TIM_CR1_UDIS          (1U << 1)
#define TIM_CR1_URS           (1U << 2)
#define TIM_CR1_OPM           (1U << 3)
#define TIM_CR1_DIR           (1U << 4)
#define TIM_CR1_ARPE          (1U << 7)

/* TIM2 DIER */

#define TIM_DIER_UIE          (1U << 0)

/* TIM2 SR */

#define TIM_SR_UIF            (1U << 0)

/* TIM2 EGR */

#define TIM_EGR_UG            (1U << 0)

/* TIM2 APB1 clock enable */

#define RCC_APB1ENR_TIM2EN    (1U << 0)


/* =========================================================
 * TIM2 Channel Output
 * ========================================================= */

/*
 * TIM2 CCMR1 - Channel 1 output compare / PWM
 *
 * CC1S  = bits 1:0
 * OC1PE = bit 3
 * OC1M  = bits 6:4
 *
 * PWM mode 1 = OC1M = 110
 */
#define TIM_CCMR1_CC1S       (3U << 0)
#define TIM_CCMR1_OC1PE      (1U << 3)
#define TIM_CCMR1_OC1M       (7U << 4)
#define TIM_CCMR1_OC1M_1     (1U << 5)
#define TIM_CCMR1_OC1M_2     (1U << 6)

#define TIM_CCER_CC1E         (1U << 0)
#define TIM_CCER_CC1P         (1U << 1)


/* =========================================================
 * ADC1
 * ========================================================= */

#define ADC1_BASE             (APB2PERIPH_BASE + 0x240UL)

#define ADC_SR_EOC              (1U << 1)

#define ADC_CR2_ADON            (1U << 0)
#define ADC_CR2_CONT            (1U << 1)
#define ADC_CR2_CAL             (1U << 2)
#define ADC_CR2_RSTCAL          (1U << 3)
#define ADC_CR2_EXTTRIG         (1U << 20)
#define ADC_CR2_SWSTART         (1U << 22)

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_TypeDef;

#define ADC1                  ((ADC_TypeDef *)ADC1_BASE)

#define RCC_APB2ENR_ADC1EN    (1U << 9)




/* ========================= IWDG ========================= */

#define IWDG_BASE 0x40003000UL

typedef struct
{
    volatile uint32_t KR;
    volatile uint32_t PR;
    volatile uint32_t RLR;
    volatile uint32_t SR;
} IWDG_TypeDef;

#define IWDG ((IWDG_TypeDef *)IWDG_BASE)

/* IWDG Key Register */
#define IWDG_KR_ENABLE        0xCCCCU
#define IWDG_KR_WRITE_ACCESS  0x5555U
#define IWDG_KR_RELOAD        0xAAAAU

/* IWDG Prescaler Register */
#define IWDG_PR_PR_MASK       0x07U

/* IWDG Status Register */
#define IWDG_SR_PVU           (1U << 0)
#define IWDG_SR_RVU           (1U << 1)


/* =========================================================
 * AFIO / EXTI Register Definitions
 * ========================================================= */

#ifndef AFIO_BASE
#define AFIO_BASE 0x40010000UL
#endif

#ifndef EXTI_BASE
#define EXTI_BASE 0x40010400UL
#endif

#ifndef AFIO_TypeDef
typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    volatile uint32_t MAPR2;
} AFIO_TypeDef;
#endif

#ifndef EXTI_TypeDef
typedef struct
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;
#endif

#ifndef AFIO
#define AFIO ((AFIO_TypeDef *)AFIO_BASE)
#endif

#ifndef EXTI
#define EXTI ((EXTI_TypeDef *)EXTI_BASE)
#endif

#endif /* STM32F103XX_H */
