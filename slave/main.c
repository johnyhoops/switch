#include "util/delay.h"
#include "avr/wdt.h"

#include "modbus.h"
#include "reg.h"

#define kRevision 13


void delay(uint16_t ms)
{
	while(ms--) _delay_ms(1);
}


int main(void)
{
	wdt_enable(WDTO_2S);
	reg_init();
	
	reg_setRegister(DisplayInteger, kRevision);
	delay(1000);
	reg_setRegister(DisplayString, 0);
	
	modbus_init();

	while(1){	
		reg_update();
		if(SREG & 0b10000000) wdt_reset();
		delay(100);
	}

	return 0;
}
