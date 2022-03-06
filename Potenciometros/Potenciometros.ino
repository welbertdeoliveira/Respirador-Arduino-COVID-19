
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);



void setup() 
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  lcd.init();
  lcd.clear();
  lcd.setBacklight (HIGH);
  delay(200);
  lcd.setCursor(0,0);
  lcd.print("Ambu Automatico 2.0");
  delay(1000);
  lcd.setCursor(5,2);
  lcd.print("Auto-teste");
  delay(2000);
  lcd.setCursor(10,3);
  lcd.print("OK");
  delay(1000);
  lcd.clear();
     
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Ajuste de parametros");
  lcd.setCursor(0,1);
  lcd.print("Vz:");
  lcd.setCursor(4,1);
  lcd.print((analogRead(A0)/10));
  lcd.setCursor(0,2);
  lcd.print("Sp:");
  lcd.setCursor(4,2);
  lcd.print((analogRead(A1)/10));
  lcd.setCursor(0,3);
  lcd.print("Plt:");
  lcd.setCursor(5,3);
  lcd.print((analogRead(A2)/10));
  lcd.setCursor(10,1);
  lcd.print("O2:");
  lcd.setCursor(14,1);
  lcd.print((analogRead(A3)/10));
  lcd.setCursor(10,2);
  lcd.print("Pr:");
  lcd.setCursor(14,2);
  lcd.print((analogRead(A4)/10));
  // put your main code here, to run repeatedly:
}
