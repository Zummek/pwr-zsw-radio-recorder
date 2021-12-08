#include "AppRadio.hpp"

#include <string.h>

RDA5807M AppRadio::radio;
RDSParser AppRadio::rds;
RADIO_FREQ AppRadio::freq;
int AppRadio::volume;
char AppRadio::rdsTemp[RADIO_RDS_BUFF_SIZE];
char AppRadio::rdsText[RADIO_RDS_BUFF_SIZE];
onRDSChangeFuncType AppRadio::rdsCallback = []() {};

void AppRadio::init(onRDSChangeFuncType func)
{
  rdsCallback = func;

  radio.init();
  radio.setMono(true);
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
    Lcd::displayFrequency(newFreq);

    _resetSavedRDS();
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
  delay(500);
  int newFreq = AppRadio::radio.getFrequency();
  Lcd::displayFrequency(newFreq);
  Serial.print("Set frequency: ");
  Serial.println(newFreq);

  _resetSavedRDS();
}

void AppRadio::seekDown() {
  Serial.println("Seek down");
  AppRadio::radio.seekDown();
  delay(500);
  int newFreq = AppRadio::radio.getFrequency();
  Lcd::displayFrequency(newFreq);
  Serial.print("Set frequency: ");
  Serial.println(newFreq);

  _resetSavedRDS();
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

void AppRadio::checkRDS()
{
  radio.checkRDS();
}

void AppRadio::_processRDS(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4)
{
  rds.processData(block1, block2, block3, block4);
}

void AppRadio::_processRDSServiceName(char* name)
{
  Serial.print("Service name: ");
  Serial.println(name);
}

void AppRadio::_processRDSText(char* text)
{
  Serial.print("Text: ");
  Serial.println(text);

  Lcd::displayRDS(text);

  if (!strchr(text, '-')) return;

  strncpy(rdsTemp, text, RADIO_RDS_BUFF_SIZE);
  static constexpr uint8_t lastBuffPos =
    RADIO_RDS_BUFF_SIZE - 1;
  if (strlen(text) > lastBuffPos)
  {
    rdsTemp[lastBuffPos] = '\0';
  }

  if (strcmp(rdsTemp, rdsText))
  {
    strncpy(rdsText, rdsTemp, RADIO_RDS_BUFF_SIZE);
    (*rdsCallback)();
  }
}

void AppRadio::_resetSavedRDS()
{
  strncpy(rdsText, "Unknown", RADIO_RDS_BUFF_SIZE);
  (*rdsCallback)();
}
