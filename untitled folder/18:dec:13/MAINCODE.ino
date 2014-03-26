#include <TwoMsTimer.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial mySerial(P1_2,P1_3);
SoftwareSerial rfid(P1_5,P1_3);
//Keypad pin interface pins:
const int column[] = {
  P4_1,P4_2,P2_7,P3_2};
const int row[] = {
  P6_6, P1_6, P3_3, P3_4};
//keypad number control:
char Keypad[4] [4] = {  
  'D' , 'C' , 'B' , 'A',
  '#' , '9' , '6' , '3',
  '0' , '8' , '5' ,' 2',
  '*' , '7' , '4' , '1'   };        //password of the system
LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1);

String tteurl_statement="Enter TTE CODE:";
String pnrurl_statement="Enter PNR CODE:";
String ticketanalysisurl_statement="ENTER STATION CODE:";
String tteurl="/srv.php?ttecode=";
String pnrurl="/srv.php?pnrcode=";
String posturl="/srv.php";
String host="www.ln0.in";
String backlight_status;
void automatic_bacllight(void){
  int a=analogRead(A4);
  a=a/12;
  a=255-a;
  if(a<0) a=0;
  analogWrite(P1_4,a);
  delay(10);
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
          digitalWrite(P3_5,HIGH);
          keypress =Keypad[x] [y];
         // while(!(digitalRead(row[y])));
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
  digitalWrite(P3_5,LOW);
  return keypress;
}

char key() {
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
          digitalWrite(P3_5,HIGH);
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
  digitalWrite(P3_5,LOW);
  return keypress;
}

String smart_card(void){
  String card;
  char a;
  int i=0;
  while(i<10){
    if(rfid.available()){
      a=rfid.read();
      card.concat(a);
      i++;
    }
  }
  return card;
}
  

int lock(void){
  static int i;
  String pass;
  String password=("1111");
  static char a;
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
  digitalWrite(P3_5,HIGH);
  if ((password.compareTo(pass))==0){
    lcd.print("TRUE ");
  }
  else{
    lcd.print("WORNG");
    lock();
  }
  lcd.clear();
  digitalWrite(P3_5,LOW);
  return 0;
}
void send_cmd(String cmd,char *response,int wait){
  mySerial.println(" ");
  delay(200);
  mySerial.println(cmd);
  mySerial.setTimeout(wait); 
  if(mySerial.find(response));
}

void CIICR(void){
  delay(200);
  mySerial.println("AT+CIICR");
  if(mySerial.find("OK"));
  
  mySerial.println("AT+CIFSR");
  mySerial.setTimeout(5000);
 if(mySerial.find("ERROR")) CIICR() ;
}

void CIPSTART(String host)
{
  mySerial.print("AT+CIPSTART=\"TCP\",\"");
  mySerial.print(host);
  mySerial.println("\",\"80\"");
  if((mySerial.find("OK")));
  if((mySerial.find("CONNECT")));
}

void query_get(String query_url, String query_statement)
{
  lcd.clear();
  static int i;
  static char a;
  lcd.setCursor(0,0);
  lcd.print(query_statement);
  lcd.setCursor(0,1);
  for(i=0;i<10;i++){
    a = key();
    lcd.print(a);
    query_url.concat(a);	
  }
  CIPSTART(host);
  mySerial.println("AT+CIPSEND");
  delay(500);
  mySerial.print("GET ");
  mySerial.print(query_url);
  mySerial.println(" HTTP/1.1");
  mySerial.print("HOST:");
  mySerial.println(host);
  mySerial.println("User-Agent: ATCAD");
  mySerial.write(10);
  mySerial.write(26);
  delay(4000);
}

