#include <Arduino.h>
#include <Wire.h>
#include <AppWire.h>

void setup()
{
  // Setup serial for debug
  Serial.begin(115200);
  Serial.println("\nHello - User controller");

  // Setup the I2C Bus as Master
  Wire.begin();
}

void loop()
{
}
