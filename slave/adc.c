#include <avr/io.h>
#include <avr/interrupt.h>

#include "bios.h"
#include "adc.h"

#define kHighThreshold 768
#define kLowThreshold 256
#define kCounterOffset 2

static volatile uint16_t adc_result[8] = {0};
static volatile uint8_t adc_channel = 0;


static void init(uint8_t reference)
{
	PORTC |= (1 << 2);					// turn on auxiliary power to VADC
	DDRC |= (1 << 2);

	adc_channel = 0;
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS0) | (1 << ADIE);

	ADMUX = adc_channel | (1 << REFS0);	// AVCC for AREF and ADCx
	ADCSRA |= (1 << ADSC);				// start conversion

	while(adc_channel == 0);			// wait for first conversions to complete
	while(adc_channel == 1);
}


static int16_t getValue(uint8_t reference)
{
	uint8_t sreg;
	int16_t value;

	reference--;
	reference ^= 0x01;

	sreg = SREG;
	cli();
	value = adc_result[reference & 0x07];
	SREG = sreg;
	return value;
}


ISR(ADC_vect)
{
	static uint8_t seconds = 0;

	volatile uint16_t* result = &adc_result[0];


	if(adc_channel != 0){
		result++;
	}

	*result = ADC;

	if((*(result + 2) & 0x01) == 0){
		if(*result > kHighThreshold) (*(result + 2))++;
	} else {
		if(*result < kLowThreshold) (*(result + 2))++;
	}

	if(seconds != bios_seconds){
		seconds = bios_seconds;
		*(result + 4) = *(result + 2) - *(result + 6);
		*(result + 5) = *(result + 3) - *(result + 7);
		*(result + 6) = *(result + 2);
		*(result + 7) = *(result + 3);
	}

	adc_channel ^= 0x01;
	ADMUX = adc_channel | (1 << REFS0); 		// AVCC for AREF and ADCx
	ADCSRA |= (1 << ADSC);        				// start next conversion
}


HAL adcHAL = {
	init,
	0,
	getValue,
	0
};


HAL* adc(void)
{
	return &adcHAL;
}
