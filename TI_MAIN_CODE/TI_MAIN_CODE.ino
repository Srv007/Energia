
 #include <LiquidCrystal.h>
 
 // initialize the library with the numbers of the interface pins
 LiquidCrystal lcd(P4_1,P4_2,P2_7,P3_2,P6_6,P1_6);

 //Keypad pin interface pins:
 const int column[] = {
  P8_2, P3_7, P4_0, P4_3};
 const int row[] = {
  P1_2, P1_3, P1_4, P1_5};
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //keypad number control:
 char Keypad[4] [4] = {  
  'D' , 'C' , 'B' , 'A',
  '#' , '9' , '6' , '3',
  '0' , '8' , '5' ,' 2',
  '*' , '7' , '4' , '1'   };        //password of the system
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void setup() {
  Serial.begin(9600);
  for (int x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }

  for (int y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  }
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  char a[3];
  CGATT();
 lcd.print(a);
 delay(1000);
  CGDCONT();
  lock();
 }
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void loop()
 {
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
 }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
  return keypress;
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int lock(void){
  static int i;
  String pass;
  String password=("1111");
  char a;
  lcd.setCursor(0, 0);
  lcd.print("ENTER PASSWORD:");
  lcd.setCursor(0, 1);
  lcd.print("       ");
  lcd.setCursor(0, 1);
  lcd.blink();  
  for  (i=0;i<4;i++){
    a = key();
    lcd.print("*");
    pass.concat(a);	
  }
  lcd.noBlink();
  lcd.setCursor(0, 1);
  if ((password.compareTo(pass))==0){
    lcd.print("TRUE ");
    Serial.print("WELCOME");
  }
  else{
    lcd.print("WORNG");
    lock();
  }
  lcd.clear();
  return 0;
 }
 //*********************************************************************************************************************/
 void clc(int i)
 {
   
     lcd.setCursor(0, i);
     for(int i=0;i<16;i++){
       lcd.print(" ");
     }
 }
 
 //************************************************************************************************
 void CGATT(void)

{
	Serial.print("AT+CGATT=1");
        Serial.print("\n\r");
}

void CGDCONT(void)

{
  	Serial.print("AT+CGDCONT=1,");
	Serial.print('"');
	Serial.print("IP");
	Serial.print('"');
	Serial.print(',');
	Serial.print('"');
	Serial.print("INTERNET");
	Serial.print('"');
	Serial.print("\n\r");
}


