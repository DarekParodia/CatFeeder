#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "general.h"
#include "uart.h"
#include "ds3231.h"
#include "stepper.h"

struct ds3231_clock_t clock = {0};
struct ds3231_clock_t last_saved_clock = {0};
struct stepper_t stepper = {0};

struct feed_time_t feed_times[4] = {
    {0, 0, 0, 380, 360, FEED_TIME_ROT_SPEED, FEED_TIME_ROT_SPEED / 2},
    {11, 30, 0, 380, 360, FEED_TIME_ROT_SPEED, FEED_TIME_ROT_SPEED / 2},
    {11, 31, 0, 380, 360, FEED_TIME_ROT_SPEED, FEED_TIME_ROT_SPEED / 2},
    {11, 31, 30, 380, 360, FEED_TIME_ROT_SPEED, FEED_TIME_ROT_SPEED / 2}};

struct free_feed_t free_feed = {5, 7200, 720, 360, FREE_FEED_ROT_SPEED, FEED_TIME_ROT_SPEED / 2};

pin_t enable_pin = {&DDRD, &PORTD, PD5};
pin_t dir_pin = {&DDRD, &PORTD, PD6};
pin_t step_pin = {&DDRD, &PORTD, PD7};

void check_feeding_time()
{
   ds3231_read_clock(&clock);
   loadClock(&last_saved_clock);

   int starting_index = loadLastFeedIndex() + 1;
   int num_feed_times = sizeof(feed_times) / sizeof(feed_times[0]);

   uartSendStr("Starting index: ");
   uartSendStr(int2str(starting_index));
   uartSendStr("\r\n");
   // reset last feed index if it's a new day
   if ((clock.day != last_saved_clock.day || clock.month != last_saved_clock.month || clock.year != last_saved_clock.year) && starting_index != 0)
   {
      uartSendStr("New day! resseting index counter...\r\n");
      saveLastFeedIndex(0);
      starting_index = 0;
   }

   for (int i = starting_index; i < num_feed_times; i++)
   {
      if (clock.hours > feed_times[i].hours)
      {
         give_feed(i);
         return; // just to be sure that it feeds the cat only once
      }
      else if (clock.hours == feed_times[i].hours && clock.minutes > feed_times[i].minutes)
      {
         give_feed(i);
         return;
      }
      else if (clock.hours == feed_times[i].hours && clock.minutes == feed_times[i].minutes && clock.seconds > feed_times[i].seconds)
      {
         give_feed(i);
         return;
      }
   }
}

void give_feed(uint8_t index)
{
   uartSendStr("Feeding time! ");
   uartSendStr(int2str(feed_times[index].hours));
   uartSendStr(":");
   uartSendStr(int2str(feed_times[index].minutes));
   uartSendStr(":");
   uartSendStr(int2str(feed_times[index].seconds));

   uartSendStr(" with rotation: ");
   uartSendStr(int2str(feed_times[index].rotation));

   uartSendStr(" and speed: ");
   uartSendStr(int2str(feed_times[index].speed * 1000));
   uartSendStr("rpms\r\n");

   // gives the food
   uartSendStr("Feeding...\r\n");
   setSpeed(feed_times[index].speed);
   stepperForward(&stepper);
   rotateByDegrees(&stepper, feed_times[index].rotation);

   // takes the food back
   uartSendStr("Taking the food back...\r\n");
   setSpeed(feed_times[index].back_speed);
   stepperBackward(&stepper);
   rotateByDegrees(&stepper, feed_times[index].rotation_reversed);

   // save stuff in eeprom in case of power loss
   uartSendStr("Saving Index ");
   uartSendStr(int2str(index));
   uartSendStr("...\r\n");
   saveLastFeedIndex(index);
   uartSendStr("Saving Clock...\r\n");
   saveClock(&clock);
   last_saved_clock = clock;
   uartSendStr("Feeding done!\r\n");
}

void initialize()
{
   uartInit(9600);
   i2cInit(true);

   // init stepper at pin 7 and enable at pin 6
   setPinMode(&enable_pin, OUTPUT);
   setPinMode(&dir_pin, OUTPUT);
   setPinMode(&step_pin, OUTPUT);

   initStepper(&stepper, &step_pin, &dir_pin, &enable_pin);

   timers_init(); // for millis and micros

   uartSendStr("Initialized!\r\n");
}

void printClock(struct ds3231_clock_t *clk)
{
   uartSendStr(int2str(clk->hours));
   uartSendStr(":");
   uartSendStr(int2str(clk->minutes));
   uartSendStr(":");
   uartSendStr(int2str(clk->seconds));
   uartSendStr("\r\n");
}

void loop()
{
   ds3231_read_clock(&clock);
   uartSendStr("Time: ");
   printClock(&clock);

   // rotateByDegrees(&stepper, 360);
   check_feeding_time();
   delay(1000);

   // eeprom test
   // uartSendStr("Reading clock from eeprom...\r\n");
   // loadClock(&clock);
   // printClock(&clock);
   // delay(1000);
   // uartSendStr("Saving clock to eeprom...\r\n");
   // ds3231_read_clock(&clock);
   // saveClock(&clock);
   // printClock(&clock);
   // delay(1000);
   // uartSendStr("Reading clock from eeprom...\r\n");
   // loadClock(&clock);
   // printClock(&clock);
   // delay(5000);
   // uartSendStr("Repeating! =================================\r\n");
}

int main(void)
{
   initialize();
   while (1)
   {
      loop();
   }
}
