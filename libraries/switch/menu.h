#ifndef MENU_H_
#define MENU_H_

#include "device.h"

#include "Arduino.h"

#define MENU_MAX_ITEM 		8

// keys used in navigation of menu system
#define kIncLong	'A'
#define kDecLong	'B'
#define kSelLong	'S'
#define kIncShort	'a'
#define kDecShort	'b'
#define kSelShort	's'
#define kNoKey		0

#define menu_kBufferSize 32

extern const char CLRSCR[];
extern const char FLASHON[];
extern const char FLASHOFF[];
extern const char CRLF[];


class Menu {
public:
	Menu();
	void add(Device& portEntry);
	void update(char pressedKey, char* displayBuffer);

private:
	Device* deviceArray[MENU_MAX_ITEM];
	uint8_t device_counter;
	uint8_t device_index;
	uint8_t status;
	int16_t value;
};

extern Menu menu;


#endif /* MENU_H_ */
