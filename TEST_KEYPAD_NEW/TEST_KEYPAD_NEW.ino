const unsigned int matrix_column[] = {
  P6_0, P6_4, P6_2, P6_3};
const unsigned int matrix_row[] = {
  P2_4, P2_5, P2_6, P2_7};
#include <LiquidCrystal.h>
LiquidCrystal lcd(P1_3,P1_2,P4_3,P4_0,P3_7,P8_2 );

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  for (int x=0; x<3; x++)  {
    pinMode(matrix_column[x], OUTPUT);
  }
  for (int y=0; y<3; y++)  {
    pinMode(matrix_row[y], INPUT);
  }
}

void loop()
{
  for (int x=0; x<3; x++) {
    digitalWrite(matrix_column[x], LOW);
    for (int y=0; y<3; y++) {
      if ((digitalRead(matrix_row[y]))) 
      {
        Serial.print("Button at ");
        lcd.print(matrix_column[x]);
        Serial.print(" and ");
        lcd.print(matrix_row[y]);
        Serial.print(" was pressed.");
      }
      digitalWrite(matrix_column[x], LOW);
    }
  }
}

