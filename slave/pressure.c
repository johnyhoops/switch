#include <avr/io.h>
#include <avr/interrupt.h>

#include "pressure.h"

#include "twi.h"


int16_t pressure_get(int16_t type);

HAL pressure = {
	pressure_get,
	0
};

static volatile uint8_t pressure_i2cStatus = 0;
static const uint8_t pressure_i2cAddress = 72;


void pressure_init(void)
{
	if(pressure_i2cStatus == 0){
		pressure_i2cStatus = ~0;
		twi_init();
	}
}


int16_t pressure_get(int16_t type)
{
	int16_t value;
	uint8_t rxBuffer[2];
	uint8_t config = 0x8F;

	pressure_init();

	twi_writeTo(pressure_i2cAddress, &config, 1, 1);
	twi_readFrom(pressure_i2cAddress, rxBuffer, 2);

	value = (rxBuffer[0] << 8) + rxBuffer[1];

	return value;
}
