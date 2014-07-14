/*
  TEQIP-II
 by Saurabh Verma
 email:saurabh@gbpuat-tech.ac.in
 */
#include <TwoMsTimer.h>
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd( P4_1, P4_2, P2_7, P3_2, P6_6,P1_6);

//Keypad pin interface pins:
const int column[] = {
  P3_7, P4_0, P4_3, P1_2};
const int row[] = {
  P1_3, P1_4, P1_5, P2_4};

char Keypad[4] [4] = {  
  'D' , 'C' , 'B' , 'A',
  '#' , '9' , '6' , '3',
  '0' , '8' , '5' ,' 2',
  '*' , '7' , '4' , '1'   };        //

#define Motion_sensor P2_2
#define lcd_light P2_5
#define light_sensor A0
#define light1 P8_1
#define light2 P2_3
#define light3 P2_6
#define light4 P3_1
char data[6];
float temp;
float temp_max=0;
float temp_min=0;
const int ledPin = GREEN_LED;      // the number of the LED pin 
int ledState = LOW;             // ledState used to set the LED
int light1_state=LOW;
int light2_state=LOW;
int light3_state=LOW;
int light4_state=LOW;
boolean light_control_state = 0;
boolean lcd_brightness_control_state=1;
int light_sensor_value=0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20,4);
  Serial.begin(9600);
  Serial.setTimeout(500); 
  pinMode(PUSH1,INPUT_PULLUP);
  //for init keypad
  for (int x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }
  for (int y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  }

  analogWrite(lcd_light, 120);
  attachInterrupt(Motion_sensor, Motion_detected,RISING);
  TwoMsTimer::set(200, backend_process);
  TwoMsTimer::start();
  pinMode(ledPin, OUTPUT);
  pinMode(light1,OUTPUT);
  pinMode(light2,OUTPUT);
  pinMode(light3,OUTPUT);
  pinMode(light4,OUTPUT);
  init_main_display();
}

void loop() {
  /*while(1){
   lcd.print(keypress());}*/
  update_main_display_values();
  update_light_sensor_value();
  temp_update();
  if(digitalRead(PUSH1)==LOW){
    main_menu();
  }
  if (Serial.available() > 0){
    Serial.readBytes(data, 2);
    if(data[0]=='O'){
      switch(data[1]){
      case '1':
        light1_state=HIGH;
        digitalWrite(light1,light1_state);
        break;

      case '2':
        light2_state=HIGH;
        digitalWrite(light2,light2_state);
        break;

      case '3':
        light3_state=HIGH;
        digitalWrite(light3,light3_state);
        break;

      case '4':
        light4_state=HIGH;
        digitalWrite(light4,light4_state);
        break;
      }
    }
    if(data[0]=='F'){
      switch(data[1]){
      case '1':
        light1_state=LOW;
        digitalWrite(light1,light1_state);
        break;

      case '2':
        light2_state=LOW;
        digitalWrite(light2,light2_state);
        break;

      case '3':
        light3_state=LOW;
        digitalWrite(light3,light3_state);
        break;

      case '4':
        light4_state=LOW;
        digitalWrite(light4,light4_state);
        break;
      }
    }
  } 
}

void init_main_display(void){
  print_text("Light1:    Light2:","Light3:    Light4:","Temperature:","Light Int.:");
}

void update_main_display_values(void){
  lcd.setCursor(7,0);
  if(light1_state==LOW)
    lcd.print("OFF");
  else
    lcd.print("ON ");

  lcd.setCursor(7,1);
  if(light3_state==LOW){
    lcd.print("OFF");
  }
  else
    lcd.print("ON ");

  lcd.setCursor(18,0);
  if(light2_state==LOW)
    lcd.print("OF");
  else
    lcd.print("ON");

  lcd.setCursor(18,1);
  if(light4_state==LOW)
    lcd.print("OF");
  else
    lcd.print("ON");

}

void backend_process(void){//saurabh
  if(lcd_brightness_control_state==1){
    light_sensor_value=analogRead(A0);
    light_sensor_value=(light_sensor_value-3000)/4.2;
    if(light_sensor_value<=0) light_sensor_value=0;
    if(light_sensor_value>=255) light_sensor_value=255;
    analogWrite(lcd_light, light_sensor_value);
  }
}

void update_light_sensor_value(void){
    lcd.setCursor(10,3);
    lcd.print("     ");
    lcd.setCursor(10,3);
    lcd.print(light_sensor_value);
}

void temp_update(void){
  temp=analogRead(A1);
  temp=(3300*temp)/40950;
  lcd.setCursor(12,2);
  if(temp>temp_max) temp_max=temp;
  if(temp<temp_min) temp_min=temp;
  lcd.print(temp);
  lcd.print(" C");
  delay(100);
}


void print_text(char *l1,char *l2,char *l3,char *l4){
  lcd.clear();
  lcd.print(l1);
  lcd.setCursor(0,1);
  lcd.print(l2);
  lcd.setCursor(0,2);
  lcd.print(l3);
  lcd.setCursor(0,3);
  lcd.print(l4);
}

void print_text(char *l1,char *l2,char *l3){
  lcd.clear();
  lcd.print(l1);
  lcd.setCursor(0,1);
  lcd.print(l2);
  lcd.setCursor(0,2);
  lcd.print(l3);
}

void print_text(char *l1,char *l2){
  lcd.clear();
  lcd.print(l1);
  lcd.setCursor(0,1);
  lcd.print(l2);
}

