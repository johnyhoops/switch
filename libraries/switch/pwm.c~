#include <avr/io.h>

#include "pwm.h"

int16_t pwm_get(int16_t address);
int16_t pwm_set(int16_t address, int16_t value);

HAL pwm = {
		pwm_get,
		pwm_set
};

void pwm_init(void)
{
	DDRB |= (1 << PORTB1) | (1 << PORTB2);
}

int16_t pwm_get(int16_t address)
{
	pwm_init();
	if(address == 1){
		return PORTB & (1 << PORTB2);
	}
	if(address == 2){
		return PORTB & (1 << PORTB1);
	}
	return 0;
}

int16_t pwm_set(int16_t address, int16_t value)
{
	pwm_init();
	if(address == 1){
		if(value == 0) PORTB &= ~(1 << PORTB2); else PORTB |= (1 << PORTB2);
	}
	if(address == 2){
		if(value == 0) PORTB &= ~(1 << PORTB1); else PORTB |= (1 << PORTB1);
	}
	return 0;
}
