#include <TwoMsTimer.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define rfid_RX P1_5
#define rfid_TX P6_0

//Side button Configuration//
#define button1 P4_3
#define button2 P4_0
#define button3 P8_2
#define lock_button P6_3

//vibration pin Configuration
#define vibraton_pin P7_0

//uart configuration for rf-id
SoftwareSerial rfid(rfid_RX,rfid_TX);

//Keypad pin interface pins:
const int column[] = {
  P4_1,P4_2,P2_7,P3_2};
const int row[] = {
  P6_6, P1_6, P1_2, P1_3};

//keypad number control:
char Keypad[4] [4] = {  
  'D' , 'C' , 'B' , 'A',
  '#' , '9' , '6' , '3',
  '0' , '8' , '5' ,' 2',
  '*' , '7' , '4' , '1'   };  

int Keypad_int[4] [4] = {  
  0 , 0 , 0 ,  0,
  0 , 9 , 6 ,  3,
  0 , 8 , 5 ,  2,
  0 , 7 , 4 ,  1    };    

LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1); //LCD pin Configuration

char pnrdata[50][4][20]; //storage for pnr numbers history
int current_pnr_no=0;
//post request 
String tteurl_statement="Enter TTE CODE:";
String pnrurl_statement="Enter PNR CODE:";
String ticketanalysisurl_statement="ENTER STATION CODE:";
String tteurl="/srv.php?ttecode=";
String pnrurl="/srv.php?pnrcode=";
String posturl="/srv.php";
String host="www.ln0.in";

//gobel variables
int lcd_backlight_value;                       // lcd_backlight_value value of light sensor
boolean backlight_status;     // backlight status
boolean temp_backlight_status;     // backlight status
boolean vibration_status;     // backlight status
String id_card="4E00707BD590";//id card number of tte for unlock device
String password=("1111");

//globle variables for Device Uptime
unsigned long time;
unsigned long sec;
unsigned long mint;
unsigned long hrs;
//Globle Variables for real time dislay
int real_sec;
int real_mint;
int real_hrs;
char a;
int i;

void automatic_bacllight(void){
  if(backlight_status==true){
    lcd_backlight_value=analogRead(A4);
    lcd_backlight_value=lcd_backlight_value/12;
    lcd_backlight_value=255-lcd_backlight_value;
    if(lcd_backlight_value<0) lcd_backlight_value=0;
    analogWrite(P1_4,lcd_backlight_value);
    delay(10);
  }
}

int keypress_int() {
  int keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,HIGH);
          keypress =Keypad_int[x] [y];
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
  delay(10);
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}

char keypress() {
  char keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,HIGH);
          keypress =Keypad[x] [y];
          if(keypress!='z') goto out;
        }
      }
      digitalWrite(column[x], HIGH);
      if(keypress!='z') break;
    }
out:;
    if(keypress!='z') break;
  }
  delay(10);
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}

char keymain() {
  char keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    if (digitalRead(button1)==LOW) function1();
    if (digitalRead(button2)==LOW) function2();
    if (digitalRead(button3)==HIGH) function3();
    if (digitalRead(lock_button)==LOW) {
      device_lock();
      break;
    }
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,HIGH);
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
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}

char key() {
  char keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    if(digitalRead(lock_button)==LOW) {
      device_lock();
      break;
    }
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(vibraton_pin,HIGH);
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
  digitalWrite(vibraton_pin,LOW);
  return keypress;
}

String smart_card(void){
  String smart_card_number;
  rfid.flush();
  i=0;
  while(i<12){
    if (digitalRead(button3)==HIGH) loop();
    if(rfid.available()){
      a=rfid.read();
      smart_card_number.concat(a);
      i++;
    }
  }
  rfid.println(smart_card_number);
  return smart_card_number;
}


