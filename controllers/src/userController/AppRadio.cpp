#include "AppRadio.h"

RDA5807M AppRadio::radio;
RDSParser AppRadio::rds;
RADIO_FREQ AppRadio::freq;

void AppRadio::init()
{
  radio.init();
  radio.setMono(false);
  radio.setMute(false);
  radio.setVolume(RADIO_DEFAULT_VOLUME);
  radio.attachReceiveRDS(_processRDS);
  rds.attachServicenNameCallback(_processRDSServiceName);
  rds.attachTextCallback(_processRDSText);
}

void AppRadio::setFrequency(RADIO_FREQ newFreq)
{
  if (freq != newFreq)
  {
    freq = newFreq;
    radio.setBandFrequency(RADIO_BAND_FM, newFreq);
    // TODO: temporary for testing without display
    Serial.print("Set frequency: ");
    Serial.println(newFreq);
  }
}

void AppRadio::_processRDS(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4)
{
  rds.processData(block1, block2, block3, block4);
}

void AppRadio::_processRDSServiceName(char *name)
{
  Serial.print("Service name: ");
  Serial.println(name);
  // TODO: Do something about this later
}

void AppRadio::_processRDSText(char *text)
{
  Serial.print("Text: ");
  Serial.println(text);
  // TODO: Do something about this later
}
