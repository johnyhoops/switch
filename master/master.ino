#include "slave.h"



void setup()
{
  Slave device1 = Slave(1); 
  
}

void loop()
{
 uint16_t reg; 
 device1.read(&reg, 8, 1);
  
}
