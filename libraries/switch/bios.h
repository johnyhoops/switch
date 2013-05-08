#ifndef BIOS_H
#define BIOS_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

extern uint8_t 	bios_segment[8];
extern uint8_t	bios_seconds;
extern uint8_t	bios_minutes;
extern uint8_t	bios_hours;
extern uint8_t	bios_days;
extern uint8_t	bios_ticks;

void bios_init(void);

// bios_getKey returns...
#define kLeftShort 	0x02 	// for left button short push
#define kCenterShort 	0x04 	// for center button short push
#define kRightShort 	0x08 	// for right button short push
#define kLeftLong 	0x20 	// for left button long push
#define kCenterLong 	0x40 	// for center button long push
#define kRightLong 	0x80 	// for right button long push

extern uint8_t bios_getKey(void);

#if defined(__cplusplus)
}
#endif

#endif
