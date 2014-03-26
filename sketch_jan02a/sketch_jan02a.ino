
#include <LiquidCrystal.h>
LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1);

void setup()
{
  analogReference(INTERNAL1V5);
   lcd.begin(20,4);
   lcd.clear();
  lcd.setCursor(0,0); // put your setup code here, to run once:
  lcd.print("hello");
  delay(1000);
  analogWrite(P1_4,255);
}

void loop()
{
 /* // put your main code here, to run repeatedly:
  double a=analogRead(TEMPSENSOR);
  double c;
  c=((a*1.5)/4095);*/
  int c=adc=analogRead(A4);
  lcd.print(c);
  delay(500);
  lcd.clear();
}
