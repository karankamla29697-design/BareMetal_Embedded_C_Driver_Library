#include "i2c.h"
#include "gpio.h"
#include "rcc.h"

#define I2C_TIMEOUT_COUNT 1000000UL

static uint8_t i2c_status = I2C_OK;

static uint8_t I2C_WaitSR1(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT_COUNT;

    while ((I2C1->SR1 & flag) == 0U)
    {
        if (timeout == 0U)
        {
            i2c_status = I2C_TIMEOUT;
            return 0U;
        }

        timeout--;
    }

    return 1U;
}


static void I2C_ClearADDR(void)
{
    volatile uint32_t dummy;

    dummy = I2C1->SR1;
    dummy = I2C1->SR2;

    (void)dummy;
}

static uint8_t I2C_WaitBusFree(void)
{
    uint32_t timeout = I2C_TIMEOUT_COUNT;

    while ((I2C1->SR2 & I2C_SR2_BUSY) != 0U)
    {
        if (timeout == 0U)
        {
            i2c_status = I2C_TIMEOUT;
            return 0U;
        }

        timeout--;
    }

    return 1U;
}

uint8_t I2C_Init(uint32_t speed_hz)
{
    uint32_t pclk1;
    uint32_t freq_mhz;
    uint32_t ccr;

    if (speed_hz == 0U)
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    /*
     * I2C1:
     *
     * PB6 = SCL
     * PB7 = SDA
     *
     * Both require alternate-function open-drain.
     */
    GPIO_Init(
        GPIO_PORT_B,
        6U,
        GPIO_MODE_OUTPUT_50MHZ,
        GPIO_OUTPUT_AF_OD
    );

    GPIO_Init(
        GPIO_PORT_B,
        7U,
        GPIO_MODE_OUTPUT_50MHZ,
        GPIO_OUTPUT_AF_OD
    );

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1->CR1 = I2C_CR1_SWRST;
    I2C1->CR1 = 0U;

    pclk1 = RCC_GetAPB1Clock();

    if (pclk1 == 0U)
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    /*
     * CR2.FREQ contains APB1 frequency in MHz.
     *
     * STM32F1 I2C peripheral supports APB1 up to 36 MHz.
     */
    freq_mhz = pclk1 / 1000000U;

    if ((freq_mhz == 0U) || (freq_mhz > 36U))
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    I2C1->CR2 = freq_mhz & 0x3FU;

    /*
     * Own address is irrelevant for master-only operation,
     * but OAR1 must contain the required address format.
     */
    I2C1->OAR1 = (1U << 14);

    if (speed_hz <= 100000U)
    {
        /*
         * Standard mode:
         *
         * CCR = PCLK1 / (2 * I2C_speed)
         */
        ccr = pclk1 / (2U * speed_hz);

        if (ccr < 4U)
        {
            ccr = 4U;
        }

        I2C1->CCR = ccr & 0x0FFFU;

        /*
         * TRISE = FREQ(MHz) + 1 for standard mode.
         */
        I2C1->TRISE = freq_mhz + 1U;
    }
    else
    {
        /*
         * Fast mode, duty = 2:
         *
         * CCR = PCLK1 / (3 * I2C_speed)
         */
        ccr = pclk1 / (3U * speed_hz);

        if (ccr < 1U)
        {
            ccr = 1U;
        }

        I2C1->CCR =
            I2C_CCR_FS |
            (ccr & 0x0FFFU);

        /*
         * Fast-mode maximum rise time:
         *
         * TRISE = FREQ(MHz) * 300ns + 1
         */
        I2C1->TRISE = ((freq_mhz * 300U) / 1000U) + 1U;
    }

    I2C1->CR1 = I2C_CR1_PE;

    i2c_status = I2C_OK;

    return I2C_OK;
}

uint8_t I2C_Start(void)
{
    if (I2C_WaitBusFree() == 0U)
    {
        return I2C_TIMEOUT;
    }

    I2C1->CR1 |= I2C_CR1_START;

    if (I2C_WaitSR1(I2C_SR1_SB) == 0U)
    {
        return I2C_TIMEOUT;
    }

    return I2C_OK;
}

uint8_t I2C_Stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;

    i2c_status = I2C_OK;

    return I2C_OK;
}

uint8_t I2C_WriteByte(uint8_t data)
{
    if (I2C_WaitSR1(I2C_SR1_TXE) == 0U)
    {
        return I2C_TIMEOUT;
    }

    I2C1->DR = data;

    if (I2C_WaitSR1(I2C_SR1_BTF) == 0U)
    {
        return I2C_TIMEOUT;
    }

    return I2C_OK;
}

uint8_t I2C_ReadByte(uint8_t *data, uint8_t acknowledge)
{
    if (data == 0)
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    if (acknowledge != 0U)
    {
        I2C1->CR1 |= I2C_CR1_ACK;
    }
    else
    {
        I2C1->CR1 &= ~I2C_CR1_ACK;
    }

    if (I2C_WaitSR1(I2C_SR1_RXNE) == 0U)
    {
        return I2C_TIMEOUT;
    }

    *data = (uint8_t)(I2C1->DR & 0xFFU);

    return I2C_OK;
}

