// MODBUS RTU Serial Port Driver

// uses UART, TCNT0
// assumes 8 MHz clock

#include "string.h"

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"

#include "uart.h"

// internal buffers
static uint8_t tx_buffer[uart_kBufferSize];
static uint8_t rx_buffer[uart_kBufferSize];

static uint8_t* tx_ptr;
static uint8_t* rx_ptr;

static uint8_t tx_ctr;
static uint8_t rx_ctr;
static uint8_t rx_status;

static rxFunction callback;

#define uart_kTimer0Reload (255 - 15)


uint8_t uart_open(rxFunction rxCallback)
{
	tx_ptr = tx_buffer;
	rx_ptr = rx_buffer;
	tx_ctr = 0;
	rx_ctr = 0;
	rx_status = uart_kIDLE;
	
	callback = rxCallback;

	TCCR0 = (1 << CS02) | (1 << CS00);	// set timer clk period 128 us
	
	UCSRB = (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);	// uart 19200 8 n 1
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = 25;											

	sei();	// enable interrupts

	return uart_kOK;
}


uint8_t uart_tx(uint8_t* data, uint8_t bytesToSend)
{
	if (tx_ptr != tx_buffer) return uart_kBUSY;
	
	tx_ctr = bytesToSend;
	memcpy(tx_buffer, data, tx_ctr);	// set up buffer	
		
	UCSRB &= ~(1 << RXEN);		// disable receiver
	UCSRB &= ~(1 << RXCIE);		// disable RX interrupts
	UCSRB |=  (1 << UDRIE);		// enable UDRE interrupt
	UCSRB &= ~(1 << TXCIE);		// disable tx complete interrupt
	
	return uart_kOK;
}

uint8_t uart_rx(uint8_t* data, uint8_t* bytesRead)
{
	if(rx_status == uart_kOK){	
		memcpy((char*)data, (char*)rx_buffer, rx_ctr);
		*bytesRead = rx_ctr;
		rx_status = uart_kIDLE;	
		return uart_kOK;
	}  
	return rx_status;
}


// *** ISR Routines ***

// tx data register empty interrupt
ISR(USART_UDRE_vect)
{
	// are there more bytes to send
	if (tx_ctr > 0){
		UDR = *(tx_ptr++);
		tx_ctr--;
	}
	// was that the last byte to send
	if (tx_ctr == 0){
		UCSRB &= ~(1 << UDRIE);		// disable data ready interrupt
		UCSRB |= (1 << TXCIE);  	// enable tx complete interrupt
	}
}

// tx complete interrupt
ISR(USART_TXC_vect)
{
	UCSRB &= ~(1 << TXCIE);		// disable tx complete interrupt
	UCSRB |= (1 << RXEN);		// enable receiver
	UCSRB |= (1 << RXCIE);		// enable rx interrupts
	tx_ptr = tx_buffer;
}

// rx data interrupt
ISR(USART_RXC_vect)
{
	TCNT0 = uart_kTimer0Reload;	// reset timeout counter

	if(rx_status != uart_kBUSY){	
		rx_ptr = rx_buffer;	// reset if unread data still in buffer
		rx_ctr = 0;
		rx_status = uart_kBUSY;
	}
	
	do {
		uint8_t rxData = UDR; // read data 
		if(rx_ctr < uart_kBufferSize){
			*rx_ptr++ = rxData;
			rx_ctr++;
		}
		
	} while (UCSRA & (1 << RXC));

	TIFR |= (1 << TOV0);	// clear overflow flag
	TIMSK |= (1 << TOIE0);	// enable overflow interrupt
}

// rx timer timout interrupt
ISR(TIMER0_OVF_vect)
{
	TIMSK &= ~(1 << TOIE0);	// disable overflow interrupt	
	rx_status = uart_kOK;
	if(callback) (*callback)();
}
