#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "general.h"

int main(void)
{
   setPinMode(&DDRB, DDB5, OUTPUT);
   bool ledState = LOW;
   while (1)
   {
      setPin(&PORTB, PORTB5, ledState);
      ledState = !ledState;
      delay(1000);
   }
}
