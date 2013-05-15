#include "util/delay.h"

#include "modbus.h"
#include "bios.h"


int main(void)
{
	bios_init();
	modbus_init();
	
	while(1){	
		
		for(uint8_t i = 0 ; i < 100 ; i++) _delay_ms(1); // 100 ms delay

		reg_update();
	}

	return 0;
}