int main_lock(void){
  String user_enter_password;
  String code=String(12);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("        LOCK    ");
  lcd.setCursor(0,1);
  lcd.print("1-Manual");
  lcd.setCursor(0,2);
  lcd.print("2-ID CARD");
  a=key();
  switch(a){
  case '1':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ENTER PASSWORD:");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.blink();
    for  (i=0;i<4;i++){
      a = key();
      lcd.print("*");
      user_enter_password.concat(a);	
    }
    lcd.noBlink();
    lcd.setCursor(0, 1);
    digitalWrite(vibraton_pin,HIGH);
    if ((password.compareTo(user_enter_password))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      main_lock();
    }
    break;

  case '2':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Swipe Your Card:");
    code=smart_card();
    lcd.setCursor(0,1);
    lcd.print(code);
    lcd.setCursor(0,1);
    lcd.print("********");
    if ((id_card.compareTo(code))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      main_lock();
    }
    break;

  default:
    main_lock();
    break;
  }
  lcd.clear();
  digitalWrite(vibraton_pin,LOW);
  lcd.noBlink();
  return 0;
}

int device_lock(void){
  String user_enter_password;
  String code=String(12);
  lcd.clear();
  lcd.print("        LOCK");
  delay(500);
  temp_backlight_status=backlight_status;
  backlight_status=false;
  delay(500);
  analogWrite(P1_4,0);
  while(digitalRead(lock_button)==HIGH);
  lcd.clear();
  backlight_status=temp_backlight_status;
  analogWrite(P1_4,244);
  lcd.setCursor(0, 0);
  lcd.print("        LOCK    ");
  lcd.setCursor(0,1);
  lcd.print("1-Manual");
  lcd.setCursor(0,2);
  lcd.print("2-ID CARD");
  delay(500);
  a=key();
  switch(a){
  case '1':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ENTER PASSWORD:");
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.blink();
    for  (i=0;i<4;i++){
      a = key();
      lcd.print("*");
      user_enter_password.concat(a);	
    }
    lcd.noBlink();
    lcd.setCursor(0, 1);
    digitalWrite(vibraton_pin,HIGH);
    if ((password.compareTo(user_enter_password))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      device_lock();
      break;
    }
    break;

  case '2':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Swipe Your Card:");
    code=smart_card();
    lcd.setCursor(0,1);
    lcd.print(code);
    lcd.setCursor(0,1);
    lcd.print("********");
    if ((id_card.compareTo(code))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(vibraton_pin,LOW);
      device_lock();
      break;
    }
    break;

  default:
    device_lock();
    break;
  }
  lcd.clear();
  digitalWrite(vibraton_pin,LOW);
  lcd.noBlink();
  loop();
  return 0;
}
void send_cmd(String cmd,char *response,int wait){
  Serial1.println(" ");
  delay(200);
  Serial1.println(cmd);
  Serial1.setTimeout(wait); 
  if(Serial1.find(response)) lcd.print("OK");
}

void CIICR(void){
  delay(200);
  Serial1.println("AT+CIICR");
  if(Serial1.find("OK"));

  Serial1.println("AT+CIFSR");
  Serial1.setTimeout(5000);
  if(Serial1.find("ERROR")) CIICR() ;
}

void CIPSTART(String host)
{
  Serial1.print("AT+CIPSTART=\"TCP\",\"");
  Serial1.print(host);
  Serial1.println("\",\"80\"");
  if((Serial1.find("OK")));
  if((Serial1.find("CONNECT")));
}

void query_get(String query_url, String query_statement)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(query_statement);
  lcd.setCursor(0,1);
  for(i=0;i<10;i++){
    a = key();
    lcd.print(a);
    query_url.concat(a);	
  }
  CIPSTART(host);
  Serial1.println("AT+CIPSEND");
  delay(500);
  Serial1.print("GET ");
  Serial1.print(query_url);
  Serial1.println(" HTTP/1.1");
  Serial1.print("HOST:");
  Serial1.println(host);
  Serial1.println("User-Agent: ATCAD");
  Serial1.write(10);
  Serial1.write(26);
  delay(4000);
}

void query_post(String query_url, String query_statement,String parameter)
{
  lcd.clear();
  CIPSTART(host);
  lcd.setCursor(0,0);
  lcd.print(query_statement);
  lcd.setCursor(0,1);
  for(i=0;i<10;i++){
    a = key();
    lcd.print(a);
    parameter.concat(a);	
  }
  delay(2000);
  Serial1.println("AT+CIPSEND");
  delay(500);
  Serial1.print("POST ");
  Serial1.print(query_url);
  Serial1.println(" HTTP/1.1");
  Serial1.print("HOST: ");
  Serial1.println(host);
  Serial1.println("User-Agent: ATCAD");
  Serial1.println("Connection: Keep-Alive");
  Serial1.println("Content-Type: application/x-www-form-urlencoded");
  Serial1.println("Content-Length: 18");
  Serial1.println("");
  Serial1.println(parameter);
  Serial1.write(10);
  Serial1.write(26);
}

