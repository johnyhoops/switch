
//pump control program 1.1
//update 3/6/13


#include <avr/wdt.h>

#include "switch.h"

// declare global variables
int mode;
int pressSensor;
int tempSensor;
int tempSetPoint = 210;

// declare menus
MenuItem menuItems[] = {
  {"mode", Unit, ReadWrite, &mode},
  {"cal", Unit, ReadOnly, &pressSensor},
  {"temp", Deci, ReadOnly, &tempSensor},
  {"Set", Deci, ReadWrite, &tempSetPoint},
  {0}   // must end with a zero to indicate end of list
};

void setup() 
{
    wdt_enable(WDTO_2S);
    Switch.begin();
    Switch.addMenu(menuItems);
}

void loop() {
  // update measurements
  int potValue = Switch.get(Input1);
  tempSensor = Switch.get(Philips2);
  pressSensor = Switch.get(InputI2C);
  
  // update logic
  static int motorStatus = 0;
  static int dutyCycle = 0;
  static int demandPWM = 0;
  
  if (demandPWM > dutyCycle){
    ++ dutyCycle;
  }
  if (demandPWM < dutyCycle){
    -- dutyCycle;
  }
    
  switch(motorStatus){
    case 0:
    
    break;
    
    case 1:
    
    break;
    
    case 2:
    
    break;
  }

  // update switch
  Switch.update();
  
  // kick the dog
  wdt_reset();
  
}
