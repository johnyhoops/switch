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
#define Counter1				reg_kCounter1
#define Frequency1			reg_kFrequency1
#define Counter2				reg_kCounter2
#define Frequency2			reg_kFrequency2
#define Philips1				reg_kPhilips1
#define Philips2				reg_kPhilips2
#define Honeywell1			reg_kHoneywell1
#define Honeywell2			reg_kHoneywell2

#define LeftShort				bios_kLeftShort 		// for left button short push
#define CenterShort			bios_kCenterShort 	// for center button short push
#define RightShort			bios_kRightShort 		// for right button short push
#define LeftLong				bios_kLeftLong 			// for left button long push
#define CenterLong			bios_kCenterLong 		// for center button long push
#define RightLong				bios_kRightLong 		// for right button long push
#define NoKey						0 									// no key pressed

#define ReadOnly				0
#define ReadWrite				1

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

#define On							255
#define Off							0

#endif
