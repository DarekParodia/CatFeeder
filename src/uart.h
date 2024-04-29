#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

#include "general.h"

void uartInit(uint32_t baudrate);
void uartSend(uint8_t data);
void uartSendStr(const char *data);

#endif // UART_H