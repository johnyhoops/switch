#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define map_kOK 0
#define kAddressInvalid 1

#define kSlaveAddressEEPROM 0

#define map_kDigit1Register	0
#define map_kDigit2Register	1
#define map_kDigit3Register	2
#define map_kDigit4Register	3
#define map_kOutput1Register 4
#define map_kOutput2Register 5
#define map_kSlaveAddressRegister 6

#define map_kReadKeyRegister	8
#define map_kADCInput1Register 9
#define map_kADCInput2Register 10
#define map_kI2CPressureRegister 11
#define map_kSecondsRegister 12
#define map_kMinutesRegister 13
#define map_kHoursRegister 14
#define map_kTicksRegister 15

#define map_kEEPROMBaseRegister 16

uint8_t map_setRegister(uint16_t registerAddress, int16_t registerValue);
uint8_t map_getRegister(uint16_t registerAddress, int16_t* registerValue);
uint8_t map_update(void);

#endif
