#ifndef CONTROLLERS_h
#define CONTROLLERS_h

#define AVG_OF_N_SAMPLES 15

#define FREQ_POT_PIN A1
#define FREQ_POT_MIN 30
#define FREQ_POT_MAX 1020

#define VOLUME_POT_PIN A0
#define VOLUME_POT_MIN 30
#define VOLUME_POT_MAX 1020

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
  static struct AvgController volumePot;

  static void readAndProcess();
  static void readFreqPot();
  static void readVolumePot();
  static int getFormatedFreq();
  static int getFormatedVolume();

private:
  static int round10(int num);
};

#endif
