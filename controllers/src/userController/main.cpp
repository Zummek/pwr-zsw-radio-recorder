#include <Arduino.h>
#include <Wire.h>
#include "I2CBus.hpp"
#include "AppRadio.hpp"
#include "Controllers.hpp"
#include "Lcd.hpp"
#include "Equalizer.hpp"

void setup()
{
  // Setup serial for debug
  Serial.begin(115200);
  Serial.println("\nHello - User controller");

  // Setup the I2C Bus as Master
  Wire.begin();

  Controllers::init();
  AppRadio::init();

  Equalizer::init();
  Lcd::init();
}

void loop()
{
  AppRadio::radio.checkRDS();
  Controllers::readAndProcess();

  Equalizer::process();
}
