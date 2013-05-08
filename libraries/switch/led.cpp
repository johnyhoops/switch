#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Arduino.h"

#include "bios.h"
#include "menu.h"
#include "map.h"

#include <led.h>

#define SEGMENTDP		1
#define kSegmentBufferSize	8
#define kNumberOfDigits		4
#define ESC			0x1B


uint8_t getMap(char asciicode);


Led::Led()
{

}

void Led::begin(){
	bios_init();				// enable rtc/display/buttons
}

int16_t Led::getKey()
{
	uint8_t key = bios_getKey();

	if(key == kRightLong) return kIncLong;
	if(key == kLeftLong) return kDecLong;
	if(key == kCenterLong) return kSelLong;
	if(key == kRightShort) return kIncShort;
	if(key == kLeftShort) return kDecShort;
	if(key == kCenterShort) return kSelShort;
	return kNoKey;
}

enum {
	kGroundState = 0,
	kEscapeReceived,
	kBracketReceived,
	kDigitReceived
};

size_t Led::write(uint8_t data) {
	static uint8_t state = kGroundState;
	static uint8_t digit;
	static uint8_t flash = false;
	static uint8_t segmentBuffer[kSegmentBufferSize];
	static uint8_t segmentBufferIndex = 0;

	// check for 
	if(state ==  kEscapeReceived){
		if(data == '['){
			state = kBracketReceived;
		} else {
			state = kGroundState;
		}
	} else if(state ==  kBracketReceived){
		if(isdigit(data)){
			digit = data;
			state = kDigitReceived;
		} else {
			state = kGroundState;
		}
	} else if(state ==  kDigitReceived){
		if(isalpha(data)){
			if((data == 'J') && (digit == '2')) {
				segmentBufferIndex = 0;
				memset(segmentBuffer, 0, kSegmentBufferSize);
			}
			if((data == 'm') && (digit == '0')) flash = false;
			if((data == 'm') && (digit == '5')) flash = true;
			state = kGroundState;
		}
	} else {
		if(data == ESC){
			state = kEscapeReceived;
			digit = 0;	
		} else { 
			// process non-escaped characters
			if(data == '\n'){
				memcpy(bios_segment, segmentBuffer, kSegmentBufferSize);
			} else if(isprint(data)){
				if (data == '.'){
					if(segmentBufferIndex != 0) segmentBufferIndex--;
					data = segmentBuffer[segmentBufferIndex] | SEGMENTDP;
				} else {
					data = getMap(data);
				}
				
				// update segments
				segmentBuffer[segmentBufferIndex] = data;
				if(flash == false) segmentBuffer[segmentBufferIndex + kNumberOfDigits] = data;
				if (++segmentBufferIndex > 3) segmentBufferIndex = 0;
			}			
		}
	}
	return 1;
}

Led led;

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//


