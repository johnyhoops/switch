#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>

class Slave {

public:
	Slave(uint8_t address);
	~Slave(void);
	uint16_t read(uint16_t address);
	void write(uint16_t address, uint16_t data);

private:
	uint8_t slaveAddress;
}

#endif
