#ifndef LCD_h
#define LCD_h

#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16 
#define LCD_ROWS 2

<<<<<<< HEAD
#define LCD_FREQUENCY_COL 0
#define LCD_FREQUENCY_ROW 0
#define LCD_RDS_COL 0
#define LCD_RDS_ROW 1

=======
>>>>>>> Add Lcd class with simple interface
class Lcd
{
public:
  static LiquidCrystal_I2C lcd;

  static void init();
<<<<<<< HEAD
  static void displayFrequency(int freq);
  static void displayRDS(char *rds);

private:
  static void _write(uint8_t col, uint8_t row, char *text);
=======
  static void write(uint8_t col, uint8_t row, char *text);
>>>>>>> Add Lcd class with simple interface
};

#endif