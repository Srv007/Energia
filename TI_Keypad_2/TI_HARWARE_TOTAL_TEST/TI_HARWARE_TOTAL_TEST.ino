#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(P4_1,P4_2,P2_7,P3_2,P6_6,P1_6);
const int column[] = {
  P8_2, P3_7, P4_0, P4_3};
const int row[] = {
  P1_2, P1_3, P1_4, P1_5};
char Keypad[4] [4] = {  
  'D' , 'C' , 'B' , 'A',
  '#' , '9' , '6' , '3',
  '0' , '8' , '5' ,' 2',
  '*' , '7' , '4' , '1'   };
  
void setup() {
  for (int x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }

  for (int y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  }
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
}

void loop()
{
  char a;
  a=key();
  lcd.print(a);
}

char key() {
  for (int x=0; x<3; x++)  {
    digitalWrite(column[x], HIGH);
  }
  char keypress='z';
  while (1)  {
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          keypress =Keypad[x] [y];  
          while(!(digitalRead(row[y])));
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }
  delay(5);
  return keypress;
}



