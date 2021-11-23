#ifndef LCD_h
#define LCD_h

#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16 
#define LCD_ROWS 2

#define LCD_FREQUENCY_COL 0
#define LCD_FREQUENCY_ROW 0

class Lcd
{
public:
  static LiquidCrystal_I2C lcd;

  static void init();
  static void displayFrequency(int freq);

private:
  static void write(uint8_t col, uint8_t row, char *text);
};

#endif