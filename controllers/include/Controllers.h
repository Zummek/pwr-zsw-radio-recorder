#ifndef CONTROLLERS_h
#define CONTROLLERS_h

#define AVG_OF_N_SAMPLES 12

#define FREQ_POT_PIN A1
#define FREQ_POT_MIN 30
#define FREQ_POT_MAX 1020

#include <Arduino.h>

class Controllers
{
public:
  static int avgFreqPotVal;

  static void readAndProcess();
  static void readFreqPot();
  static int getFormatedFreq();

private:
  static int freqPotValesTotal;
  static int freqPotVales[AVG_OF_N_SAMPLES];
  static int freqPotValuesIndex;

  static int round10(int num);
};

#endif
