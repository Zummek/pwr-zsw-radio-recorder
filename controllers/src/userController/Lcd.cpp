#include "Lcd.h"

LiquidCrystal_I2C Lcd::lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void Lcd::init()
{
  lcd.init();
  lcd.backlight();
}

void Lcd::write(uint8_t col, uint8_t row, char *text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

void Lcd::displayFrequency() {
  char buffer[11];
  int freq = Controllers::getFormatedFreq();
  sprintf(buffer, "%3d.%02d MHz", freq / 100, freq % 100);
  Lcd::write(LCD_FREQUENCY_COL, LCD_FREQUENCY_ROW, buffer);
}
