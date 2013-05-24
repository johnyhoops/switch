#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"


int16_t adc_get(int16_t channel);

const HAL adc = {
	adc_get,
	0
};

static volatile uint16_t adc_data[2];
static volatile uint8_t adc_channel = 0xFF;

void adc_init(void)
{
	if(adc_channel == 0xFF){
		PORTC |= (1 << 2);	// turn on auxiliary power to VADC
		DDRC |= (1 << 2);

		adc_channel = 0;
		ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS0) | (1 << ADIE);

		ADMUX = adc_channel | (1 << REFS0);	// AVCC for AREF and ADCx
		ADCSRA |= (1 << ADSC);	// start conversion
	}
}


int16_t adc_get(int16_t channel)
{
	uint8_t sreg;
	int16_t value;

	adc_init();

	if((channel < ADC_IN2) || (channel > ADC_IN1)){
		return 0;
	}

	sreg = SREG;
	cli();
	value = adc_data[channel];
	SREG = sreg;
	return value;
}


ISR(ADC_vect)
{
	adc_data[adc_channel] = ADC;
	adc_channel = (adc_channel + 1) & 0x01;
	ADMUX = adc_channel | (1 << REFS0); 			// AVCC for AREF and ADCx
	ADCSRA |= (1 << ADSC);        				// start next conversion
}
