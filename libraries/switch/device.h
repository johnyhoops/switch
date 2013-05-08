#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdint.h>
#include "hal.h"
#include "format.h"

class Device {
public:
	Device(const char* lab, HAL& hw, int16_t ref, int16_t min, int16_t max, uint8_t fmt)
	{
		hal = &hw;
		reference = ref;
		minValue = min;
		maxValue = max;
		format = fmt;
		label = lab;
	}

	Device(HAL& hw, int16_t ref = 0)
	{
		hal = &hw;
		reference = ref;
		minValue = -32768;
		maxValue = 32767;
		format = format_OneDigit;
		label = "";
	}

	void setRange(int16_t min, int16_t max)
	{
		minValue = min;
		maxValue = max;
	}

	void setFormat(uint8_t fmt)
	{
		format = fmt;
	}

	void setLabel(const char* lab)
	{
		label = lab;

	}

	const char* getLabel()
	{
		return label;
	}

	int16_t getValue()
	{
		return this->hal->getValue(this->reference);
	}

	int16_t isWriteable()
	{
		return (this->hal->setValue != 0);
	}

	void setValue(int16_t value)
	{
		this->hal->setValue(this->reference, value);
	}

	void getString(char* buffer, int16_t value)
	{
		format_toString(buffer, value, this->format);
	}

	void getString(char* buffer)
	{
		format_toString(buffer, this->hal->getValue(this->reference), this->format);
	}

	int16_t validateValue(int16_t value)
	{
		if(value > this->maxValue) value = this->minValue;
		if(value < this->minValue) value = this->maxValue;
		return value;
	}

private:
	HAL* hal;
	uint8_t reference;
	int16_t minValue;
	int16_t maxValue;
	uint8_t format;
	const char* label;
};


#endif /* DEVICE_H_ */
