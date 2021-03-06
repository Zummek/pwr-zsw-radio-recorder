#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16 
#define LCD_ROWS 2

#define LCD_RDS_COL 0
#define LCD_RDS_ROW 0
#define LCD_FREQUENCY_COL 0
#define LCD_FREQUENCY_ROW 1
#define LCD_RECORDING_COL 10
#define LCD_RECORDING_ROW 1
#define LCD_EQUALIZER_COL 13
#define LCD_EQUALIZER_ROW 1

class Lcd
{
public:
  static LiquidCrystal_I2C lcd;

  static void init();
  static void displayFrequency(int freq);
  static void displayRDS(char* rds);
  static void displayBands(int* value);
  static void displayRecording(char* c);

private:
  static void _write(uint8_t col, uint8_t row, char* text);
  static void _writeByte(uint8_t col, uint8_t row, int n);
};

#endif