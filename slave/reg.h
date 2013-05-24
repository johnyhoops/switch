#ifndef REG_H
#define REG_H

#include <stdint.h>

#define reg_kOK 								0
#define reg_kAddressInvalid 		1

#define reg_kSlaveAddress 			0
#define reg_kOutput1 						1
#define reg_kOutput2 						2

#define reg_kInput1 						8
#define reg_kInput2							9
#define reg_kInputI2C						10
#define reg_kKeypad							11
#define reg_kCounter1						12
#define reg_kFrequency1					13
#define reg_kCounter2						14
#define reg_kFrequency2					15
#define reg_kPhilips1						16
#define reg_kPhilips2						17
#define reg_kHoneywell1					18
#define reg_kHoneywell2					19

#define reg_kDisplayString			32
#define reg_kDisplayFormat 			34
#define reg_kDisplayInteger			35



#define reg_kEEPROM							64

#define reg_kFlash							128
#define reg_kFlashMask					(reg_kFlash - 1)

#define reg_kSlaveAddressEEPROM 0

void reg_init(void);
uint8_t reg_update(void);
void reg_setDisplay(char* str);
uint8_t reg_setRegister(uint16_t registerAddress, int16_t registerValue);
uint8_t reg_getRegister(uint16_t registerAddress, int16_t* registerValue);

#endif
