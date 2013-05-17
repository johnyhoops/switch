#include "slave.h"

Slave slaveOne(1);

void setup()
{

}

void loop()
{
  static int i = 0;

  int button = slaveOne.get(Keypad);
  
  switch(button){
    case LeftShort:
      slaveOne.set(Display, " .");
      break;
    case CenterShort:
      slaveOne.set(Display, "  .");
      break;
    case RightShort:
      slaveOne.set(Display, "   .");
      break;    
    case LeftLong:
      slaveOne.set(Display, "_");
      break;    
    case CenterLong:
      slaveOne.set(Display, " _");
      break; 
    case RightLong:
      slaveOne.set(Display, "  _");
      break;   
    default:
      slaveOne.set(Display, "");
      break;      
  }
  
  delay(500);
 
  i = i + 25;
  slaveOne.set(Output1, i);
  
  slaveOne.set(DisplayFormat, Flash);
  slaveOne.set(Display, "temp");
  delay(1000);

  int temperature = slaveOne.get(Input1);
  slaveOne.set(DisplayFormat, Deci );
  slaveOne.set(Display, temperature);
  delay(1000);
  
  slaveOne.set(DisplayFormat, NoFlash);
  slaveOne.set(Display, "pres");
  delay(1000);

  int pressure = slaveOne.get(InputI2C);
  slaveOne.set(DisplayFormat, Centi + Flash);
  slaveOne.set(Display, pressure);
  slaveOne.set(DisplayFormat, NoFlash);
  delay(1000);
  
}
