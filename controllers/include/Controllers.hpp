#ifndef CONTROLLERS_H
#define CONTROLLERS_H

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
#include "AppRadio.hpp"

struct DebounceButton
{
  bool state;
  bool lastState;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
};

enum class UserAction
{
  startBtnPress,
  stopBtnPress,
  idle
};

class Controllers
{
public:
  static int frequency;
  static bool blockManuallyFrequency;
  static Ewma freqFilter;
  static int volume;
  static bool bloackManuallyVolume;
  static Ewma volumeFilter;
  static struct DebounceButton muteBtn;
  static bool allowIRRepeat;

  static void init();
  static UserAction readAndProcess();
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
