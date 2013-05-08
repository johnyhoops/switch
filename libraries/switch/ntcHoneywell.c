#include <avr/pgmspace.h>

#include "ntcHoneywell.h"

static int16_t getValue(int16_t reference);

HAL ntcHoneywell = {
		getValue,
		0
};

// stainless steel cased NTC thermistor from Honeywell with 4K7 drive resistor
#define TEMP_TABLE {	1018,	1016,	1013,	1008,	1003,	995,		986,		974,	\
					960,		942,		920,		894,		864,		829,		790,		747,	\
					701,		653,		602,		552,		502,		453,		406,		363,	\
					322,		286,		252,		222,		195,		172,		151,		133,	\
					117,		102,		90,		80,		70,		62,		55}


static int16_t PROGMEM TEMP_LOOKUP_TABLE[] =  TEMP_TABLE;

#define TEMP_LOOKUP_TABLE_SIZE (sizeof(TEMP_LOOKUP_TABLE) / sizeof(int16_t))

static int16_t getDeciCelcius(int16_t rawADC)
{
	int16_t ref_lo, ref_hi, delta, temp;
	int8_t i;

	i = TEMP_LOOKUP_TABLE_SIZE;

	do {
        ref_lo = (int16_t)pgm_read_word(&TEMP_LOOKUP_TABLE[--i]);
    } while (((int16_t)rawADC > ref_lo) && (i > 1));

    ref_hi = (int16_t)pgm_read_word(&TEMP_LOOKUP_TABLE[--i]);
    delta = ref_hi - ref_lo;
    temp = -400 + (i + 1) * 50 - (50 * ((int16_t)rawADC - ref_lo)) / delta;

	return temp;
}


static int16_t getValue(int16_t reference)
{
	return getDeciCelcius(adc.getValue(reference));
}



