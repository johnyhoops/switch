#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c.h"
#include "twi.h"


int16_t i2c_get(int16_t type);

HAL i2c = {
	i2c_get,
	0
};

static volatile uint8_t i2c_status = 0;
static const uint8_t i2c_address = 72;


void i2c_init(void)
{
	if(i2c_status == 0){
		i2c_status = ~0;
		twi_init();
	}
}


int16_t i2c_get(int16_t type)
{
	int16_t value;
	uint8_t rxBuffer[2];
	uint8_t config = 0x8F;

	i2c_init();

	twi_writeTo(i2c_address, &config, 1, 1);
	twi_readFrom(i2c_address, rxBuffer, 2);

	value = (rxBuffer[0] << 8) + rxBuffer[1];

	return value;
}
