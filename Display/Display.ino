
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() 
{
  lcd.init();
  lcd.clear();
  lcd.setBacklight (HIGH);
  delay(200);
  lcd.setCursor(0,0);
  lcd.print("Ambu Automatico 2.0");
  lcd.setCursor(0,1);
  lcd.print("Velocidade: 20/min");
  lcd.setCursor(0,2);
  lcd.print("Plato: 2 seg");
  lcd.setCursor(0,3);
  lcd.print("02: 47%");
}

void loop() {
  // put your main code here, to run repeatedly:

}
