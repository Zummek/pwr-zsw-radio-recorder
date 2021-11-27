#include <Arduino.h>
#include <Wire.h>
#include "AppWire.hpp"

void I2CReceiveEvent(int howMany)
{
  int command = Wire.read();

  switch (command)
  {
  case WireCommands::audioController::startRecording:
    Serial.println("Start Recording");
    break;

  case WireCommands::audioController::stopRecording:
    Serial.println("Stop Recording");
    break;

  default:
    break;
  }
}

void setup()
{
  // Setup serial for debug
  Serial.begin(115200);
  Serial.println("\nHello - Audio controller");

  // Setup the I2C Bus as Slave on address 2
  Wire.begin(WireAddresses::audioController);
  Wire.onReceive(I2CReceiveEvent);
}

void loop()
{
}