void pnr_post_result()
{
  Serial1.setTimeout(50000);
  lcd.clear();
  char data[4][20];
  i=0;
  for(int j=0;j<5;j++){
    for(int k=0;k<20;k++){
      data[j][k]=' ';
    }
  }
  while(i<5){
    if(Serial1.available()){
      a=Serial1.read();
      if (a=='#'){
        lcd.setCursor(0,i);
        Serial1.readBytesUntil('*',data[i], 20);
        i++;
      }
      if (a=='*'){
        i=0;
        for(int j=0;j<i;j++){
          lcd.setCursor(0,j);
          for(int k=0;k<20;k++){
            lcd.print(data[j][k]);
          }
        }
        break;
      }
    }
  }
  for(int j=0;j<i;j++){
    for(int k=0;k<20;k++){
      pnrdata[current_pnr_no][j][k]=data[j][k];
    }
  }
  current_pnr_no++;
}


void menu(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-PNR QUERY");
  lcd.setCursor(0,1);
  lcd.print("2-TICKET ANALYSIS");
  lcd.setCursor(0,2);
  lcd.print("3-SETTINGS");
  lcd.setCursor(0,3);
  lcd.print("4-TTE LOGOUT");
}
void pnr_query(void){
  String ticket =String(40);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-SMART CRAD");
  lcd.setCursor(0,1);
  lcd.print("2-MANUAL");
  a=key();
  switch(a){
  case '1':
    smart_card_number();
    while(key()=='A');
    break;

  case '2':
    query_post(posturl,pnrurl_statement,"pnrcode=");
    Serial1.flush();
    char data[80];
    int p;
    p=0;
    lcd.clear(); 
    break;

  case 'C':
    break;

  case 'D': 
    break;

  default:
    break;
  }
  // delay(1000);
}

void tte_login_logout(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-SMART CRAD");
  lcd.setCursor(0,1);
  lcd.print("2-MANUAL");
  a=key();
  switch(a){
  case '1':
    smart_card_number();
    while(key()=='A');
    break;

  case '2':
    query_post(posturl,tteurl_statement,"ttecode=");
    break;

  case 'C':
    break;

  case 'D': 
    break;

  default:
    break;
  }
  // delay(1000);
}

void smart_card_number(){
  String code=String(12);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SWIPE CARD");
  lcd.setCursor(0,1);
  code=smart_card();
  lcd.print(code);
}

void ticketanalysis(void){
  query_post(posturl,ticketanalysisurl_statement,"station=");
}

void settings(void){
  char menu;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-BACKLIGHT");
  lcd.setCursor(0,1);
  lcd.print("2-POWER DOWN MODE");
  lcd.setCursor(0,2);
  lcd.print("3-VIBRATION");
  lcd.setCursor(0,3);
  lcd.print("4-Time");
  menu =key();
  switch(menu){
  case '1':
    backlight_settings();
    break;

  case '2':
    // powerdownmode_settings(); 
    break;
  case '3':
    vibration_settings(); 
    break;

  case '4': 
    time_settings();
    break;

  case 'D': 
    break;

  default:
    settings();
  }
}

void backlight_settings(void){
  char press;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BACKLIGHT SETTINGS:");
  lcd.setCursor(0,1);
  lcd.print("1-AUTOMATIC");
  lcd.setCursor(0,2);
  lcd.print("2-MANUAL");
  press=key();
  switch(press){
  case '1':
    automatic_bacllight_setting();
    break;

  case '2':
    manual_backlight_setting();
    break;

  case 'D':
    settings(); 
    break;

  default:
    backlight_settings();
  } 
}

int automatic_bacllight_setting(void){
  i=0;
  char press;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AUTOMATIC BACKLIGHT:");
  lcd.setCursor(0,1);
  lcd.print("1-OFF");
  lcd.setCursor(0,2);
  lcd.print("2-ON");
  press=key();
  if (press=='1'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("AUTOMATIC BACKLIGHT:");
    lcd.setCursor(0,1);
    lcd.print("CURRENT STATUS:OFF");
    backlight_status==false;
  }
  if(press=='2'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("AUTOMATIC BACKLIGHT:");
    lcd.setCursor(0,1);
    lcd.print("CURRENT STATUS:ON");
    backlight_status=true;
  }
  return 1;
}  

