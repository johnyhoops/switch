#ifndef ITOA_H
#define ITOA_H

#include <stdint.h>

enum itoa_format {
	itoa_kUnit = 0,
	itoa_kDeci,
	itoa_kCenti,
	itoa_kMilli,
	itoa_kBool,
	itoa_kTwoDigits,
	itoa_kTime,
};

enum itoa_constants {
	itoa_kMaxStringLength = 8
};

uint8_t itoa_toString(char* str, int16_t value, uint8_t format);

#endif



