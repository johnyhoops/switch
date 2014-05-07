#include <avr/wdt.h>

#include "switch.h"

// declare global variables
int16_t Ttank;
int16_t Tsolar;
int16_t Tdiff;

int16_t Ptank = 0;
int16_t Psolar = 0;
int16_t Clock;
int16_t tClock = -1;

// declare menus
MenuItem menuItems[] = {
  {"Ttnk", Deci, ReadOnly, &Ttank},
  {"Tsol", Deci, ReadOnly, &Tsolar},
  {"Diff", Deci, ReadOnly, &Tdiff},
  {"Ptnk", Bool, ReadOnly, &Ptank},
  {"Psol", Bool, ReadWrite, &Psolar},
  {"clok", Time, ReadOnly, &Clock},
  
  
  //{"Set", Deci, ReadWrite, &setPoint},
  //{"Hyst", Deci, ReadWrite, &hysteresis},
  {0}   // must end with a zero to indicate end of list
};

void setup() 
{
    wdt_enable(WDTO_2S);
    Switch.begin();
    Switch.addMenu(menuItems);
    //Switch.set(RTC, 0);
    Serial.begin(9600);
}

void loop() {
  // update measurements
  Ttank = Switch.get(Philips1);
  Tsolar = Switch.get(Philips2);
  Tdiff = Tsolar - Ttank;
  
  tClock = Switch.get(RTC) & 0xfffc; // changes every 4 mins
  if(Clock != tClock){
    Clock = tClock; 
    Serial.print(Ttank);
    Serial.print("\t");
    Serial.println(Tsolar);
    
    // update logic every 4 mins
    if((Tsolar < 600) && (Tdiff < 100)) Psolar = 0;
    
    if((Clock == 0)|| (Clock == 60 * 6)|| (Clock == 60 * 12) || (Clock == 60 * 18)) Psolar = 1;
    if(Tdiff > 200) Psolar = 1;
     
    if((Tdiff > 50) && (Ttank < 700) && (Psolar != 0)) Ptank = 1; else Ptank = 0;
  }
  
  // update output
  if(Ptank != 0) Switch.set(Output1, pwm_On); else Switch.set(Output1, pwm_Off);
  if(Psolar != 0) Switch.set(Output2, pwm_On); else Switch.set(Output2, pwm_Off);
    
  // update switch
  Switch.update();

  // kick the dog
  wdt_reset();
  
}