void manual_backlight_setting(void){
  i=0;
  char press;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MANUAL BACKLIGHT:");
  lcd.setCursor(0,1);
  lcd.print("Value:");
  backlight_status==false;
  while(1){
    press=key();
    lcd.setCursor(6,1);
    if(press=='A'){
      TwoMsTimer::stop();
      i++;
      if(i>250) i=255;
      lcd.print(i);
      analogWrite(P1_4,i);
    }
    if(press=='B'){
      TwoMsTimer::stop();
      i--;
      if (i<0) i=0;
      lcd.print(i);
      lcd.print("  ");
      analogWrite(P1_4,i);
    }
    if(press=='C'){
      break;
    }
    if(press=='D'){
      backlight_settings();
      break;
    }
  }
}
void vibration_settings(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("VIBRATION SETTING");
  lcd.setCursor(0,1);
  lcd.print("1-OFF");
  lcd.setCursor(0,2);
  lcd.print("2-ON");
  a=key();
  switch(a){
  case '1':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VIRBRATION:OFF");
    vibration_status=false;
    pinMode(vibraton_pin,INPUT);
    break;

  case '2':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VIRBRATION:ON");
    pinMode(vibraton_pin,OUTPUT);
    digitalWrite(vibraton_pin,HIGH);
    delay(500);
    vibration_status=true;
    digitalWrite(vibraton_pin,LOW);
    break;

  default :
    vibration_settings();
    break;
  }
}

void time_settings(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-Set Current Time");
  lcd.setCursor(0,1);
  lcd.print("2-Set Alarm");
  lcd.setCursor(0,2);
  lcd.print("3-Request For Time");
  a=keymain();
  switch(a){
  case '1':
    set_time();
    break;

  case '2':
    // manual_backlight_setting();
    break;

  case '3':
    // 
    break;

  case 'D':
    settings(); 
    break;

  default:
    backlight_settings();
  } 

} 

void set_time(void){
  int x,y;
  lcd.clear();
  lcd.print("Enter Hours:");
  lcd.blink();
  x=keypress_int();
  lcd.print(x);
  y=keypress_int();
  lcd.print(y);
  lcd.noBlink();
  real_hrs=x*10+y;
  lcd.setCursor(0,1);
  lcd.print("Enter Min:");
  lcd.blink();
  x=keypress_int();
  lcd.print(x);
  y=keypress_int();
  lcd.print(y);
  lcd.noBlink();
  real_mint=x*10+y;
  lcd.setCursor(0,2);
  lcd.print("Enter Min:");
  lcd.blink();
  x=keypress_int();
  lcd.print(x);
  y=keypress_int();
  lcd.print(y);
  lcd.noBlink();
  time=millis();
  sec=time/1000;
  mint=sec/60;
  hrs=mint/60;
  mint=mint-(hrs*60);
  sec=sec-(mint*60);
  real_sec=x*10+y;
  real_sec=real_sec-sec;
  real_hrs=real_hrs-hrs;
  real_mint=real_mint-mint;
  delay(1000);
}

void function1(void){
  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CUP Details");
  lcd.setCursor(0,1);
  lcd.print("1-CPU Uptime");
  lcd.setCursor(0,2);
  lcd.print("2-CPU Temperature");
  lcd.setCursor(0,3);
  lcd.print("3-pta nhi");
  a = keymain();
  switch(a){
  case '1':
    display_uptime();
    break;

  case '2':
    //ticketanalysis(); 
    break;

  case '3':
    //settings();
    break;

  case '4':
    //tte_login_logout();
    break; 

  default:
    loop();
  }
}

