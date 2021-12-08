#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "PushButton.hpp"

#include <Arduino.h>
#include <Ewma.h>
#include "AppRadio.hpp"

#define FREQ_POT_PIN A1
#define FREQ_POT_MIN 30
#define FREQ_POT_MAX 1020

#define VOLUME_POT_PIN A0
#define VOLUME_POT_MIN 30
#define VOLUME_POT_MAX 1020

#define MUTE_BTN_PIN 2
#define START_REC_BTN_PIN 6
#define STOP_REC_BTN_PIN 7

#define IR_RECEIVER_PIN 12

enum class RecAction
{
  startRecAction,
  stopRecAction,
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
  static PushButton muteBtn;
  static PushButton startRecBtn;
  static PushButton stopRecBtn;
  static bool allowIRRepeat;

  static void init();
  static RecAction readAndProcess();
  static void readFrequency();
  static void readVolume();
  static RecAction decodeIR();
  static int getFormatedFreq();
  static int getFormatedVolume();

private:
  static int round10(int num);
};

#endif
