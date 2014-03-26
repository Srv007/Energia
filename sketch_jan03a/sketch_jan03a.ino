#include <LiquidCrystal.h>
LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1);
void setup()
{
  lcd.begin(20,4);
  lcd.clear();
  Serial1.begin(9600);  // Update chunks

  analogWrite(P1_4,244);
}

void loop()
{
  char data[50];
 Serial1.setTimeout(50000);
 lcd.clear();
 while(1){
   if(Serial1.available()){
     //lcd.print("hbbvvbvbmb");
   if (Serial1.read()=='#'){
  //  lcd.print("#");
      Serial1.readBytes(data, 20); 
      lcd.print(data);
    }
    else 
    break;
   }
   }
    
}

