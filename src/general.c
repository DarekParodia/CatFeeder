#include "general.h"

inline void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        *ddr |= (1 << pin);
    }
    else
    {
        *ddr &= ~(1 << pin);
    }
}

inline void setPin(volatile uint8_t *port, uint8_t pin, uint8_t value)
{
    if (value == HIGH)
    {
        *port |= (1 << pin);
    }
    else
    {
        *port &= ~(1 << pin);
    }
}

inline void togglePin(volatile uint8_t *port, uint8_t pin)
{
    *port ^= (1 << pin);
}

inline void delay(volatile uint16_t ms)
{
    _delay_ms(ms);
}

inline void delayUs(volatile uint16_t us)
{
    _delay_us(us);
}

char *int2str(int value)
{
    static char str[12];
    snprintf(str, 12, "%d", value);
    return str;
}