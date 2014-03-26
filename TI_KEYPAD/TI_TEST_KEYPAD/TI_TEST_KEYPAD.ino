#include <LiquidCrystal.h>
const int column[] = {
  P6_0, P6_4, P6_2, P6_3};
const int row[] = {
  P2_4, P2_5, P2_6, P2_7};
char Keypad[4] [4] = {  
  '*' , '7' , '4' , '1',
  '#' , '9' , '6' , '3',
  'D' , 'C' , 'B' ,' A',
  '0' , '8' , '5' , '2'   };

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(P1_3,P1_2,P4_3,P4_0,P3_7,P8_2 );

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
 
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  for (int x=0; x<3; x++)  {
    digitalWrite(column[x], HIGH);
  }
  char keypress='z';
  while (1)  {
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if ((digitalRead(row[y])) == LOW) {
          keypress =Keypad[x] [y];
          delay(1);
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



