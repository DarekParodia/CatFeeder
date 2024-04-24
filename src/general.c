#include <avr/io.h>
#include <stdint.h>
#include "general.h"

void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode)
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

void setPin(volatile uint8_t *port, uint8_t pin, uint8_t value)
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

void togglePin(volatile uint8_t *port, uint8_t pin)
{
    *port ^= (1 << pin);
}

void delay(volatile uint16_t ms)
{
    // Implement delay logic here
}