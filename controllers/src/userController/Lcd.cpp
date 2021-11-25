#include "Lcd.h"
#include "Arduino.h"

LiquidCrystal_I2C Lcd::lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void Lcd::init()
{
  lcd.init();
  lcd.backlight();
}

void Lcd::_write(uint8_t col, uint8_t row, char *text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

void Lcd::_writeByte(uint8_t col, uint8_t row, int n) {
  lcd.setCursor(col, row);
  lcd.write(n);
}

void Lcd::displayFrequency(int freq) {
  char buffer[11];
  sprintf(buffer, "%3d.%02d MHz", freq / 100, freq % 100);
  Lcd::_write(LCD_FREQUENCY_COL, LCD_FREQUENCY_ROW, buffer);
}

void Lcd::displayRDS(char *rds) {
  Lcd::_write(LCD_FREQUENCY_COL, LCD_FREQUENCY_ROW, rds);
}

void Lcd::displayBands(int *values, uint8_t row) {
  for (int i = 0; i < 7; i++) {
    Lcd::_writeByte(i, row, map(values[i], 0, 1023, 0, 7));
  }
}
