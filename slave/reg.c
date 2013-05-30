#include <string.h>

#include <avr/io.h>
#include <avr/eeprom.h>

#include "reg.h"
#include "bios.h"
#include "seg.h"
#include "adc.h"
#include "i2c.h"
#include "itoa.h"
#include "ntcPhilips.h"
#include "ntcHoneywell.h"
#include "uif.h"
#include "pwm.h"
#include "ee.h"


typedef struct {
	getHAL 	getValue;
	int reference;
} Register;


Register map[] = {
		{i2c, 0},
		{ntcHoneywell, 1},
		{adc, 1},
		{0}
};

enum{
	InputI2C = 0,

};


void reg_init(void)
{
	uif()->init(0);
	adc()->init(0);
	i2c()->init(0);
	pwm()->init(0);

}


uint8_t reg_setRegister(uint16_t registerAddress, int16_t registerValue)
{
	if(registerAddress == reg_kDisplayFormat){
		uif()->setValue(uif_kDisplayFormat, registerValue);
		return reg_kOK;
	}
	
	if(registerAddress == reg_kDisplayInteger){
		uif()->setValue(uif_kDisplayInteger, registerValue);
		return reg_kOK;
	}
	
	if(registerAddress == reg_kDisplayString){	
		uif()->setValue(uif_kDisplayString, registerValue);
		return reg_kOK;
	}
	
	if(registerAddress == reg_kDisplayString + 1){	
		uif()->setValue(uif_kDisplayString + 1, registerValue);
		return reg_kOK;
	}

	if(registerAddress == reg_kDisplayString + 2){
		uif()->setValue(uif_kDisplayString + 2, registerValue);
		return reg_kOK;
	}

	if(registerAddress == reg_kDisplayString + 3){
		uif()->setValue(uif_kDisplayString + 3, registerValue);
		return reg_kOK;
	}

	
	if(registerAddress == reg_kOutput1){
		pwm()->setValue(pwm_kOutput1, registerValue);
		return reg_kOK;
	}
	
	if(registerAddress == reg_kOutput2){
		pwm()->setValue(pwm_kOutput2, registerValue);
		return reg_kOK;
	}
	
	if(registerAddress >= reg_kEEPROM){
		ee()->setValue(registerAddress - reg_kEEPROM, registerValue);
		return reg_kOK;
	}

	return reg_kAddressInvalid;
}


uint8_t reg_getRegister(uint16_t registerAddress, int16_t* registerValue)
{
	if(registerAddress == reg_kKeypad){
		*registerValue = uif()->getValue(uif_kKeypad);
		return reg_kOK;
	}
	if(registerAddress == reg_kInput1){
		*registerValue = adc()->getValue(adc_kInput1);
		return reg_kOK;
	}
	if(registerAddress == reg_kInput2){
		*registerValue = adc()->getValue(adc_kInput2);
		return reg_kOK;
	}
	if(registerAddress == reg_kInputI2C){
		*registerValue = i2c()->getValue(i2c_kInput);
		return reg_kOK;
	}
	if(registerAddress == reg_kPhilips1){
		*registerValue = ntcPhilips()->getValue(adc_kInput1);
		return reg_kOK;
	}
	if(registerAddress == reg_kPhilips2){
		*registerValue = ntcPhilips()->getValue(adc_kInput2);
		return reg_kOK;
	}
	if(registerAddress == reg_kHoneywell1){
		*registerValue = ntcHoneywell()->getValue(adc_kInput1);
		return reg_kOK;
	}
	if(registerAddress == reg_kHoneywell2){
		*registerValue = ntcHoneywell()->getValue(adc_kInput2);
		return reg_kOK;
	}
	if(registerAddress == reg_kCounter1){
		*registerValue = adc()->getValue(adc_kCounter1);
		return reg_kOK;
	}
	if(registerAddress == reg_kCounter2){
		*registerValue = adc()->getValue(adc_kCounter2);
		return reg_kOK;
	}
	if(registerAddress == reg_kFrequency1){
		*registerValue = adc()->getValue(adc_kFrequency1);
		return reg_kOK;
	}	
	if(registerAddress == reg_kFrequency2){
		*registerValue = adc()->getValue(adc_kFrequency2);
		return reg_kOK;
	}	
	if(registerAddress >= reg_kEEPROM){
		*registerValue = ee()->getValue(registerAddress - reg_kEEPROM);
		return reg_kOK;
	}
	return reg_kAddressInvalid;
}


uint8_t reg_update(void)
{
	static uint8_t seconds = 0;
	

	
	if(seconds != bios_seconds){
		seconds = bios_seconds;
		ntcPhilips()->update(adc_kInput1);
		ntcPhilips()->update(adc_kInput2);
		i2c()->update(i2c_kInput);

	}
	return reg_kOK;
}

