#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "general.h"
#include "uart.h"
#include "ds3231.h"
#include "stepper.h"

struct ds3231_clock_t clock = {0};
struct stepper_t stepper = {0};

void initialize()
{
   uartInit(9600);
   i2cInit(true);

   // init stepper at pin 7 and enable at pin 6
   setPinMode(&DDRD, DDD7, OUTPUT);
   setPinMode(&DDRD, DDD6, OUTPUT);
   initStepper(&stepper, &PORTD, PD7, &PORTD, PD6);

   timers_init(); // for millis and micros

   uartSendStr("Initialized!\r\n");
}

void loop()
{
   ds3231_read_clock(&clock);
   uartSendStr("Time: ");
   uartSendStr(int2str(clock.hours));
   uartSendStr(":");
   uartSendStr(int2str(clock.minutes));
   uartSendStr(":");
   uartSendStr(int2str(clock.seconds));
   uartSendStr("\r\n");

   rotateByDegrees(&stepper, 360);
   delay(1000);
}

int main(void)
{
   initialize();
   while (1)
   {
      loop();
   }
}
