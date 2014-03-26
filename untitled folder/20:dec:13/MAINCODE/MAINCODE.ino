#include <TwoMsTimer.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial rfid(P1_5,P1_3); //RX,TX
SoftwareSerial gsm(P1_2,P1_3);//RX,TX
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
int adc;
void automatic_bacllight(void){
  adc=analogRead(A4);
  adc=adc/12;
  adc=255-adc;
  if(adc<0) adc=0;
  analogWrite(P1_4,adc);
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
          digitalWrite(P7_0,HIGH);
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
  digitalWrite(P7_0,LOW);
  return keypress;
}

char keymain() {
  char keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    if (digitalRead(P4_3)==LOW) function1();
    if (digitalRead(P4_0)==LOW) function2();
    if (digitalRead(P6_3)==LOW) {
      device_lock();
      break;
    }
    if (digitalRead(P8_2)==HIGH) function3();
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(P7_0,HIGH);
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
  digitalWrite(P7_0,LOW);
  return keypress;
}

char key() {
  char keypress;
  for (int x=0; x<4; x++)  {
    digitalWrite(column[x], HIGH);
  }
  keypress='z';
  while (1)  {
    if(digitalRead(P6_3)==LOW) {
      device_lock();
      break;
    }
    for (int x=0; x<4; x++)  {
      digitalWrite(column[x], LOW);
      for (int y=0; y<4; y++) {
        if (!(digitalRead(row[y]))) {
          digitalWrite(P7_0,HIGH);
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
  digitalWrite(P7_0,LOW);
  return keypress;
}

String smart_card(void){
  String card;
  rfid.flush();
  char a;
  int i=0;
  while(i<12){
    if (digitalRead(P8_2)==HIGH) loop();
    if(rfid.available()){
      a=rfid.read();
      card.concat(a);
      i++;
    }
  }
  rfid.println(card);
  return card;
}


int main_lock(void){
  static int i;
  String pass;
  String password=("1111");
  String code=String(12);
  String id_card="4E00707BD590";
  static char a;
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
      pass.concat(a);	
    }
    lcd.noBlink();
    lcd.setCursor(0, 1);
    digitalWrite(P7_0,HIGH);
    if ((password.compareTo(pass))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(P7_0,LOW);
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
      digitalWrite(P7_0,LOW);
      main_lock();
    }
    break;

  default:
    main_lock();
    break;
  }
  lcd.clear();
  digitalWrite(P7_0,LOW);
  lcd.noBlink();
  return 0;
}

int device_lock(void){
  static int i;
  String pass;
  String password=("1111");
  String code=String(12);
  String id_card="4E00707BD590";
  static char a;
  lcd.clear();
  lcd.print("        LOCK");
  delay(500);
  lcd.clear();
  analogWrite(P1_4,0);
  delay(500);
  while(digitalRead(P6_3)==HIGH);
  lcd.clear();
  analogWrite(P1_4,200);
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
      pass.concat(a);	
    }
    lcd.noBlink();
    lcd.setCursor(0, 1);
    digitalWrite(P7_0,HIGH);
    if ((password.compareTo(pass))==0){
      lcd.print("TRUE ");
    }
    else{
      lcd.print("WORNG");
      delay(1000);
      digitalWrite(P7_0,LOW);
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
      digitalWrite(P7_0,LOW);
      device_lock();
      break;
    }
    break;

  default:
    device_lock();
    break;
  }
  lcd.clear();
  digitalWrite(P7_0,LOW);
  lcd.noBlink();
  loop();
  return 0;
}
void send_cmd(String cmd,char *response,int wait){
  gsm.println(" ");
  delay(200);
  gsm.println(cmd);
  gsm.setTimeout(wait); 
  if(gsm.find(response));
}

void CIICR(void){
  delay(200);
  gsm.println("AT+CIICR");
  if(gsm.find("OK"));

  gsm.println("AT+CIFSR");
  gsm.setTimeout(5000);
  if(gsm.find("ERROR")) CIICR() ;
}

