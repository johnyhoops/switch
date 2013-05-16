#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>
#include <stdio.h>

#include "map.h"


class Slave {

public:
	Slave(uint8_t address);
	int16_t get(uint16_t address);
	void set(uint16_t address, int16_t value);
	void set(uint16_t address, char* string);

private:
	uint8_t getRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t setRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t slaveAddress;
	uint8_t format;
};

#endif
