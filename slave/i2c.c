#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c.h"
#include "twi.h"

#define i2c_address 72
static int16_t value = 0;


static void init(uint8_t reference)
{
	twi_init();
}


static void update(uint8_t reference)
{
	uint8_t rxBuffer[2];
	uint8_t config = 0x8F;

	twi_writeTo(i2c_address, &config, 1, 1);
	twi_readFrom(i2c_address, rxBuffer, 2);

	value = (rxBuffer[0] << 8) + rxBuffer[1];
}


static int16_t getValue(uint8_t reference)
{
	return value;
}


HAL i2cHAL = {
	init,
	update,
	getValue,
	0
};


HAL* i2c(void)
{
	return &i2cHAL;
}
