#include <Arduino.h>
#include <Wire.h>
#include "AppWire.h"
#include "AppRadio.h"

void setup()
{
  // Setup serial for debug
  Serial.begin(115200);
  Serial.println("\nHello - User controller");

  // Setup the I2C Bus as Master
  Wire.begin();

  AppRadio::init();
}

void loop()
{
  AppRadio::radio.checkRDS();
  delay(50);  // TODO: temporary delay to avoid overflow
}
