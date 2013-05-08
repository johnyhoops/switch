#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

enum
{
	format_kEmpty	= 0,		// display empty string for value zero
	format_Milli	= 1,		// make fixed point with 3 decimal places
	format_Centi	= 2,		// make fixed point with 2 decimal places
	format_Deci	= 3,		// make fixed point with 1 decimal places
	format_OneDigit	= 4,		// display at least one digit
	format_TwoDigits	= 5,		// display at least two digit
	format_ThreeDigits = 6,		// display at least three digit
	format_FourDigits	= 7,		// display at least four digit
	format_kFiveDigits = 8,		// display at least five digit
	format_Time	= 9,		// display as a time
	format_Bool	= 10,		// display as on / off
	format_Hide	= 11    	// do not display
};


int format_toString(char* str, int16_t value, uint8_t format);

#ifdef __cplusplus
} // extern "C"
#endif

#endif



