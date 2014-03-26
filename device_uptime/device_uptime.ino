#include <LiquidCrystal.h>
LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1);
unsigned long time;
unsigned long sec;
unsigned long mint;
unsigned long hrs;

void setup()
{
  lcd.begin(20,4);
  lcd.clear(); // put your setup code here, to run once:
  lcd.print("Time: ");
}

void loop()
{
  time=millis();// put your main code here, to run repeatedly:
  sec=time/1000;
  mint=sec/60;
  hrs=mint/60;
  mint=mint-(hrs*60);
  sec=sec-(mint*60);
  lcd.setCursor(6, 0);
  lcd.print("            ");
  lcd.setCursor(6, 0);
  lcd.print(hrs);
  lcd.print(":");
  lcd.print(mint);
  lcd.print(":");
  lcd.print(sec);
  delay(200);
}

