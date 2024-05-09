#ifndef GENERAL_H
#define GENERAL_H

#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL // 16 MHz
#define clockCyclesToMicroseconds(a) ((a) / (F_CPU / 1000000L))
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "ds3231.h"

#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0

// EEPROM addresses offsets
#define EEPROM_ADR_CLOCK 23
#define EEPROM_ADR_LAST_FEED_INDEX EEPROM_ADR_CLOCK + sizeof(struct ds3231_clock_t)

#define FEED_TIME_ROT_SPEED 0.075f
#define FREE_FEED_ROT_SPEED 0.075f
#define DEF_DIRERCTION 0

struct feed_time_t // time at which the feeder will rotate
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t rotation;
    uint16_t rotation_reversed; // used to get the food back from the cat
    float speed;
    float back_speed;
};
typedef struct feed_time_t feed_time_t;

struct free_feed_t // how much cat can get food on its own per day
{
    uint8_t max_delays;
    uint16_t delay_in_seconds;
    uint16_t max_rotation;
    uint16_t rotation_reversed;
    float speed;
    float back_speed;
};
typedef struct free_feed_t free_feed_t;

struct pin_t
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    uint8_t pin;
};
typedef struct pin_t pin_t;

void setPinMode(pin_t *pin, uint8_t mode);
void setPin(pin_t *pin, uint8_t value);
void togglePin(pin_t *pin);

void delay(volatile uint16_t ms);
void delayMicroseconds(volatile uint16_t us);
char *int2str(int value);
char *long2str(long value);
char *float2str(float value);
void saveClock(struct ds3231_clock_t *clock);
void loadClock(struct ds3231_clock_t *clock);
void saveLastFeedIndex(uint8_t index);
uint8_t loadLastFeedIndex();

// Timer stuff
void timers_init();
unsigned long millis();
unsigned long micros();

#endif // GENERAL_H
