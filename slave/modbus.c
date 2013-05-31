#include <util/crc16.h>

#include "uart.h"
#include "reg.h"
#include "asm.h"


#define kBroadcastAddress 			0

#define kSlaveAddressOffset 		0
#define kFunctionCodeOffset 		1

#define kFunctionReadRegisters 		3
#define kFunctionWriteRegisters 	16

#define kFunctionInvalidException 	1
#define kAddressInvalidException 	2

#define kErrorMask 					0x80

static uint8_t slaveAddress;
static uint8_t buffer[uart_kBufferSize];

void modbus_update(void);


uint16_t crc(uint8_t* data, uint8_t bytes)
{
	uint16_t CRC = 0xFFFF;

	while(bytes--){ 							// crc check whole frame
		CRC = _crc16_update(CRC, *data++);
	}
	return CRC;
}


void modbus_send(uint8_t* data, uint8_t bytes)
{
	if(bytes + 2 < uart_kBufferSize){
		uint16_t CRC = crc(data, bytes);		// calculate and add the crc
		data[bytes] = CRC & 0xFF;
		data[bytes + 1] = (CRC >> 8) & 0xFF;
		uart_tx(data, bytes + 2);
	}
}


void modbus_init(void)
{
	uart_open(modbus_update);
	int16_t address;
	reg_getRegister(SlaveAddress, &address);
	slaveAddress = address;
}


void modbus_update(void)
{
	uint8_t bytes, error;
	int16_t value;

	if(uart_rx(buffer, &bytes) != uart_kOK) return;
	if(crc(buffer, bytes) != 0) return;
	if((buffer[kSlaveAddressOffset] != slaveAddress) 
			&& (buffer[kSlaveAddressOffset] != kBroadcastAddress)) return;
			
			//asm_htons(*(uint16_t*)(&buffer[kRegisterAddressOffset]));
	
	uint16_t registerAddress = ((uint16_t)buffer[2] << 8) + (uint16_t)buffer[3];
	uint16_t registerQuantity = ((uint16_t)buffer[4] << 8) + (uint16_t)buffer[5];
	
	if(buffer[kFunctionCodeOffset] == kFunctionReadRegisters){
		if(buffer[kSlaveAddressOffset] == kBroadcastAddress) return;
		uint8_t byteCount = registerQuantity * 2;
		buffer[2] = byteCount;
		for(uint8_t i = 0 ; i < registerQuantity ; i++){
			error = reg_getRegister(registerAddress + i, &value);
			if(error != reg_kOK) break;
			buffer[3 + i * 2] = (uint16_t)value >> 8;
			buffer[4 + i * 2] = (uint16_t)value & 0xFF;		
		}
		if(error != reg_kOK){
			buffer[kFunctionCodeOffset] |= kErrorMask;
			buffer[2] = kAddressInvalidException;
			modbus_send(buffer, 3);		
		} else {
			modbus_send(buffer, 3 + registerQuantity * 2);
		} 
		
	} else if (buffer[kFunctionCodeOffset] == kFunctionWriteRegisters){
		for(uint8_t i = 0 ; i < registerQuantity ; i++){
			error = reg_setRegister(registerAddress + i, 
					((uint16_t)buffer[7 + i * 2] << 8) + (uint16_t)buffer[8 + i * 2]);
			if(error != reg_kOK) break;
		}
		if(buffer[kSlaveAddressOffset] == kBroadcastAddress) return;
		if(error != reg_kOK){
			buffer[kFunctionCodeOffset] |= kErrorMask;
			buffer[2] = kAddressInvalidException;
			modbus_send(buffer, 3);
		}	else {
			modbus_send(buffer, 6);
		}
		
	} else {
		if(buffer[kSlaveAddressOffset] == kBroadcastAddress) return;
		buffer[kFunctionCodeOffset] |= kErrorMask;
		buffer[2] = kFunctionInvalidException;
		modbus_send(buffer, 3);
	}
}

