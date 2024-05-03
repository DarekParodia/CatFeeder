#include "general.h"

inline void setPinMode(volatile uint8_t *ddr, uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        *ddr |= _BV(pin);
    }
    else
    {
        *ddr &= ~_BV(pin);
    }
}

void setPin(volatile uint8_t *port, uint8_t pin, uint8_t value)
{
    if (value == HIGH)
    {
        *port |= _BV(pin);
    }
    else
    {
        *port &= ~_BV(pin);
    }
}

void togglePin(volatile uint8_t *port, uint8_t pin)
{
    *port ^= _BV(pin);
}

inline void delay(volatile uint16_t ms)
{
    _delay_ms(ms);
}

inline void delayMicroseconds(volatile uint16_t us)
{
    _delay_us(us);
}

char *int2str(int value)
{
    static char str[12];
    snprintf(str, 12, "%d", value);
    return str;
}

char *long2str(long value)
{
    static char str[12];
    snprintf(str, 12, "%ld", value);
    return str;
}

void saveClock(struct ds3231_clock_t *clock)
{
    eeprom_write_block(clock, (void *)EEPROM_ADR_CLOCK, sizeof(struct ds3231_clock_t));
}

void loadClock(struct ds3231_clock_t *clock)
{
    eeprom_read_block(clock, (void *)EEPROM_ADR_CLOCK, sizeof(struct ds3231_clock_t));
}

// Timer stuff
// i stole this code from arduino btw
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

unsigned long last_reset_time_millis = 0;
unsigned long last_reset_time_micros = 0;

void timers_init()
{
    // set timer 0 prescale factor to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // enable timer 0 overflow interrupt
    TIMSK0 |= (1 << TOIE0);

    TCNT0 = 0;

    // enable global interrupts

    sei();
}
unsigned long millis()
{
    unsigned long m;
    uint8_t oldSREG = SREG;

    cli();
    m = timer0_millis;
    SREG = oldSREG;

    return m;
}

unsigned long micros()
{
    unsigned long m;
    uint8_t oldSREG = SREG, t;

    cli();
    m = timer0_overflow_count;
#if defined(TCNT0)
    t = TCNT0;
#elif defined(TCNT0L)
    t = TCNT0L;
#else
#error TIMER 0 not defined
#endif

#ifdef TIFR0
    if ((TIFR0 & _BV(TOV0)) && (t < 255))
        m++;
#else
    if ((TIFR & _BV(TOV0)) && (t < 255))
        m++;
#endif

    SREG = oldSREG;

    return ((m << 8) + t) * (64 / (F_CPU / 1000000L));
}
ISR(TIMER0_OVF_vect)
{
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX)
    {
        f -= FRACT_MAX;
        m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}
