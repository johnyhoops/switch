#include "slave.h"
#include "led.h"

Switch switch_0(ThisSwitch);

Switch switch_1(1);
Switch switch_2(2);
Switch switch_3(3);
Switch switch_4(4);
Switch switch_5(5);
Switch switch_6(6);


uint16_t reg = 'A'; 

void setup()
{
  led.begin();
  led.println("OK");
}

void loop()
{
  
  
  switch_1.set(Output1, 255);
  
  temperature = switch1.get(Input1);
  
  pressure = switch1.get(I2C);
  
  button = switch_0.get(Keypad);
  
  switch_1.set(Display, -12);
  switch_1.set(DisplayFormat, Unity/Deci/Centi/Milli/HrMin/Bool | Flash);
  switch_1.set(Display, "Temp");
  
  
  
  
  
 
 //uint8_t error = switch_1.read(&reg, 8, 1);
 uint8_t error = switch_1.writeRegisters(&reg, 0, 1);
  if(error != kOK) led.println(error); else led.println("OK");
 delay(1000);
 reg++;
error = switch_1.writeRegisters(&reg, 1, 1);
 if(error != kOK) led.println(error); else led.println("OK");
 delay(1000);
 reg++;
error = switch_1.writeRegisters(&reg, 2, 1);
 if(error != kOK) led.println(error); else led.println("OK");
 delay(1000);
 reg++;
 error = switch_1.writeRegisters(&reg, 3, 1); 
 if(error != kOK) led.println(error); else led.println("OK");
 delay(1000);
 reg++;
 if(reg > 'Z') reg = 'A';
}
