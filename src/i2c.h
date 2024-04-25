#ifndef I2C_H
#define I2C_H

#define I2C_F_SCL 100000
#define I2C_DDR DDRC
#define I2C_PORT PORTC
#define I2C_SDA PC4
#define I2C_SCL PC5

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>
#include <util/twi.h>
#include <stdbool.h>

#include "general.h"

void i2cInit(bool enablePullup);
void i2cWait();
int8_t i2cStart();
int8_t i2cStop();
int8_t i2cRestart();
int8_t i2cSetAddr(uint8_t addr);
int8_t i2cSend(uint8_t data);
int8_t i2cWrite(const uint8_t *data, size_t len);
int8_t i2cRead(uint8_t *data, size_t len);

#endif // I2C_H