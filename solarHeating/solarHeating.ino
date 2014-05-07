#include <avr/wdt.h>

#include "switch.h"

// declare global variables
int16_t Tin;
int16_t Tout;
int16_t Tsol;
int16_t Diff;
int16_t pumpStatus = 0;
int16_t setPoint = 200;
int16_t hysteresis = 10;

int16_t Clock= 10 * 60;
int16_t tClock;

// declare menus
MenuItem menuItems[] = {
  {"Sol", Deci, ReadOnly, &Tsol},
  {"In", Deci, ReadOnly, &Tin},
  {"out", Deci, ReadOnly, &Tout},

  {"Diff", Deci, ReadOnly, &Diff},
  {"Pump", Bool, ReadWrite, &pumpStatus},
  //{"Set", Deci, ReadWrite, &setPoint},
  //{"Hyst", Deci, ReadWrite, &hysteresis},
  {0}   // must end with a zero to indicate end of list
};

void setup() 
{
    wdt_enable(WDTO_2S);
    Switch.begin();
    Switch.addMenu(menuItems);
    
    Serial.begin(9600);
}

void loop() {
  // update measurements
  Tout = Switch.get(Honeywell1);
  Tin = Switch.get(Honeywell2);
  Tsol = Switch.get(InputI2CNoGain);
  Diff = Tin - Tout;
  
  // update logic
  tClock = Switch.get(RTC) & 0xfff8; // changes every 8 mins
  if(Clock != tClock){
    Clock = tClock;
    if(Diff < 15) pumpStatus = 0;
    if(Tsol - 50 > Tout) pumpStatus = 1;
    
    Serial.print(Tin);
    Serial.print("\t");
    Serial.print(Tout);
    Serial.print("\t");
    Serial.println(Tsol);
  }
  
  // update output
  if(pumpStatus != 0) Switch.set(Output1, pwm_On); else Switch.set(Output1, pwm_Off);

  // update switch
  Switch.update();
  
  // kick the dog
  wdt_reset();
  
}
