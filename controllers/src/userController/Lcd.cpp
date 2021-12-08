#include "Lcd.hpp"
#include "Equalizer.hpp"
#include "Arduino.h"

byte char1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte char2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
};

byte char3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
};

byte char4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte char5[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte char6[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte char7[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte char8[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

LiquidCrystal_I2C Lcd::lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void Lcd::init()
{
  lcd.init();
  lcd.backlight();
  Lcd::lcd.createChar(0, char1);
  Lcd::lcd.createChar(1, char2);
  Lcd::lcd.createChar(2, char3);
  Lcd::lcd.createChar(3, char4);
  Lcd::lcd.createChar(4, char5);
  Lcd::lcd.createChar(5, char6);
  Lcd::lcd.createChar(6, char7);
  Lcd::lcd.createChar(7, char8);
}

void Lcd::_write(uint8_t col, uint8_t row, char* text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

void Lcd::_writeByte(uint8_t col, uint8_t row, int n) {
  lcd.setCursor(col, row);
  lcd.write(n);
}

void Lcd::displayFrequency(int freq) {
  char buffer[11];
  sprintf(buffer, "%3d.%02dMHz", freq / 100, freq % 100);
  Lcd::_write(LCD_FREQUENCY_COL, LCD_FREQUENCY_ROW, buffer);
}

void Lcd::displayRDS(char* rds) {
  char buffer[20];
  strncpy(buffer, rds, 20);
  buffer[19] = '\0';
  Lcd::_write(LCD_RDS_COL, LCD_RDS_ROW, buffer);
}

void Lcd::displayBands(int values[]) {
  for (int i = 0; i < EQUALIZER_BAUNDS_COUNT; i++)
    Lcd::_writeByte(LCD_EQUALIZER_COL + i, LCD_EQUALIZER_ROW, map(values[i], 0, 1023, 0, 7));
}

void Lcd::displayRecording(char* c) {
  Lcd::_write(LCD_RECORDING_COL, LCD_RECORDING_ROW, c);
}
