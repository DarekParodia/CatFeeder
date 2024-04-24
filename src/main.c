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
      // PORTB ^= (1 << PORTB5);
      setPin(&PORTB, PORTB5, ledState);
      ledState = !ledState;

      for (volatile uint16_t i = 0; i < 60000; i++)
      {
         // do nothing
      }
   }
}
