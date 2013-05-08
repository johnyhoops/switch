#include <avr/io.h>
#include <avr/eeprom.h>

#include "map.h"
#include "bios.h"
#include "charmap.h"
#include "adc.h"
#include "pressure.h"

static int16_t pressureMeasurement = 0;

uint8_t map_setRegister(uint16_t registerAddress, int16_t registerValue)
{
	if((registerAddress >= map_kDigit1Register) && (registerAddress <= map_kDigit4Register)){
		bios_segment[registerAddress] = charmap_getSegments((uint16_t)registerValue >> 8);
		bios_segment[registerAddress + 4] = charmap_getSegments((uint16_t)registerValue & 0xFF);
		return map_kOK;
	}
	if(registerAddress == map_kOutput1Register){
		DDRB |= (1 << PORTB2);
		if(registerValue){
			PORTB |= (1 << PORTB2); 
		} else {
			PORTB &= ~(1 << PORTB2);
		} 
		return map_kOK;
	}
	if(registerAddress == map_kOutput2Register){
		DDRB |= (1 << PORTB1);
		if(registerValue){
			PORTB |= (1 << PORTB1); 
		} else {
			PORTB &= ~(1 << PORTB1);
		} 
		return map_kOK;
	}
	if(registerAddress == map_kSlaveAddressRegister){
		if((uint8_t)((uint16_t)registerValue >> 8) ^ (uint8_t)((uint16_t)registerValue & 0xFF) == 0xFF){
			eeprom_write_word(kSlaveAddressEEPROM, (uint16_t)registerValue);
			return map_kOK;
		}
	}
	return kAddressInvalid;
}


uint8_t map_getRegister(uint16_t registerAddress, int16_t* registerValue)
{
	if(registerAddress == map_kReadKeyRegister){
		*registerValue = bios_getKey();
		return map_kOK;
	}
	if(registerAddress == map_kADCInput1Register){
		*registerValue = adc.getValue(ADC_IN1);
		return map_kOK;
	}
	if(registerAddress == map_kADCInput2Register){
		*registerValue = adc.getValue(ADC_IN2);
		return map_kOK;
	}
	if(registerAddress == map_kI2CPressureRegister){
		*registerValue = pressureMeasurement;
		return map_kOK;
	}
	if(registerAddress >= map_kEEPROMBaseRegister){
		*registerValue = eeprom_read_word((const uint16_t*)(registerAddress * 2 - map_kEEPROMBaseRegister));
		return map_kOK;
	}
	return kAddressInvalid;
}

uint8_t map_update(void)
{
	pressureMeasurement = pressure.getValue(0);

	return map_kOK;
}

