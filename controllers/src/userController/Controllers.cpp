#include "Controllers.h"
<<<<<<< HEAD
#include <IRremote.h>
=======
#include "AppRadio.h"
#include "Lcd.h"
>>>>>>> Move displaying freq to Controllers

Ewma Controllers::freqFilter(0.05);
bool Controllers::blockManuallyFrequency = false;
int Controllers::frequency;
Ewma Controllers::volumeFilter(0.5);
bool Controllers::bloackManuallyVolume = false;
int Controllers::volume;
struct DebounceButton Controllers::muteBtn;
bool Controllers::allowIRRepeat = false;

void Controllers::init()
{
  pinMode(MUTE_BTN_PIN, INPUT);
  IrReceiver.begin(IR_RECEIVER_PIN);
}

void Controllers::readAndProcess()
{
  readFrequency();
  AppRadio::setFrequency(getFormatedFreq());
  readVolume();
  AppRadio::setVolume(getFormatedVolume());
  if (readMute())
    AppRadio::switchMute();
  decodeIR();
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

bool Controllers::readMute()
{
  int reading = digitalRead(MUTE_BTN_PIN);

  if (reading != muteBtn.lastState)
  {
    muteBtn.lastDebounceTime = millis();
  }

  if ((millis() - muteBtn.lastDebounceTime) > muteBtn.debounceDelay)
  {
    if (reading != muteBtn.state)
    {
      muteBtn.state = reading;

      if (muteBtn.state == HIGH)
      {
        muteBtn.lastState = reading;
        return true;
      }
    }
  }

  muteBtn.lastState = reading;

  return false;
}

void Controllers::decodeIR()
{
  if (IrReceiver.decode()) {
    if (allowIRRepeat || !(IrReceiver.decodedIRData.flags & (IRDATA_FLAGS_IS_AUTO_REPEAT | IRDATA_FLAGS_IS_REPEAT)))
    {
      allowIRRepeat = false;

      switch (IrReceiver.decodedIRData.command)
      {
      case 0xC:
        Serial.println("START RECORDING");
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
        Serial.println("NONE");
        break;

      default:
        IrReceiver.printIRResultShort(&Serial);
      }
    }
    IrReceiver.resume();
  }
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