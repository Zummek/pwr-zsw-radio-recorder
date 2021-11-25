#include <Arduino.h>
#include <Wire.h>
#include "AppWire.h"
#include "AppRadio.h"
#include "Controllers.h"
#include "Lcd.h"
#include "Equalizer.h"

byte v1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte v2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
};

byte v3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
};

byte v4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte v5[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte v6[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte v7[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte v8[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

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
  Lcd::lcd.createChar(0, v1);
  Lcd::lcd.createChar(1, v2);
  Lcd::lcd.createChar(2, v3);
  Lcd::lcd.createChar(3, v4);
  Lcd::lcd.createChar(4, v5);
  Lcd::lcd.createChar(5, v6);
  Lcd::lcd.createChar(6, v7);
  Lcd::lcd.createChar(7, v8);
}

int i = 0; // to check if program is running

void loop()
{
  AppRadio::radio.checkRDS();
  Controllers::readAndProcess();

  Equalizer::process();
  Serial.println(i++);

  delay(50); // TODO: temporary delay to avoid overflow
}