uint8_t I2C_Write(
    uint8_t address,
    const uint8_t *data,
    uint32_t length
)
{
    uint32_t i;
    uint8_t status;

    if ((data == 0) && (length != 0U))
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    status = I2C_Start();

    if (status != I2C_OK)
    {
        return status;
    }

    /*
     * 7-bit address + write bit.
     */
    I2C1->DR = (uint32_t)(address << 1U);

    if (I2C_WaitSR1(I2C_SR1_ADDR) == 0U)
    {
        I2C_Stop();
        return I2C_TIMEOUT;
    }

    I2C_ClearADDR();

    if ((I2C1->SR2 & I2C_SR2_MSL) == 0U)
    {
        I2C_Stop();
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    for (i = 0U; i < length; i++)
    {
        status = I2C_WriteByte(data[i]);

        if (status != I2C_OK)
        {
            I2C_Stop();
            return status;
        }
    }

    I2C_Stop();

    i2c_status = I2C_OK;

    return I2C_OK;
}

uint8_t I2C_Read(
    uint8_t address,
    uint8_t *data,
    uint32_t length
)
{
    uint32_t i;
    uint8_t status;

    if ((data == 0) || (length == 0U))
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    status = I2C_Start();

    if (status != I2C_OK)
    {
        return status;
    }

    /*
     * 7-bit address + read bit.
     */
    I2C1->DR = (uint32_t)((address << 1U) | 1U);

    if (I2C_WaitSR1(I2C_SR1_ADDR) == 0U)
    {
        I2C_Stop();
        return I2C_TIMEOUT;
    }

    /*
     * Single-byte receive requires ACK disabled before
     * clearing ADDR and STOP generation.
     */
    if (length == 1U)
    {
        I2C1->CR1 &= ~I2C_CR1_ACK;

        I2C_ClearADDR();

        I2C1->CR1 |= I2C_CR1_STOP;

        if (I2C_WaitSR1(I2C_SR1_RXNE) == 0U)
        {
            return I2C_TIMEOUT;
        }

        data[0] = (uint8_t)(I2C1->DR & 0xFFU);

        i2c_status = I2C_OK;

        return I2C_OK;
    }

    /*
     * Multi-byte receive.
     */
    I2C1->CR1 |= I2C_CR1_ACK;

    I2C_ClearADDR();

    for (i = 0U; i < length; i++)
    {
        if (i == (length - 1U))
        {
            /*
             * Last byte:
             * NACK + STOP.
             */
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C1->CR1 |= I2C_CR1_STOP;
        }

        if (I2C_WaitSR1(I2C_SR1_RXNE) == 0U)
        {
            return I2C_TIMEOUT;
        }

        data[i] = (uint8_t)(I2C1->DR & 0xFFU);
    }

    i2c_status = I2C_OK;

    return I2C_OK;
}

uint8_t I2C_WriteRegister(
    uint8_t address,
    uint8_t reg,
    const uint8_t *data,
    uint32_t length
)
{
    uint32_t i;
    uint8_t status;

    if ((data == 0) && (length != 0U))
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    status = I2C_Start();

    if (status != I2C_OK)
    {
        return status;
    }

    I2C1->DR = (uint32_t)(address << 1U);

    if (I2C_WaitSR1(I2C_SR1_ADDR) == 0U)
    {
        I2C_Stop();
        return I2C_TIMEOUT;
    }

    I2C_ClearADDR();

    status = I2C_WriteByte(reg);

    if (status != I2C_OK)
    {
        I2C_Stop();
        return status;
    }

    for (i = 0U; i < length; i++)
    {
        status = I2C_WriteByte(data[i]);

        if (status != I2C_OK)
        {
            I2C_Stop();
            return status;
        }
    }

    I2C_Stop();

    i2c_status = I2C_OK;

    return I2C_OK;
}

uint8_t I2C_ReadRegister(
    uint8_t address,
    uint8_t reg,
    uint8_t *data,
    uint32_t length
)
{
    uint8_t status;

    if ((data == 0) || (length == 0U))
    {
        i2c_status = I2C_ERROR;
        return I2C_ERROR;
    }

    /*
     * First transaction:
     * write register address.
     */
    status = I2C_Start();

    if (status != I2C_OK)
    {
        return status;
    }

    I2C1->DR = (uint32_t)(address << 1U);

    if (I2C_WaitSR1(I2C_SR1_ADDR) == 0U)
    {
        I2C_Stop();
        return I2C_TIMEOUT;
    }

    I2C_ClearADDR();

    status = I2C_WriteByte(reg);

    if (status != I2C_OK)
    {
        I2C_Stop();
        return status;
    }

    /*
     * Repeated START.
     */
    I2C1->CR1 |= I2C_CR1_START;

    if (I2C_WaitSR1(I2C_SR1_SB) == 0U)
    {
        I2C_Stop();
        return I2C_TIMEOUT;
    }

    I2C1->DR = (uint32_t)((address << 1U) | 1U);

    if (I2C_WaitSR1(I2C_SR1_ADDR) == 0U)
    {
        I2C_Stop();
        return I2C_TIMEOUT;
    }

    /*
     * Reuse the normal receive sequence after the
     * repeated START.
     */
    if (length == 1U)
    {
        I2C1->CR1 &= ~I2C_CR1_ACK;

        I2C_ClearADDR();

        I2C1->CR1 |= I2C_CR1_STOP;

        if (I2C_WaitSR1(I2C_SR1_RXNE) == 0U)
        {
            return I2C_TIMEOUT;
        }

        data[0] = (uint8_t)(I2C1->DR & 0xFFU);
    }
    else
    {
        uint32_t i;

        I2C1->CR1 |= I2C_CR1_ACK;

        I2C_ClearADDR();

        for (i = 0U; i < length; i++)
        {
            if (i == (length - 1U))
            {
                I2C1->CR1 &= ~I2C_CR1_ACK;
                I2C1->CR1 |= I2C_CR1_STOP;
            }

            if (I2C_WaitSR1(I2C_SR1_RXNE) == 0U)
            {
                return I2C_TIMEOUT;
            }

            data[i] = (uint8_t)(I2C1->DR & 0xFFU);
        }
    }

    i2c_status = I2C_OK;

    return I2C_OK;
}

uint8_t I2C_GetStatus(void)
{
    return i2c_status;
}
