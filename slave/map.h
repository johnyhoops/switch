#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C"{
#endif

enum map_registers {
	// digital outputs
	Output1 = 0,
	Output2,

	// analog inputs
	Input1,
	Input2,
	InputI2C,

	// processed digital inputs
	Counter1,
	Frequency1,
	Counter2,
	Frequency2,

	// processed analog inputs
	Philips1,
	Philips2,
	Honeywell1,
	Honeywell2,

	// user interface
	Keypad,
	DisplayIntegerFormat,
	DisplayInteger,
	DisplayStringFormat,
	DisplayString1,
	DisplayString2,
	DisplayString3,
	DisplayString4,

	// eeprom storage
	SlaveAddress,
	Eeprom1,
	Eeprom2,
	Eeprom3,
};

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MAP_H */
