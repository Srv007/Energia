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
const int row[] = {
 P3_7, P4_0, P4_3, P1_2,P1_3, P1_4, P1_5, P2_4};

char Keypad[] = { '1','2','3','4','A','B','C','D' };        //

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
int serial=LOW;
boolean light_control_state = 0;
boolean lcd_brightness_control_state=1;
int light_sensor_value;
float max_temp;
float min_temp;
unsigned long time;
long int hrs;
long int mint;
long int sec;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20,4);
  Serial.begin(9600);
  Serial.setTimeout(500); 
  pinMode(PUSH1,INPUT_PULLUP);
  //for init keypad
  for (int y=0; y<8; y++)  {
    pinMode(row[y], INPUT);
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
  temp_update();
  temp_min=temp;
  init_main_display();
}

void loop() {
  /*while(1){
   lcd.print(keypress());}*/
  delay(90);
  digitalWrite( GREEN_LED,light1_state);
  if(digitalRead(row[0])==HIGH) light1_state=~light1_state; digitalWrite(light1,light1_state);
  if(digitalRead(row[1])==HIGH) light2_state=~light2_state; digitalWrite(light2,light2_state);
  if(digitalRead(row[2])==HIGH) light3_state=~light3_state; digitalWrite(light3,light3_state);
  if(digitalRead(row[3])==HIGH) light4_state=~light4_state; digitalWrite(light4,light4_state);
  if(digitalRead(row[4])==HIGH) Serial.begin(9600);
  if(digitalRead(row[5])==HIGH) Serial.begin(115200);
  if(digitalRead(row[6])==HIGH) {
    lcd.clear();
    lcd.clear();
    lcd.print("Max Temp:");
    lcd.print(temp_max);
    lcd.print(" C");
    lcd.setCursor(0,1);
    lcd.print("Min Temp:");
    lcd.print(temp_min);
    lcd.print(" C");
    while(1){
      time = millis();
      lcd.setCursor(0,2);
      time =time/1000;
      hrs=time/3600;
      mint=(time/60)-(hrs*60);
      sec=(time)-(mint*60)-(hrs*3600);
      lcd.print("Uptime:");
      lcd.print("          ");
      lcd.setCursor(7,2);
      lcd.print(hrs);
      lcd.print(":");
      lcd.print(mint);
      lcd.print(":");
      lcd.print(sec);
      delay(300);
      if(digitalRead(row[7])==HIGH){
      break;
      }
    }
    init_main_display();
  }
  //if(digitalRead(row[7])==HIGH) init_main_display(); loop();
  temp_update();
  update_main_display_values();
  update_light_sensor_value();
  if (Serial.available() > 0){
    Serial.readBytes(data, 2);
    if(data[0]=='O'){
      switch(data[1]){
      case '1':
        light1_state=HIGH;
        digitalWrite(light1,light1_state);
        digitalWrite( GREEN_LED,light1_state);
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
  if(light1_state==LOW){
    lcd.print("OFF");
    digitalWrite( GREEN_LED,light1_state);
  }
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

void Motion_detected(void){
  // if the LED is off turn it on and vice-versa:
  if(light_sensor_value>254){
    if (light1_state == LOW)
      light1_state=~light1_state;
  }
  digitalWrite( GREEN_LED,light1_state);
}