void main_menu(void){
main_menu:  
  print_text("1-Light Control","2-LCD Brightness","3-menu3","4-menu4");
  char x=keypress();
  switch(x){
  case '1':
    light_control();
    break;

  case '2':
    lcd_brightness_control();
    break;

  case '3':
    //menu3();

  case '4':
    //menu4();

  case 'D':
    break;

  default:
    goto main_menu;
  }
  init_main_display();
  return;
}

void light_control(void){
  print_text("Light Control:","1-Automatic Control","2-Manual Control");
light_control:  
  char x=keypress();
  switch(x){
  case '1':
    automatic_control();
    break;

  case '2':
    maunal_control();

  case 'D':
    break;

  default:
    goto light_control;
    break;
  }
  init_main_display();
}

void automatic_control(void){
  lcd.clear();
  print_text("Automatic Control:","1-Change State","State:");
automatic_control:  
  lcd.setCursor(6,2);
  if(light_control_state==0) lcd.print("AUTOMATIC");
  else lcd.print("MANUAL    ");
  char x=keypress();
  switch(x){
  case '1':
    light_control_state=~light_control_state;
    goto automatic_control;

  case 'D':
    break;

  default:
    goto automatic_control;
    break;
  }
  if(light_control_state==0) Serial.begin(9600);
  else Serial.end();
  init_main_display();
}

void maunal_control(void){
  light_control_state=1;
  print_text("1-Light 1:","2-Light 2:","3-Light 3:","4-Light 4:");
maunal_control:
  lcd.setCursor(10,0);
  if(light1_state==LOW)
    lcd.print("OFF");
  else
    lcd.print("ON ");

  lcd.setCursor(10,1);
  if(light2_state==LOW){
    lcd.print("OFF");
  }
  else
    lcd.print("ON ");

  lcd.setCursor(10,2);
  if(light3_state==LOW)
    lcd.print("OFF");
  else
    lcd.print("ON  ");

  lcd.setCursor(10,3);
  if(light4_state==LOW)
    lcd.print("OFF");
  else
    lcd.print("ON  ");

  char x=keypress();
  switch(x){
  case '1':
    if(light1_state==LOW)
      light1_state=HIGH;
    else
      light1_state=LOW;
    digitalWrite(light1,light1_state);
    goto maunal_control;

  case '2':
    if(light2_state==LOW)
      light2_state=HIGH;
    else
      light2_state=LOW;
    digitalWrite(light2,light2_state);
    goto maunal_control;
    
  case '3':
    if(light3_state==LOW)
      light3_state=HIGH;
    else
      light3_state=LOW;
    digitalWrite(light3,light3_state);
    goto maunal_control;

  case '4':
    if(light4_state==LOW)
      light4_state=HIGH;
    else
      light4_state=LOW;
    digitalWrite(light4,light4_state);
    goto maunal_control;  

  case 'D':
    break;

  default:
    goto maunal_control;
    break;
  }
  init_main_display();
}

void lcd_brightness_control(){
lcd_brightness_control:  
  print_text("1-Automatic Control","2-Manula Control");
  char x=keypress();
  switch(x){
  case '1':
    automatic_brighrness_control();
    break;

  case '2':
    manual_brightness_control();
    break;

  case 'D':
    break;

  default:
    goto lcd_brightness_control;
  }
  init_main_display();
}

void automatic_brighrness_control(){
automatic_brightness_control:  
  print_text("Automatic Control:","1-ChangeControl","Status:");
  lcd.setCursor(8,2);
  if(lcd_brightness_control_state==1) lcd.print("AUTOMATIC");
  else lcd.print("MANUAL     ");
  char x=keypress();
  switch(x){
  case '1':
    lcd_brightness_control_state=~lcd_brightness_control_state;
    goto automatic_brightness_control;

  case 'D':
    break;

  default:
    goto automatic_brightness_control;
  }
  init_main_display();
}

void manual_brightness_control(void){
  lcd_brightness_control_state=0;
  print_text("Manual Conntrol:","Brightness value:","Status:");
  lcd.setCursor(8,2);
  if(lcd_brightness_control_state==1) lcd.print("AUTOMATIC");
  else lcd.print("MANUAL     ");
manual_brightness_control:
  analogWrite(lcd_light, light_sensor_value);
  char x=keypressed();
  switch(x){
  case 'A':
  light_sensor_value++;
  lcd.setCursor(16,1);
  lcd.print("   ");
  lcd.setCursor(16,1);
  if(light_sensor_value>=255) light_sensor_value=0;
  lcd.print(light_sensor_value);
  goto manual_brightness_control;
  
  case 'B':
  light_sensor_value--;
  lcd.setCursor(16,1);
  lcd.print("    ");
  lcd.setCursor(16,1);
  if(light_sensor_value<=0) light_sensor_value=255;
  lcd.print(light_sensor_value);
  goto manual_brightness_control;
    
  case 'D':
    break;

  default:
    goto manual_brightness_control;
  }
  init_main_display();
}

//This function is for PIR Motion sensor,it's call when there is a rising pulse in PIN8_1
void Motion_detected(void){
  // if the LED is off turn it on and vice-versa:
  if (ledState == LOW)
    ledState = HIGH;
  else
    ledState = LOW;
  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);
}

char keypress() {
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

char keypressed() {
  for (int x=0; x<3; x++)  {
    digitalWrite(column[x], HIGH);
  }
  char keypress='z';
  while (1)  {
    for (int x=0; x<4; x++)  {
      delay(50);
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        delay(50);
        if (!(digitalRead(row[y]))) {
          keypress =Keypad[x] [y];  
          //while(!(digitalRead(row[y])));
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }
  delay(100);
  return keypress;
}





