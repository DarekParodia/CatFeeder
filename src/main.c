#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "general.h"
#include "uart.h"
#include "ds3231.h"

bool ledState = LOW;
struct ds3231_clock_t clock = {0};

int main(void)
{
   uartInit(9600);
   setPinMode(&DDRB, DDB5, OUTPUT);
   i2cInit(true);

   uartSendStr("Initialized!\r\n");
   while (1)
   {
      // clear time
      ds3231_read_clock(&clock);
      uartSendStr("Time: ");
      uartSendStr(int2str(clock.hours));
      uartSendStr(":");
      uartSendStr(int2str(clock.minutes));
      uartSendStr(":");
      uartSendStr(int2str(clock.seconds));
      uartSendStr("\r\n");

      setPin(&PORTB, PORTB5, ledState);
      ledState = !ledState;
      delay(1000);
   }
}
