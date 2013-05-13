#include <util/crc16.h>
#include <util/delay.h>

#include "slave.h"

#define kSlaveAddressOffset 0
#define kFunctionCodeOffset 1
#define kRegisterAddressOffset 2
#define kRegisterQuantityOffset 4

#define kFunctionReadRegisters 3
#define kFunctionWriteRegisters 16


uint16_t crc(uint8_t* data, uint8_t bytes)
{
	uint16_t CRC = 0xFFFF;

	while(bytes--){ 					// crc check whole frame
		CRC = _crc16_update(CRC, *data++);
	}
	return CRC;
}


void tx(uint8_t* buffer, uint8_t bytes)
{
	UCSRB &= ~(1 << RXEN);		// disable receiver
	for(uint8_t i = 0 ; i < bytes ; i++){
		while(!( UCSRA & (1 << UDRE)));
		UDR = buffer[i];
	}
	while(!(UCSRA & (1 << TXC)));
	UCSRB |= (1 << RXEN);			// enable receiver
}


uint8_t rx(uint8_t* buffer, uint8_t* bytes)
{
	*bytes = 0;
	while(*bytes < kBufferSize){
		uint8_t timer = 0;
		while (!(UCSRA & (1 << RXC))){
			if(++timer > 9) return kOK;
			_delay_us(26);	// 1/2 character period at 19200 bps
		}
		buffer[*bytes] = UDR;
		*bytes += 1;
	}
	return kOverrunError;
}


void send(uint8_t* buffer, uint8_t bytes)
{
	if(bytes + 2 < kBufferSize){
		uint16_t CRC = crc(data, bytes);		// calculate and add the crc
		buffer[bytes] = CRC & 0xFF;
		buffer[bytes + 1] = (CRC >> 8) & 0xFF;
		uart_tx(buffer, bytes + 2);
	}
}


uint8_t receive(uint8_t* buffer, uint8_t* bytes)
{
	uint8_t byteCount, error;
	
	*bytes = 0;
	error = rx(buffer, &byteCount);
	if(error) return error;
	if(byteCount < 4) return kFrameError;
	uint16_t CRC = crc(buffer, byteCount);		// calculate the crc
	if(CRC) return kCRCError;
	*bytes = byteCount - 2;
	return kOK;
}


Slave::Slave(uint8_t address)
{
	Slave::slaveAddress = address;
	// init uart
	UCSRB = (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);	// uart 19200 8 n 1
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = 25;
}


~Slave(void)
{
	end();
}


uint8_t Slave::read(uint16_t* data, uint16_t address, uint8_t quantity)
{
	uint8_t buffer[kBufferSize];
	
	buffer[kSlaveAddressOffset] = this.slaveAddress;
	buffer[kFunctionCodeOffset] = kFunctionReadRegisters;
	buffer[kRegisterAddressOffset] = 
	
	
	buffer[kSlaveAddressOffset] = this.slaveAddress;
	buffer[kFunctionCodeOffset] = kFunctionReadRegisters;
	buffer[kRegisterAddressOffset] = address >> 8;
	buffer[kRegisterAddressOffset + 1] = address & 0xFF;
	buffer[kRegisterQuantityOffset] = 0;
	buffer[kRegisterQuantityOffset + 1] = quantity;
	send(buffer, kRegisterQuantityOffset + 2);
	uint8_t bytes;
	uint8_t error = receive(buffer, &bytes);
	if(error != kOK) return error;
	for(uint8_t i = 0 ; i < quantity ; i++){
		data[i] = (uint16_t)buffer[4 + i * 2] + ((uint16_t)buffer[3 + i * 2] << 8);
	}	
	return kOK;
}

uint8_t Slave::write(uint16_t* data, uint16_t address, uint8_t quantity)
{

}
