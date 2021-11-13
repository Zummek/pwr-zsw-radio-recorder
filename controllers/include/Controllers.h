#ifndef CONTROLLERS_h
#define CONTROLLERS_h

#define AVG_OF_N_SAMPLES 12

#define FREQ_POT_PIN A1
#define FREQ_POT_MIN 30
#define FREQ_POT_MAX 1020

#include <Arduino.h>

struct AvgController
{
  int avgValue;
  int totalValue;
  int values[AVG_OF_N_SAMPLES];
  int index;
};

class Controllers
{
public:
  static struct AvgController freqPot;

  static void readAndProcess();
  static void readFreqPot();
  static int getFormatedFreq();

private:
  static int round10(int num);
};

#endif
