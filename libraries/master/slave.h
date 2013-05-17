#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>
#include <stdio.h>

#include "map.h"


class Slave {

public:
	Slave(unsigned int address);
	int get(unsigned int address);
	void set(unsigned int address, int value);
	void set(unsigned int address, char* string);

private:
	uint8_t getRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t setRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t slaveAddress;
	uint8_t format;
};

#endif
