#include <string.h>

#include <avr/io.h>
#include <avr/eeprom.h>

#include "reg.h"
#include "bios.h"
#include "seg.h"
#include "adc.h"
#include "i2c.h"
#include "itoa.h"

#define kSegmentDP					1
#define kSegmentBufferSize	8
#define kNumberOfDigits			4

static int16_t i2cValue = 0;
static uint8_t format = 0;

void reg_init(void)
{
	DDRB |= (1 << PORTB2) | (1 << PORTB1);
	TCCR1B = (1 << CS11) | (1 << CS10);
	TCCR1A = (1 << WGM10);
}

void setDisplay(char* str)
{
	uint8_t segmentBuffer[kSegmentBufferSize];
	uint8_t segmentBufferIndex = 0;
	uint8_t segments;
	
	memset(segmentBuffer, 0, kSegmentBufferSize);
	while(*str){
		if(*str == '.'){
			if(segmentBufferIndex != 0){
				segmentBufferIndex--;
			}
			segments = segmentBuffer[segmentBufferIndex] | kSegmentDP;
		} else {
			segments = seg_getSegments(*str);
		}
		segmentBuffer[segmentBufferIndex] = segments;
		if((format & Flash) == 0){
			segmentBuffer[segmentBufferIndex + kNumberOfDigits] = segments;
		}
		if(++segmentBufferIndex > 3){
			break;
		}
		str++;
	}	
	memcpy(bios_segment, segmentBuffer, kSegmentBufferSize);
}

uint8_t reg_setRegister(uint16_t registerAddress, int16_t registerValue)
{
	static char str[itoa_kMaxStringLength];

	if(registerAddress == DisplayFormat){
		format = registerValue;
		return kOK;
	}
	
	if(registerAddress == DisplayInteger){
		itoa_toString(str, registerValue, format & FlashMask);
		setDisplay(str);
		return kOK;
	}
	
	if(registerAddress == DisplayString){	
		*((int16_t*)(&str[0])) = registerValue;
		return kOK;
	}
	
	if(registerAddress == DisplayString + 1){	
		*((int16_t*)(&str[2])) = registerValue;
		str[4] = 0;
		setDisplay(str);
		return kOK;
	}
	
	if(registerAddress == Output1){
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
		return kOK;
	}
	
	if(registerAddress == Output2){
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
		return kOK;
	}
	
	if(registerAddress == SlaveAddress){
		if(((uint8_t)((uint16_t)registerValue >> 8) 
			^ (uint8_t)((uint16_t)registerValue & 0xFF)) == 0xFF){
			eeprom_write_word(kSlaveAddressEEPROM, (uint16_t)registerValue);
		}
		return kOK;
	}
	return kAddressInvalid;
}


uint8_t reg_getRegister(uint16_t registerAddress, int16_t* registerValue)
{
	if(registerAddress == Keypad){
		*registerValue = bios_getKey();
		return kOK;
	}
	if(registerAddress == Input1){
		*registerValue = adc.getValue(ADC_IN1);
		return kOK;
	}
	if(registerAddress == Input2){
		*registerValue = adc.getValue(ADC_IN2);
		return kOK;
	}
	if(registerAddress == InputI2C){
		*registerValue = i2cValue;
		return kOK;
	}
	if(registerAddress >= EEPROM){
		*registerValue = eeprom_read_word((const uint16_t*)(registerAddress * 2 - EEPROM));
		return kOK;
	}
	return kAddressInvalid;
}

uint8_t reg_update(void)
{
	i2cValue = i2c.getValue(0);
	return kOK;
}

