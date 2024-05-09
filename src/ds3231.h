// code stolen from here: https://github.com/ecnx/avr-ds3231

#ifndef DS3231_H
#define DS3231_H

#define DS3231_I2C_SLAW_WRITE 0xD0
#define DS3231_I2C_SLAW_READ 0xD1
#define DS3231_EEPROM_CLOCK 0x00

#include <stdint.h>
#include "i2c.h"

struct ds3231_raw_clock_t
{
    /* Clock Seconds - 00h */
    uint8_t seconds : 4;
    uint8_t seconds10 : 3;
    uint8_t unused1 : 1;

    /* Clock Minutes - 01h */
    uint8_t minutes : 4;
    uint8_t minutes10 : 3;
    uint8_t unused2 : 1;

    /* Clock Hours - 02h */
    uint8_t hours : 4;
    uint8_t hours10 : 2;
    uint8_t unused3 : 2;

    /* Clock Day - 03h */
    uint8_t day : 3;
    uint8_t unused4 : 5;

    /* Clock Date - 04h */
    uint8_t date : 4;
    uint8_t date10 : 2;
    uint8_t unused5 : 2;

    /* Clock Month - 05h */
    uint8_t month : 4;
    uint8_t month10 : 1;
    uint8_t unused6 : 2;
    uint8_t century : 1;

    /* Clock Year - 06h */
    uint8_t year : 4;
    uint8_t year10 : 4;
} __attribute__((packed));

struct ds3231_clock_t
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t century;
    uint8_t month;
    uint8_t year;
};

int8_t ds3231_read_clock(struct ds3231_clock_t *clock);
int8_t ds3231_write_clock(struct ds3231_clock_t *clock);
uint32_t ds3231_clock_to_unixtime(struct ds3231_clock_t *clock);

#endif // DS3231_H