#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>
#include <stdio.h>

#include "Print.h"

#define kOK 0
#define kOverrunError 1

#define kCRCError 3
#define kTimeoutError 4
#define kFrameError 5

class Slave : public Print {

public:
	Slave(uint8_t address);
	virtual size_t write(uint8_t);
	uint8_t readRegisters(uint16_t* data, uint16_t address, uint8_t quantity);
	uint8_t writeRegisters(uint16_t* data, uint16_t address, uint8_t quantity);

private:
	uint8_t slaveAddress;
};

#endif
