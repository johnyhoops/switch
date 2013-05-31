#ifndef REG_H
#define REG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif


enum reg_errors {
	reg_kOK = 0,
	reg_kAddressInvalid
};

enum reg_registers {

	Output1 = 0,			// digital outputs
	Output2,
	Input1,					// analog inputs
	Input2,
	InputI2C,

	Philips1,				// processed analog inputs
	Philips2,
	Honeywell1,
	Honeywell2,
	Counter1,				// processed digital inputs

	Counter2,
	Frequency1,
	Frequency2,
	Keypad,					// user interface
	DisplayIntegerFormat,

	DisplayInteger,
	DisplayStringFormat,
	DisplayString,
	DisplayString2,
	DisplayString3,

	DisplayString4,
	SlaveAddress,			// eeprom storage
	Eeprom1,
	Eeprom2,
	Eeprom3,
};

uint8_t reg_init(void);
uint8_t reg_update(void);

uint8_t reg_setRegister(uint8_t registerAddress, int16_t registerValue);
uint8_t reg_getRegister(uint8_t registerAddress, int16_t* registerValue);


#ifdef __cplusplus
} // extern "C"
#endif


#endif
