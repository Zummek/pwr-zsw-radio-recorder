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
