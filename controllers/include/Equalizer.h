#ifndef EQUALIZER_h
#define EQUALIZER_h

#include "Lcd.h"

#define EQUALIZER_STROBE 3
#define EQUALIZER_RESET 4
#define EQUALIZER_ANALOG A3
 
class Equalizer
{
private:
  static int *bandValues;
public:
  static void init();
  static void process();
};

#endif