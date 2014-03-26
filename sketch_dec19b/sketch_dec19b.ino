#include <LiquidCrystal.h>


LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1);
void function1(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("THIS IS FIUNCTION 1");
  delay(1000);
}
  
 void setup(){
  lcd.begin(20,4);
  lcd.clear();
  pinMode(P8_2, INPUT_PULLDOWN); 
  }
void loop()
{
  if (digitalRead(P8_2)==HIGH){
    lcd.print("HELLO");
    delay(10
  }
    // put your main code here, to run repeatedly:
  
}
