#include "Controllers.hpp"
#include <IRremote.h>
#include "AppRadio.hpp"

Ewma Controllers::freqFilter(0.05);
bool Controllers::blockManuallyFrequency = false;
int Controllers::frequency;
Ewma Controllers::volumeFilter(0.5);
bool Controllers::bloackManuallyVolume = false;
int Controllers::volume;
PushButton Controllers::muteBtn(MUTE_BTN_PIN);
PushButton Controllers::startRecBtn(START_REC_BTN_PIN);
PushButton Controllers::stopRecBtn(STOP_REC_BTN_PIN);
bool Controllers::allowIRRepeat = false;

void Controllers::init()
{
  muteBtn.init();
  startRecBtn.init();
  stopRecBtn.init();
  IrReceiver.begin(IR_RECEIVER_PIN);
}

RecAction Controllers::readAndProcess()
{
  readFrequency();
  AppRadio::setFrequency(getFormatedFreq());
  readVolume();
  AppRadio::setVolume(getFormatedVolume());
  if (muteBtn.isPressed())
    AppRadio::switchMute();
  RecAction action = decodeIR();
  if (startRecBtn.isPressed())
    action = RecAction::startRecAction;
  if (stopRecBtn.isPressed())
    action = RecAction::stopRecAction;
  return action;
}

void Controllers::readFrequency()
{
  int tempFreq = freqFilter.filter(analogRead(FREQ_POT_PIN));
  if (tempFreq != frequency)
  {
    frequency = tempFreq;
    blockManuallyFrequency = false;
  }
}

void Controllers::readVolume()
{
  int tempVolume = volumeFilter.filter(analogRead(VOLUME_POT_PIN));
  if (tempVolume != volume)
  {
    volume = tempVolume;
    bloackManuallyVolume = false;
  }
}

RecAction Controllers::decodeIR()
{
  RecAction outAction = RecAction::idle;
  if (IrReceiver.decode()) {
    if (allowIRRepeat || !(IrReceiver.decodedIRData.flags & (IRDATA_FLAGS_IS_AUTO_REPEAT | IRDATA_FLAGS_IS_REPEAT)))
    {
      allowIRRepeat = false;

      switch (IrReceiver.decodedIRData.command)
      {
      case 0xC:
        outAction = RecAction::startRecAction;
        break;
      case 0x20:
        blockManuallyFrequency = true;
        AppRadio::seekUp();
        allowIRRepeat = true;
        break;
      case 0x21:
        blockManuallyFrequency = true;
        AppRadio::seekDown();
        allowIRRepeat = true;
        break;
      case 0xD:
        AppRadio::switchMute();
        break;
      case 0x11:
        bloackManuallyVolume = true;
        AppRadio::decreaseRemoteVolume();
        allowIRRepeat = true;
        break;
      case 0x10:
        bloackManuallyVolume = true;
        AppRadio::increaseRemoteVolume();
        allowIRRepeat = true;
        break;
      case 0xB:
        outAction = RecAction::stopRecAction;
        break;
      default:
        IrReceiver.printIRResultShort(&Serial);
      }
    }
    IrReceiver.resume();
  }
  return outAction;
}

int Controllers::getFormatedFreq()
{
  int freq = map(frequency, FREQ_POT_MIN, FREQ_POT_MAX, 8750, 10800);

  if (freq < 8750)
    freq = 8750;
  else if (freq > 10800)
    freq = 10800;

  return round10(freq);
}

int Controllers::getFormatedVolume()
{
  return map(volume, VOLUME_POT_MIN, VOLUME_POT_MAX, 0, 15);
}

int Controllers::round10(int num)
{
  return (num + 5) / 10 * 10;
}
