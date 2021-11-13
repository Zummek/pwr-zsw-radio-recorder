#ifndef CONTROLLERS_h
#define CONTROLLERS_h

#define FREQ_POT_PIN A1
#define FREQ_POT_MIN 30
#define FREQ_POT_MAX 1020

#define VOLUME_POT_PIN A0
#define VOLUME_POT_MIN 30
#define VOLUME_POT_MAX 1020

#include <Arduino.h>
#include <Ewma.h>
#include "AppRadio.h"

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
  static int frequency;
  static Ewma freqFilter;
  static int volume;
  static Ewma volumeFilter;

  static void readAndProcess();
  static void readFrequency();
  static void readVolume();
  static int getFormatedFreq();
  static int getFormatedVolume();

private:
  static int round10(int num);
};

#endif