void display_uptime()
{
  lcd.clear();
  while(digitalRead(button1)==HIGH){
    time=millis();// put your main code here, to run repeatedly:
    sec=time/1000;
    mint=sec/60;
    hrs=mint/60;
    mint=mint-(hrs*60);
    sec=sec-(mint*60);
    lcd.setCursor(0, 0);
    lcd.print("Uptime:");
    lcd.setCursor(7, 0);
    lcd.print("            ");
    lcd.setCursor(7, 0);
    lcd.print(hrs);
    lcd.print(":");
    lcd.print(mint);
    lcd.print(":");
    lcd.print(sec);
    lcd.setCursor(0, 1);
    lcd.print("Real Time:");
    lcd.setCursor(10, 1);
    lcd.print("            ");
    lcd.setCursor(10, 1);
    lcd.print(hrs+real_hrs);
    lcd.print(":");
    lcd.print(mint+real_mint);
    lcd.print(":");
    lcd.print(sec+real_sec);
    lcd.setCursor(0, 2);
    lcd.print("Alarm Time:00:00:00");
    delay(300);
  }
  delay(100);
  function1();

}

void function2(void){
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-PNR History");
  a=key();
  switch(a){
  case '1':
    display_pnr_history();
    break;

  case '2':
    //ticketanalysis(); 
    break;

  case '3':
    //settings();
    break;

  case '4':
    //tte_login_logout();
    break; 

  default:
    loop();
  }
  loop();
}

void display_pnr_history(void){
  int j=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press A=NEXT");
  lcd.setCursor(0,1);
  lcd.print("Press B=Back");
  lcd.setCursor(0,2);
  lcd.print("Press D=Cancel");
  delay(2000);
out:;
  lcd.clear();
  if(j<0) j=0;
  for(i=0;i<5;i++){
    lcd.setCursor(0,i);
    lcd.print(pnrdata[i+j*4][0]);
  }
  a=key();
  if(a=='A'){
    j++;
    goto out;
  }
  if(a=='B'){
    j--;
    goto out;
  }

  switch(a){
  case '1':
    lcd.print(pnrdata[1+j*4][0]);
    break;

  case '2':
    lcd.print(pnrdata[2+j*4][0]);
    break;

  case '3':
    lcd.print(pnrdata[3+j*4][0]);
    break;

  case '4':
    lcd.print(pnrdata[4+j*4][0]);
    break; 

  case 'D':
    break;

  default:
    goto out;
  }
}

void function3(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("THIS IS FIUNCTION 3");
  delay(1000);
  loop();
}

void setup(){
  backlight_status=true;
  pinMode( button1, INPUT_PULLUP); 
  pinMode( button2, INPUT_PULLUP); 
  pinMode( lock_button, INPUT_PULLUP); 
  pinMode( button3, INPUT_PULLDOWN); 
  pinMode( vibraton_pin, OUTPUT);
  digitalWrite( vibraton_pin, LOW);
  TwoMsTimer::set( 200, automatic_bacllight);
  lcd.begin( 20, 4);
  lcd.clear();
  Serial1.begin(9600);   // initialize UART communication @ 9600 bps serial port 1 is used pin P3.3 and P3.4 are used 
  rfid.begin(9600);      // initialize UART communication with RF-ID @ 9600 bps software-serial is used
  for (int x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }
  for (int y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  } 
  lcd.clear();
  // Serial1.println("ATE0");
  /* send_cmd("AT+CGATT=1","OK",5000);
   send_cmd("AT+CGDCONT=1,\"IP\",\"INTERNET\"","OK",5000);
   send_cmd("AT+CSTT=\"INTERNET\",\"\",\"\"","OK",5000);
   CIICR();
   delay(5000);
   send_cmd("AT+CIPHEAD=1","OK",5000);
   //query_get(tteurl,tteurl_statement);*/
   pinMode( P1_4, OUTPUT);
   digitalWrite( P1_4, HIGH);
   lcd_backlight_value = analogRead(A4);
   lcd_backlight_value = lcd_backlight_value/12;
   lcd_backlight_value = 255-lcd_backlight_value;
   if(lcd_backlight_value<0) lcd_backlight_value=0;
   analogWrite(P1_4,lcd_backlight_value);
   Serial1.println("ATE0");
   //main_lock();
      pnrdata[0][0][0]='S';
      pnrdata[1][0][0]='A';
      pnrdata[2][0][0]='S';
      pnrdata[3][0][0]='S';
}

void loop(){
main_menu:
  menu();
  char menu = keymain();
  switch(menu){
  case '1':
    pnr_query();
    break;

  case '2':
    ticketanalysis(); 
    break;

  case '3':
    settings();
    break;

  case '4':
    tte_login_logout();
    break;  
  }
}











