#ifndef ITOA_H
#define ITOA_H

#include <stdint.h>

#define itoa_kUnit						0
#define	itoa_kDeci						1
#define itoa_kCenti						2
#define itoa_kMilli						3
#define itoa_kBool						4
#define itoa_kTwoDigits				5
#define itoa_kTime						6

#define itoa_kMaxStringLength	8

uint8_t itoa_toString(char* str, int16_t value, uint8_t format);

#endif