void query_post(String query_url, String query_statement,String parameter)
{
  lcd.clear();
  static int i;
  static char a;
  CIPSTART(host);
  lcd.setCursor(0,0);
  lcd.print(query_statement);
  lcd.setCursor(0,1);
  for(i=0;i<10;i++){
    a = key();
    lcd.print(a);
    parameter.concat(a);	
  }
  delay(5000);
  mySerial.println("AT+CIPSEND");
  delay(500);
  mySerial.print("POST ");
  mySerial.print(query_url);
  mySerial.println(" HTTP/1.1");
  mySerial.print("HOST:");
  mySerial.println(host);
  mySerial.println("User-Agent: ATCAD");
  mySerial.println("Content-Type: application/x-www-form-urlencoded");
  mySerial.println("Content-Length: 18");
  mySerial.println(parameter);
  mySerial.write(10);
  mySerial.write(26);
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1-SMART CRAD");
  lcd.setCursor(0,1);
  lcd.print("2-MANUAL");
  char a=key();
 
  switch(a){
    case '1':
    while(1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SWIPE CARD");
    lcd.setCursor(0,1);
    lcd.print(smart_card());
    char d;
    d=key();
    if(d=='D') break;
    }
    break;
    
    case '2':
    manual_backlight_setting();
    break;
    
    case 'C':
    break;
   
    case 'D': 
    break;
    
    default:
    break;
  }
}
  

void ticketanalysis(void){
  query_post(posturl,ticketanalysisurl_statement,"station=");
}

void settings(void){
  char menu;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SETTINGS:");
  lcd.setCursor(0,1);
  lcd.print("1-BACKLIGHT");
  lcd.setCursor(0,2);
  lcd.print("2-POWER DOWN MODE");
settings:
  menu =key();
  switch(menu){
  case '1':
    backlight_settings();
    break;

  case '2':
   // powerdownmode_settings(); 
    break;
  case 'D': 
    break;
    
  default:
   goto settings;
   break;
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
backlight_settings:  
  press=key();
  switch(press){
    case '1':
    automatic_bacllight_setting();
    break;
    
    case '2':
    manual_backlight_setting();
    break;
   
    case 'D': 
    break;
    
    default:
   goto backlight_settings;
   break;
  } 
}

int automatic_bacllight_setting(void){
  int i=0;
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
    TwoMsTimer::stop();
  }
  if(press=='2'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("AUTOMATIC BACKLIGHT:");
    lcd.setCursor(0,1);
    lcd.print("CURRENT STATUS:ON");
    TwoMsTimer::start();
  }
  return 1;
}  

void manual_backlight_setting(void){
  int i=0;
  char press;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MANUAL BACKLIGHT:");
  lcd.setCursor(0,1);
    lcd.print("Value:");
  while(1){
    press=keypress();
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

void setup(){
  pinMode(P3_5,OUTPUT);
  digitalWrite(P3_5,LOW);
  TwoMsTimer::set(200, automatic_bacllight);
  lcd.begin(20,4);
  lcd.clear();
  mySerial.begin(9600); // initialize UART communication @ 9600 bps
  rfid.begin(9600);
// while(mySerial.read()!='y');
//  mySerial.setTimeout(5000);
  for (int x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }
  for (int y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  } 
  lcd.clear();
/*  mySerial.println("ATE0");
  send_cmd("AT+CGATT=1","OK",5000);
  delay(5000);
  send_cmd("AT+CGDCONT=1,\"IP\",\"INTERNET\"","OK",5000);
  delay(5000);
  send_cmd("AT+CSTT=\"INTERNET\",\"\",\"\"","OK",5000);
  delay(5000);
  CIICR();
  delay(5000);
  send_cmd("AT+CIPHEAD=1","OK",5000);
  delay(5000);
  //lock();
  //query_get(tteurl,tteurl_statement);*/
  pinMode(P1_4, OUTPUT);
  digitalWrite(P1_4,HIGH);

}

void loop(){
  main_menu:
  menu();
  char menu =key();
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
   query_get(tteurl,tteurl_statement);
   query_post(posturl,pnrurl_statement,"ttecode:");
   break;  
  }
  while(key()=='A');

}

