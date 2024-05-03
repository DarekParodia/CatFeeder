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
#define EEPROM_ADR_CLOCK 0

extern volatile unsigned long milis_counter;
extern volatile unsigned long micros_counter;
extern unsigned long last_reset_time_millis;
extern unsigned long last_reset_time_micros;

void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode);
void setPin(volatile uint8_t *port, uint8_t pin, uint8_t value);
void togglePin(volatile uint8_t *port, uint8_t pin);
void delay(volatile uint16_t ms);
void delayMicroseconds(volatile uint16_t us);
char *int2str(int value);
char *long2str(long value);
char *float2str(float value);
void saveClock(struct ds3231_clock_t *clock);
void loadClock(struct ds3231_clock_t *clock);

// Timer stuff
void timers_init();
unsigned long millis();
unsigned long micros();

#endif // GENERAL_H
