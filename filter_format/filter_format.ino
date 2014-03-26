

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
  Serial1.setTimeout(50000);
  lcd.clear();
  char data[4][40];
  char a;
  int i=0;
  for(int j=0;j<5;j++){
    for(int k=0;k<19;k++){
      data[j][k]=' ';
    }
  }
  while(1){
    if(Serial1.available()){
      a=Serial1.read();
      if (a=='#'){
        lcd.setCursor(0,i);
        Serial1.readBytesUntil('*',data[i], 20);
      }
      i++;
      if (a=='*'){
        i=0;
        for(int j=0;j<5;j++){
          lcd.setCursor(0,j);
          for(int k=0;k<20;k++){
            lcd.print(data[j][k]);
          }
        }
        break;
      }
    }
  }
}






