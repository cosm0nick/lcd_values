#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // (I2C address, columns, lines)

unsigned long updateInterval = 1000;
unsigned long prevTime = 0;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  init_lcd_output();
  analogWrite(11, 0);
}

void init_lcd_output()
{
  // static display characters
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.setCursor(0, 1);
  lcd.print("A: ");
}

void lcd_clear()
{
  // clear 14 columns on each row, start at column 3
  lcd.setCursor(3, 0);
  lcd.print("             ");
  lcd.setCursor(3, 1);
  lcd.print("             ");
}

void lcd_update(float volts, float amps) 
{
  // clear before updating
  lcd_clear();
  // write from column 3, row 0
  lcd.setCursor(3, 0);
  lcd.print(String(volts, 2) + "V");
  // write from column 3, row 1
  lcd.setCursor(3, 1);
  lcd.print(String(amps, 2) + "mA");
}

void measure(float &volts, float &amps)
{
  int measureA0 = analogRead(A0);
  int measureA1 = analogRead(A1);
  volts = measureA0 * 5.0 / 1024.0;
  amps = (measureA0 - measureA1) * 5000.0 / (1024.0 * 270.0);
}

void handle_input(float &volts, float &amps)
{
  // reads duty cycle input from serial
  if (Serial.available() > 0) 
  {
    String raw_input = Serial.readStringUntil('\n');
    raw_input.trim();
    //Serial.println("Read duty cycle: " + raw_input);
    analogWrite(11, raw_input.toInt());
    delay(150);
    measure(volts, amps);
    Serial.println(String(volts, 2) + "V\t" + String(amps, 2) + "mA");
  }
}

void display_cycle(float volts, float amps)
{
  // refresh display interval check, if interval is too small output is illegible
  unsigned long currTime = millis();
  if (currTime - prevTime > updateInterval) 
  {
    prevTime = currTime;
    lcd_update(volts, amps);
  }
}

void loop()
{
  float volts = 0, amps = 0;
  measure(volts, amps);
  handle_input(volts, amps);
  display_cycle(volts, amps);
}
