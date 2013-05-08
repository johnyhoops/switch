#ifndef led_h
#define led_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "Print.h"

class Led : public Print {
public:
  Led();
  void begin();
  virtual size_t write(uint8_t);
  int16_t getKey();
private:
};

extern Led led;

#endif


