#ifndef CONTROLLERS_h
#define CONTROLLERS_h

#define FREQ_POT_PIN A1
#define FREQ_POT_MIN 30
#define FREQ_POT_MAX 1020

#define VOLUME_POT_PIN A0
#define VOLUME_POT_MIN 30
#define VOLUME_POT_MAX 1020

#define MUTE_BTN_PIN 2

#define IR_RECEIVER_PIN 12

#include <Arduino.h>
#include <Ewma.h>
#include "AppRadio.h"

struct DebounceButton
{
  bool state;
  bool lastState;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
};

class Controllers
{
public:
  static int frequency;
  static Ewma freqFilter;
  static int volume;
  static Ewma volumeFilter;
  static struct DebounceButton muteBtn;

  static void init();
  static void readAndProcess();
  static void readFrequency();
  static void readVolume();
  static bool readMute();
  static void decodeIR();
  static int getFormatedFreq();
  static int getFormatedVolume();

private:
  static int round10(int num);
};

#endif
