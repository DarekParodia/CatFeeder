#include "uart.h"

void uartInit(uint32_t baudrate)
{
    uint16_t ubrr = F_CPU / 16 / baudrate - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uartSend(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = data;
}

void uartSendStr(const char *data)
{
    while (*data)
    {
        uartSend(*data++);
    }
}