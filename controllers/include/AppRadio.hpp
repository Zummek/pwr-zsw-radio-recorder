#ifndef APPRADIO_H
#define APPRADIO_H

#define RADIO_MAX_VOLUME 15

#include <radio.h>
#include <RDA5807M.h>
#include <RDSParser.h>

#include "Lcd.hpp"

class AppRadio
{
public:
  static RDA5807M radio;
  static RDSParser rds;
  static RADIO_FREQ freq;
  static int volume;

  static void init();
  static void getFrequency();
  static void setFrequency(RADIO_FREQ freq);
  static void setVolume(int volume);
  static void switchMute();
  static void seekUp();
  static void seekDown();
  static void increaseRemoteVolume();
  static void decreaseRemoteVolume();

private:
  static void _processRDS(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4);
  static void _processRDSServiceName(char* name);
  static void _processRDSText(char* text);
};

#endif
