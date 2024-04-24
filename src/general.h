#ifndef GENERAL_H
#define GENERAL_H

#include <avr/io.h>
#include <stdint.h>

#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0

void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode);
void setPin(volatile uint8_t *port, uint8_t pin, uint8_t value);
void togglePin(volatile uint8_t *port, uint8_t pin);
void delay(volatile uint16_t ms);

#endif // GENERAL_H