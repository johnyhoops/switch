#include "util/delay.h"

#include "modbus.h"
#include "bios.h"
#include "reg.h"

#define kRevision "R 11"


void delay(uint16_t ms)
{
	while(ms--){
		_delay_ms(1);
	}
}


int main(void)
{

	modbus_init();
	
	//reg_setDisplay(kRevision);
	delay(1000);
	//reg_setDisplay("");
	
	while(1){	
		reg_update();
	}

	return 0;
}
