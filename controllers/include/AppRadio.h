#ifndef APPRADIO_h
#define APPRADIO_h

#define RADIO_DEFAULT_VOLUME 7 // max 15

#include <radio.h>
#include <RDA5807M.h>
#include <RDSParser.h>

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

private:
  static void _processRDS(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4);
  static void _processRDSServiceName(char *name);
  static void _processRDSText(char *text);
};

#endif
