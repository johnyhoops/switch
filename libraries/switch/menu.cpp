#include <string.h>

#include "menu.h"

enum{
	menu_kShowLabel	= 0,
	menu_kShowValue,
	menu_kShowSetting
};


const char CLRSCR[] = 	"\x1b" "[2J";
const char FLASHON[] = "\x1b" "[5m";
const char FLASHOFF[] = "\x1b" "[0m";
const char CRLF[] = 	"\r\n";


/*
int16_t menu_adj(int16_t value, int16_t delta, Device* Device)
{
	if(value > Device->max) value = Device->max;
	if(value < Device->min) value = Device->min;

	if(delta > 0){
		if ((value + delta) > Device->max)
			while ((value - delta) >= Device->min) value -= delta;
		else
			value += delta;
	}

	if(delta < 0){
		if ((value + delta) < Device->min)
			while ((value - delta) <= Device->max) value -= delta;
		else
			value += delta;
	}
	return value;
}
*/

Menu::Menu()
{
	device_counter = 0;
	device_index = 0;
	status = menu_kShowLabel;
}

void Menu::add(Device& deviceEntry)
{
	if(device_counter >= MENU_MAX_ITEM) return;
	deviceArray[device_counter] = &deviceEntry;
	device_counter++;
}

void Menu::update(char pressedKey, char* displayBuffer)
{
	if(device_counter == 0){
		return;
	}

	// if key pressed update state and menu value
	Device* device = deviceArray[device_index];
	if(status == menu_kShowValue){
		if((pressedKey == kSelLong) && device->isWriteable()){
			status = menu_kShowSetting;
		} else if(pressedKey != 0) {
			status = menu_kShowLabel;
		}
		value = device->getValue();
	} else if(status == menu_kShowSetting){
		if((pressedKey == kSelShort) || (pressedKey == kSelLong)){
			device->setValue(value);
			status = menu_kShowValue;
		}
		if(pressedKey == kDecShort){
			value = device->validateValue(value - 1);
		}
		if(pressedKey == kIncShort){
			value = device->validateValue(value + 1);
		}
		if(pressedKey == kDecLong){
			value = device->validateValue(value - 10);
		}
		if(pressedKey == kIncLong){
			value = device->validateValue(value + 10);
		}
	} else {
		status = menu_kShowLabel;
		if((pressedKey == kIncShort) || (pressedKey == kIncLong)){
			device_index++;
			if (device_index == device_counter) device_index = 0;
		}
		if((pressedKey == kDecShort) || (pressedKey == kDecLong)){
			if (device_index == 0) device_index = device_counter;
			device_index--;

		}
		if((pressedKey == kSelShort) || (pressedKey == kSelLong)){
			status = menu_kShowValue;
			value = device->getValue();
		}
	}

	// update display with new information
	char buffer[menu_kBufferSize];
	strcpy(displayBuffer, CLRSCR);
	device = deviceArray[device_index];
	if(status == menu_kShowValue){
		device->getString(buffer);
		strcat(displayBuffer, buffer);
		strcat(displayBuffer, CRLF);
	} else if(status == menu_kShowSetting){
		strcat(displayBuffer, FLASHON);
		device->getString(buffer, value);
		strcat(displayBuffer, buffer);
		strcat(displayBuffer, FLASHOFF);
		strcat(displayBuffer, CRLF);
	} else {
		strcat(displayBuffer, device->getLabel());
		strcat(displayBuffer, CRLF);
	}
}

Menu menu;


