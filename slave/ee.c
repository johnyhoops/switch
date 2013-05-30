#include <avr/eeprom.h>

#include "ee.h"


int16_t eeprom_get(int16_t address)
{
	return (int16_t)eeprom_read_word((uint16_t*)(address << 1));
}


int16_t eeprom_set(int16_t address, int16_t value)
{
	eeprom_write_word((uint16_t*)(address << 1), (uint16_t)value);
	return 0;
}

static int16_t getValue(uint8_t reference)
{
	return (int16_t)eeprom_read_word((uint16_t*)((uint16_t)reference << 1));
}

static void setValue(uint8_t reference, int16_t value)
{
	eeprom_write_word((uint16_t*)((uint16_t)reference << 1), (uint16_t)value);
}


HAL eeHAL = {
	0,
	0,
	getValue,
	setValue
};


HAL* ee(void)
{
	return &eeHAL;
}
