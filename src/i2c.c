#include "i2c.h"

void i2cInit(bool enablePullup)
{
    if (enablePullup)
    {
        I2C_DDR &= ~(_BV(I2C_SDA) | _BV(I2C_SCL));
        I2C_PORT |= _BV(I2C_SDA) | _BV(I2C_SCL);
    }

    TWCR = _BV(TWEN);
    TWBR = ((F_CPU / I2C_F_SCL) - 16) / 2;
}

void i2cWait()
{
    while (!(TWCR & _BV(TWINT)))
        ;
}

int8_t i2cStart()
{
    TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTA);
    i2cWait();
    return (TWSR & TW_STATUS_MASK) != TW_START;
}

int8_t i2cStop()
{
    TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
    while (TWCR & _BV(TWSTO))
        ;
    return 0;
}

int8_t i2cRestart()
{
    TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTA);
    i2cWait();
    return (TWSR & TW_STATUS_MASK) != TW_REP_START;
}

int8_t i2cSetAddr(uint8_t addr)
{
    TWDR = addr;
    TWCR = _BV(TWEN) | _BV(TWINT);
    i2cWait();
    return (TWSR & TW_STATUS_MASK) != ((addr & 1) ? TW_MR_SLA_ACK : TW_MT_SLA_ACK);
}

int8_t i2cSend(uint8_t data)
{
    TWDR = data;
    TWCR = _BV(TWEN) | _BV(TWINT);
    i2cWait();
    return (TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK;
}

int8_t i2cWrite(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (i2cSend(data[i]))
        {
            return 1;
        }
    }

    return 0;
}