void CIPSTART(String host)
{
  gsm.print("AT+CIPSTART=\"TCP\",\"");
  gsm.print(host);
  gsm.println("\",\"80\"");
  if((gsm.find("OK")));
  if((gsm.find("CONNECT")));
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
  gsm.println("AT+CIPSEND");
  delay(500);
  gsm.print("GET ");
  gsm.print(query_url);
  gsm.println(" HTTP/1.1");
  gsm.print("HOST:");
  gsm.println(host);
  gsm.println("User-Agent: ATCAD");
  gsm.write(10);
  gsm.write(26);
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
  gsm.println("AT+CIPSEND");
  delay(500);
  gsm.print("POST ");
  gsm.print(query_url);
  gsm.println(" HTTP/1.1");
  gsm.print("HOST:");
  gsm.println(host);
  gsm.println("User-Agent: ATCAD");
  gsm.println("Content-Type: application/x-www-form-urlencoded");
  gsm.println("Content-Length: 18");
  gsm.println(parameter);
  gsm.write(10);
  gsm.write(26);
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
    smart_card_number();
    while(key()=='A');
    break;

  case '2':
    query_post(posturl,pnrurl_statement,"pnrcode:");
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
  char a=key();
  switch(a){
  case '1':
    smart_card_number();
    while(key()=='A');
    break;

  case '2':
    query_post(posturl,tteurl_statement,"ttecode:");
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
  lcd.print("SETTINGS:");
  lcd.setCursor(0,1);
  lcd.print("1-BACKLIGHT");
  lcd.setCursor(0,2);
  lcd.print("2-POWER DOWN MODE");
  lcd.setCursor(0,3);
  lcd.print("3-VIBRATION");
settings:
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

  case 'D': 
    delay(500);
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
void vibration_settings(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("VIBRATION SETTING");
  lcd.setCursor(0,1);
  lcd.print("1-OFF");
  lcd.setCursor(0,2);
  lcd.print("2-ON");
  char a=key();
  switch(a){
    case '1':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VIRBRATION:OFF");
    pinMode(P7_0,INPUT);
    break;
    
    case '2':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VIRBRATION:ON");
    pinMode(P7_0,OUTPUT);
    digitalWrite(P7_0,HIGH);
    delay(500);
    digitalWrite(P7_0,LOW);
    break;
    
    default :
    vibration_settings();
    break;
  }
}

void function1(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("THIS IS FIUNCTION 1"); 
  delay(1000);
  loop();
}

void function2(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("THIS IS FIUNCTION 2");
  delay(1000);
  loop();
}

void function3(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("THIS IS FIUNCTION 3");
  delay(1000);
  loop();
}
void setup(){
  pinMode(P4_3, INPUT_PULLUP); 
  pinMode(P4_0, INPUT_PULLUP); 
  pinMode(P6_3, INPUT_PULLUP); 
  pinMode(P8_2, INPUT_PULLDOWN); 
  pinMode(P7_0,OUTPUT);
  digitalWrite(P7_0,LOW);
  TwoMsTimer::set(200, automatic_bacllight);
  lcd.begin(20,4);
  lcd.clear();
  // initialize UART communication @ 9600 bps
  gsm.begin(9600); 
  rfid.begin(9600);
  for (int x=0; x<4; x++)  {
    pinMode(column[x], OUTPUT);
  }
  for (int y=0; y<4; y++)  {
    pinMode(row[y], INPUT_PULLUP);
  } 
  lcd.clear();
  /*gsm.println("ATE0");
   send_cmd("AT+CGATT=1","OK",5000);
   delay(5000);
   send_cmd("AT+CGDCONT=1,\"IP\",\"INTERNET\"","OK",5000);
   delay(5000);
   send_cmd("AT+CSTT=\"INTERNET\",\"\",\"\"","OK",5000);
   delay(5000);
   CIICR();
   delay(5000);
   send_cmd("AT+CIPHEAD=1","OK",5000);
   delay(5000);7
   lock();
   //query_get(tteurl,tteurl_statement);*/
  pinMode(P1_4, OUTPUT);
  digitalWrite(P1_4,HIGH);
  main_lock();

}

void loop(){
main_menu:
  menu();
  char menu =keymain();
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



