#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "bios.h"
#include "eeprom.h"

int16_t eeprom_get(int16_t address);
int16_t eeprom_set(int16_t address, int16_t value);

HAL eeprom = {
	eeprom_get,
	eeprom_set
};

int16_t eeprom_get(int16_t address)
{
	return (int16_t)eeprom_read_word((uint16_t*)(address << 1));
}

int16_t eeprom_set(int16_t address, int16_t value)
{
	eeprom_write_word((uint16_t*)(address << 1), (uint16_t)value);
	return 0;
}
