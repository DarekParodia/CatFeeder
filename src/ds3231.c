#include "ds3231.h"

static int8_t ds3231_read_eeprom(uint8_t addr, uint8_t *data, size_t len)
{
    return i2cStart() || i2cSetAddr(DS3231_I2C_SLAW_WRITE) || i2cSend(addr) || i2cRestart() || i2cSetAddr(DS3231_I2C_SLAW_READ) || i2cRead(data, len) || i2cStop();
}

int8_t ds3231_read_clock(struct ds3231_clock_t *clock)
{
    struct ds3231_raw_clock_t raw;

    if (ds3231_read_eeprom(DS3231_EEPROM_CLOCK, (uint8_t *)&raw, sizeof(raw)))
    {
        return 1;
    }

    clock->seconds = raw.seconds10 * 10 + raw.seconds;
    clock->minutes = raw.minutes10 * 10 + raw.minutes;
    clock->hours = raw.hours10 * 10 + raw.hours;
    clock->day = raw.hours10 * 10 + raw.day;
    clock->date = raw.date10 * 10 + raw.date;
    clock->century = raw.century;
    clock->month = raw.month10 * 10 + raw.month;
    clock->year = raw.year10 * 10 + raw.year;

    return 0;
}

uint32_t ds3231_clock_to_unixtime(struct ds3231_clock_t *clock)
{
    uint32_t unixtime = 0;

    int daysSinceEpoch = (clock->year - 1970) * 365 + ((clock->year - 1969) / 4) - ((clock->year - 2000) / 100) + ((clock->year - 1900) / 400) - 719162;

    // include leap years
    if (clock->month > 2 && (clock->year % 4 == 0 && (clock->year % 100 != 0 || clock->year % 400 == 0)))
    {
        daysSinceEpoch++;
    }

    // include days of the month
    daysSinceEpoch += (clock->month > 1 ? clock->month : 12) - 1;

    daysSinceEpoch += clock->day - 1;

    unixtime = daysSinceEpoch * 86400 + clock->hours * 3600 + clock->minutes * 60 + clock->seconds;

    return unixtime;
}