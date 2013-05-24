#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>
#include <stdio.h>

#include "map.h"

typedef struct {
	char* label;
	uint8_t format;
	uint8_t access;
	int* value;
} MenuItem;

class Slave {

public:
	Slave();
	void setSlaveAddress(unsigned int address);
	int get(unsigned int address);
	void set(unsigned int address, int value);
	void set(unsigned int address, char* string);	
	
	void attachMenu(MenuItem* menuList);
	void updateMenu(void);

private:
	uint8_t getRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t setRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t slaveAddress;
	uint8_t format;
	
	uint8_t item;
	uint8_t items;
	uint8_t state;
	uint8_t lastState;
	uint8_t lastItem;
	int16_t lastValue;
	MenuItem* menu;
};

#endif
