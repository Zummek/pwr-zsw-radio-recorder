#include "AppRadio.h"

RDA5807M AppRadio::radio;
RDSParser AppRadio::rds;
RADIO_FREQ AppRadio::freq;
int AppRadio::volume;

void AppRadio::init()
{
  radio.init();
  radio.setMono(false);
  radio.setMute(false);
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

    Lcd::displayFrequency();
  }
}

void AppRadio::setVolume(int newVolume)
{
  if (volume != newVolume)
  {
    volume = newVolume;
    radio.setVolume(newVolume);
    Serial.print("Set volume: ");
    Serial.println(newVolume);
  }
}

void AppRadio::switchMute()
{
  radio.setMute(!radio.getMute());
  Serial.print("Switch mute: ");
  Serial.println(radio.getMute());
}

void AppRadio::seekUp() {
  Serial.println("Seek up");
  AppRadio::radio.seekUp();
  Serial.println(AppRadio::radio.getFrequency());
  delay(500);
  Serial.println(AppRadio::radio.getFrequency());
}

void AppRadio::seekDown() {
  Serial.println("Seek down");
  AppRadio::radio.seekDown();
  delay(500);
  Serial.println(AppRadio::radio.getFrequency());
}

void AppRadio::increaseRemoteVolume() {
  uint8_t tempVolume = radio.getVolume();

  if (tempVolume < RADIO_MAX_VOLUME) {
    radio.setVolume(++tempVolume);
    Serial.print("Set volume: ");
    Serial.println(tempVolume);
  }
  else
    Serial.println("Volume is already max");
}

void AppRadio::decreaseRemoteVolume() {
  uint8_t tempVolume = radio.getVolume();

  if (tempVolume > 0) {
    radio.setVolume(--tempVolume);
    Serial.print("Set volume: ");
    Serial.println(tempVolume);
  }
  else
    Serial.println("Volume is already min");
}

void AppRadio::_processRDS(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4)
{
  rds.processData(block1, block2, block3, block4);
}

void AppRadio::_processRDSServiceName(char* name)
{
  Serial.print("Service name: ");
  Serial.println(name);
  // TODO: Do something about this later
}

void AppRadio::_processRDSText(char* text)
{
  Serial.print("Text: ");
  Serial.println(text);
  // TODO: Do something about this later
}
