#ifndef UART_H
#define UART_H

#include <stdint.h>

#define uart_kBufferSize	32

#define uart_kOK	0
#define uart_kBUSY	1
#define uart_kIDLE	2

typedef void(*rxFunction)(void);

uint8_t uart_open(rxFunction rxCallback);
uint8_t uart_tx(uint8_t* data, uint8_t bytesToSend);
uint8_t uart_rx(uint8_t* data, uint8_t* bytesRead);

#endif
