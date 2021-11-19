#include <Arduino.h>
#include <Wire.h>
#include "AppWire.h"
#include "AppRadio.h"
#include "Controllers.h"
#include "Lcd.h"

void setup()
{
  // Setup serial for debug
  Serial.begin(115200);
  Serial.println("\nHello - User controller");

  // Setup the I2C Bus as Master
  Wire.begin();

  Controllers::init();
  AppRadio::init();

  Lcd::init();
  Lcd::write(0, 0, "FREQ:");
}

void loop()
{
  AppRadio::radio.checkRDS();
  Controllers::readAndProcess();

  char buffer[6];
  int freq = Controllers::getFormatedFreq();
  sprintf(buffer, "%3d.%02d", freq / 100, freq % 100);
  Lcd::write(6, 0, buffer);

  delay(50); // TODO: temporary delay to avoid overflow
}
