#include <string.h>

#include <avr/io.h>
#include <avr/eeprom.h>

#include "reg.h"
#include "bios.h"
#include "seg.h"
#include "adc.h"
#include "i2c.h"
#include "itoa.h"

#define reg_kSegmentDP					1
#define reg_kSegmentBufferSize	8
#define reg_kNumberOfDigits			4

static int16_t i2cValue = 0;
static uint8_t format = 0;

void reg_init(void)
{
	DDRB |= (1 << PORTB2) | (1 << PORTB1);
	TCCR1B = (1 << CS11) | (1 << CS10);
	TCCR1A = (1 << WGM10);
}

void reg_setDisplay(char* str)
{
	uint8_t segmentBuffer[reg_kSegmentBufferSize];
	uint8_t segmentBufferIndex = 0;
	uint8_t segments;
	
	memset(segmentBuffer, 0, reg_kSegmentBufferSize);
	while(*str){
		if(*str == '.'){
			if(segmentBufferIndex != 0){
				segmentBufferIndex--;
			}
			segments = segmentBuffer[segmentBufferIndex] | reg_kSegmentDP;
		} else {
			segments = seg_getSegments(*str);
		}
		segmentBuffer[segmentBufferIndex] = segments;
		if((format & reg_kFlash) == 0){
			segmentBuffer[segmentBufferIndex + reg_kNumberOfDigits] = segments;
		}
		if(++segmentBufferIndex > 3){
			break;
		}
		str++;
	}	
	memcpy(bios_segment, segmentBuffer, reg_kSegmentBufferSize);
}

uint8_t reg_setRegister(uint16_t registerAddress, int16_t registerValue)
{
	static char str[itoa_kMaxStringLength];

	if(registerAddress == reg_kDisplayFormat){
		if(registerValue & reg_kFlash){
			memset(bios_segment + reg_kNumberOfDigits, 0, reg_kNumberOfDigits);
		} else {
			memcpy(bios_segment + reg_kNumberOfDigits, bios_segment, reg_kNumberOfDigits);
		}
		format = registerValue;
		return reg_kOK;
	}
	
	if(registerAddress == reg_kDisplayInteger){
		itoa_toString(str, registerValue, format & reg_kFlashMask);
		reg_setDisplay(str);
		return reg_kOK;
	}
	
	if(registerAddress == reg_kDisplayString){	
		*((int16_t*)(&str[0])) = registerValue;
		return reg_kOK;
	}
	
	if(registerAddress == reg_kDisplayString + 1){	
		*((int16_t*)(&str[2])) = registerValue;
		str[4] = 0;
		reg_setDisplay(str);
		return reg_kOK;
	}
	
	if(registerAddress == reg_kOutput1){
		if(registerValue <= 0){
			TCCR1A &= ~(1 << COM1B1);
			PORTB &= ~(1 << PORTB2);
		} else if(registerValue >= 255){
			TCCR1A &= ~(1 << COM1B1);
			PORTB |= (1 << PORTB2); 
		} else {
			TCCR1A |= (1 << COM1B1);
			OCR1B = registerValue;
		}
		return reg_kOK;
	}
	
	if(registerAddress == reg_kOutput2){
		if(registerValue <= 0){
			TCCR1A &= ~(1 << COM1A1);
			PORTB &= ~(1 << PORTB1);
		} else if(registerValue >= 255){
			TCCR1A &= ~(1 << COM1A1);
			PORTB |= (1 << PORTB1); 
		} else {
			TCCR1A |= (1 << COM1A1);
			OCR1A = registerValue;
		}
		return reg_kOK;
	}
	
	if(registerAddress == reg_kSlaveAddress){
		if(((uint8_t)((uint16_t)registerValue >> 8) 
			^ (uint8_t)((uint16_t)registerValue & 0xFF)) == 0xFF){
			eeprom_write_word(reg_kSlaveAddressEEPROM, (uint16_t)registerValue);
		}
		return reg_kOK;
	}
	return reg_kAddressInvalid;
}


uint8_t reg_getRegister(uint16_t registerAddress, int16_t* registerValue)
{
	if(registerAddress == reg_kKeypad){
		*registerValue = bios_getKey();
		return reg_kOK;
	}
	if(registerAddress == reg_kInput1){
		*registerValue = adc.getValue(ADC_IN1);
		return reg_kOK;
	}
	if(registerAddress == reg_kInput2){
		*registerValue = adc.getValue(ADC_IN2);
		return reg_kOK;
	}
	if(registerAddress == reg_kInputI2C){
		*registerValue = i2cValue;
		return reg_kOK;
	}
	if(registerAddress >= reg_kEEPROM){
		*registerValue = eeprom_read_word((const uint16_t*)(registerAddress * 2 - reg_kEEPROM));
		return reg_kOK;
	}
	return reg_kAddressInvalid;
}

uint8_t reg_update(void)
{
	i2cValue = i2c.getValue(0);
	return reg_kOK;
}

