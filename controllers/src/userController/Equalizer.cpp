#include "Equalizer.h"
#include "Arduino.h"

int *Equalizer::bandValues;

void Equalizer::init()
{
  pinMode(EQUALIZER_RESET, OUTPUT);
  pinMode(EQUALIZER_STROBE, OUTPUT);

  digitalWrite(EQUALIZER_RESET, LOW);
  delay(1);
  digitalWrite(EQUALIZER_RESET, HIGH);
}

void Equalizer::process()
{
  digitalWrite(EQUALIZER_RESET, HIGH);
  digitalWrite(EQUALIZER_RESET, LOW);
 
  for (int band = 0; band < 7; band++)
  {
    digitalWrite(EQUALIZER_STROBE, LOW);
    delayMicroseconds(36);
    Equalizer::bandValues[band] = analogRead(EQUALIZER_ANALOG);
    digitalWrite(EQUALIZER_STROBE, HIGH);
  }

  Lcd::displayBands(bandValues, 1);
}
