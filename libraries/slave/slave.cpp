#include <avr/io.h>
#include <util/crc16.h>
#include <util/delay.h>

#include "slave.h"
#include "asm.h"

#define kRXTimeout 100
#define kCharTimeout 20

#define kSlaveAddressOffset 0
#define kFunctionCodeOffset 1
#define kRegisterAddressOffset 2
#define kRegisterQuantityOffset 4

#define kReadDataOffset 3
#define kWriteDataOffset 7

#define kFunctionReadRegisters 3
#define kFunctionWriteRegisters 16

#define kBufferSize 32


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
	// init uart
	UCSRB = (1 << TXEN);	// uart 19200 8 n 1
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = 25;
	UCSRA |= (1 << TXC);	// clear tx complete flag

	for(uint8_t i = 0 ; i < bytes ; i++){
		while(!( UCSRA & (1 << UDRE)));
		UDR = buffer[i];
	}
	while(!(UCSRA & (1 << TXC)));

	UCSRB |= (1 << RXEN);	// enable receiver
}


uint8_t rx(uint8_t* buffer, uint8_t* bytes)
{
	*bytes = 0;
	uint8_t timer = kRXTimeout;
	while(*bytes < kBufferSize){
		while (!(UCSRA & (1 << RXC))){
			if(timer) --timer;
			if((timer == 0) && (*bytes)) return kOK;
			if(timer == 0) return kTimeoutError;
			_delay_us(50);
		}
		timer = kCharTimeout;
		buffer[*bytes] = UDR;
		*bytes = *bytes + 1;
	}
	return kOverrunError;
}


void send(uint8_t* buffer, uint8_t bytes)
{
	if(bytes + 2 < kBufferSize){
		uint16_t CRC = crc(buffer, bytes);		// calculate and add the crc
		buffer[bytes] = CRC & 0xFF;
		buffer[bytes + 1] = (CRC >> 8) & 0xFF;
		tx(buffer, bytes + 2);
	}
}


uint8_t receive(uint8_t* buffer, uint8_t* bytes)
{
	uint8_t error, byteCount;
	
	*bytes = 0;
	error = rx(buffer, &byteCount);
	if(error) return error;
	if(byteCount < 4) return kFrameError;
	uint16_t CRC = crc(buffer, byteCount);		// calculate the crc
	if(CRC) return byteCount; //kCRCError;
	*bytes = byteCount - 2;
	return kOK;
}


Slave::Slave(uint8_t address)
{
	slaveAddress = address;
}


uint8_t Slave::readRegisters(uint16_t* data, uint16_t address, uint8_t quantity)
{
	uint8_t buffer[kBufferSize];
	// build frame
	buffer[kSlaveAddressOffset] = slaveAddress;
	buffer[kFunctionCodeOffset] = kFunctionReadRegisters;
	*(uint16_t*)(&buffer[kRegisterAddressOffset]) = asm_htons(address);
	*(uint16_t*)(&buffer[kRegisterQuantityOffset]) = asm_htons(quantity);
	// send frame
	send(buffer, kRegisterQuantityOffset + 2);
	// receive response
	uint8_t bytes;
	return kOK;
	uint8_t error = receive(buffer, &bytes);
	if(error != kOK) return error;
	// extract data from frame
	for(uint8_t i = 0 ; i < quantity ; i++){
		data[i] = asm_htons(*(uint16_t*)(&buffer[kReadDataOffset + i * 2]));
	}	
	return kOK;
}

uint8_t Slave::writeRegisters(uint16_t* data, uint16_t address, uint8_t quantity)
{
	uint8_t buffer[kBufferSize];
	// build frame
	buffer[kSlaveAddressOffset] = slaveAddress;
	buffer[kFunctionCodeOffset] = kFunctionWriteRegisters;
	*(uint16_t*)(&buffer[kRegisterAddressOffset]) = asm_htons(address);
	*(uint16_t*)(&buffer[kRegisterQuantityOffset]) = asm_htons(quantity);
	for(uint8_t i = 0 ; i < quantity ; i++){
		*(uint16_t*)(&buffer[kWriteDataOffset + i * 2]) = asm_htons(data[i]);
	}	
	// send frame
	send(buffer, kWriteDataOffset + 2);
	// receive response
	uint8_t bytes;
	//return kOK;
	uint8_t error = receive(buffer, &bytes);
	if(error != kOK) return error;

	return kOK;
}

size_t Slave::write(uint8_t data) {
return 0;}
