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
  Wire.beginTransmission(WireAddresses::audioController);
  Wire.write(WireCommands::audioController::startRecording);
  Wire.endTransmission();
  delay(2000);
  Wire.beginTransmission(WireAddresses::audioController);
  Wire.write(WireCommands::audioController::stopRecording);
  Wire.endTransmission();
  delay(2000);
}