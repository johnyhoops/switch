/*
  Switch Library - Demo
 
 This sketch demonstrates the Switch API
 
*/

// include the library code:
#include "led.h"
#include "menu.h"
#include "ntcHoneywell.h"
#include "ntcPhilips.h"
#include "pwm.h"
#include "rtc.h"
#include "eeprom.h"
#include "device.h"
#include "format.h"
#include "pressure.h"

// EEPROM memory map
typedef enum {
	kEEPROM_Setp = 0,
	kEEPROM_Ton,
	kEEPROM_Toff
} ee_memoryMap;

// IO channels
typedef enum {
	kIO_Channel_1 = 1,
	kIO_Channel_2,
} io_channels;

// Set up the devices
// All devices (eeprom, inputs. outputs, real time clock) must be defined here defined

// eeprom devices
Device ontime = Device(eeprom, kEEPROM_Ton);
Device offtime = Device(eeprom, kEEPROM_Toff);
Device setpoint = Device(eeprom, kEEPROM_Setp);

// input devices
Device tubTemperature = Device(ntcPhilips, kIO_Channel_1);
Device tubPressure = Device(pressure, 0);

// output devices
Device heating = Device(pwm, kIO_Channel_1);
Device cooling = Device(pwm, kIO_Channel_2);

// real time clock device
Device clock = Device(rtc);

void setup() {
  // initialise the display
  led.begin();

  // populate the menu
  // real time clock
  clock.setLabel("cloc");
  clock.setRange(rtc_kMinTime, rtc_kMaxTime);
  clock.setFormat(format_Time);
  menu.add(clock);

  tubPressure.setLabel("pres");
  tubPressure.setFormat(format_OneDigit);
  menu.add(tubPressure);

  // outputs
  heating.setLabel("heat");
  heating.setRange(0, 1);
  heating.setFormat(format_Bool);
  menu.add(heating);

  cooling.setLabel("cool");
  cooling.setRange(0, 1);
  cooling.setFormat(format_Bool);
  menu.add(cooling);

  // inputs
  tubTemperature.setLabel("tub");
  tubTemperature.setFormat(format_Deci);
  menu.add(tubTemperature);

  // eeprom data
  ontime.setLabel("ton");
  ontime.setRange(rtc_kMinTime, rtc_kMaxTime);
  ontime.setFormat(format_Time);
  menu.add(ontime);

  offtime.setLabel("toff");
  offtime.setRange(rtc_kMinTime, rtc_kMaxTime);
  offtime.setFormat(format_Time);
  menu.add(offtime);

  setpoint.setLabel("setp");
  setpoint.setRange(0, 500);
  setpoint.setFormat(format_Deci);
  menu.add(setpoint);
}

void loop() {

  // update menu
  char buffer[menu_kBufferSize];
  char key = led.getKey();
  menu.update(key, buffer);
  led.print(buffer);

  // update logic
  if(clock.getValue() == ontime.getValue()) heating.setValue(1);
  if(clock.getValue() == offtime.getValue()) heating.setValue(0);
  if(tubTemperature.getValue() > setpoint.getValue()) cooling.setValue(0);
  if(tubTemperature.getValue() < setpoint.getValue()) cooling.setValue(1);

  // slow the loop down with a delay
  delay(100);
}

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
