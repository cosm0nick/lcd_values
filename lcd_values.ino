// DOCS & DATASHEETS AVAILABLE AT:
// https://www.optimusdigital.ro/ro/compatibile-cu-arduino-nano/1686-placa-de-dezvoltare-compatibila-cu-arduino-nano-atmega328p-i-ch340.html
// https://www.optimusdigital.ro/ro/adaptoare-i-convertoare/89-adaptor-i2c-pentru-lcd-1602.html
// https://www.optimusdigital.ro/ro/optoelectronice-lcd-uri/867-modul-lcd-1602-cu-backlight-galben-verde-de-5v.html

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // (I2C address, columns, lines)

void init_lcd_output()
{
  // static display characters
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.setCursor(0, 1);
  lcd.print("A:");
}

void lcd_clear()
{
  // clear 14 columns on each row, start at column 2
  lcd.setCursor(2, 0);
  lcd.print("              ");
  lcd.setCursor(2, 1);
  lcd.print("              ");
}

void lcd_update(float voltage, float current) 
{
  // clear before updating
  lcd_clear();
  // write from column 2, row 0
  lcd.setCursor(2, 0);
  lcd.print(String(voltage, 3));
  // write from column 2, row 1
  lcd.setCursor(2, 1);
  lcd.print(String(current, 3));
}

void setup()
{
  lcd.init();
  lcd.backlight();
  init_lcd_output();
}

int accum = 0;

unsigned long updateInterval = 1000;
unsigned long prevTime = 0;

void loop()
{
  // TODO replace with readValues() function
  accum++;

  // refresh display interval check, if interval is too small output is illegible 
  unsigned long currTime = millis();
  if (currTime - prevTime > updateInterval) 
  {
    prevTime = currTime;
    lcd_update(accum, accum);
  }
}
