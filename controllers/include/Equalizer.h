#ifndef EQUALIZER_h
#define EQUALIZER_h

#include "Lcd.h"

#define EQUALIZER_STROBE 3
#define EQUALIZER_RESET 4
#define EQUALIZER_ANALOG A3
#define EQUALIZER_BAUNDS_COUNT 7

class Equalizer
{
private:
  static int bandValues[EQUALIZER_BAUNDS_COUNT];
public:
  static void init();
  static void process();
};

#endif