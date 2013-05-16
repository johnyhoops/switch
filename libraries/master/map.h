#ifndef MAP_H
#define MAP_H

#include "../../slave/reg.h"
#include "../../slave/itoa.h"
#include "../../slave/bios.h"

#define Output1 				reg_kOutput1 
#define Output2 				reg_kOutput2

#define Input1 					reg_kInput1
#define Input2					reg_kInput2
#define InputI2C				reg_kInputI2C
#define Keypad					reg_kKeypad


#define LeftShort				bios_kLeftShort 		// for left button short push
#define CenterShort			bios_kCenterShort 	// for center button short push
#define RightShort			bios_kRightShort 	// for right button short push
#define LeftLong				bios_kLeftLong 		// for left button long push
#define CenterLong			bios_kCenterLong 	// for center button long push
#define RightLong				bios_kRightLong 		// for right button long push

#define DisplayFormat 	reg_kDisplayFormat
#define Display					reg_kDisplayInteger

#define Unit						itoa_kUnit
#define	Deci						itoa_kDeci
#define Centi						itoa_kCenti
#define Milli						itoa_kMilli
#define Bool						itoa_kBool
#define TwoDigit				itoa_kTwoDigits
#define Time						itoa_kTime

#define Flash						reg_kFlash
#define NoFlash					0

#endif
