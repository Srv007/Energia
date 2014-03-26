#include <TwoMsTimer.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>


////////////////////////////////////////ASE-128
// foreward sbox
const unsigned char sbox[256] =   {
//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

// inverse sbox
const unsigned char rsbox[256] =
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb
, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb
, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e
, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25
, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92
, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84
, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06
, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b
, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73
, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e
, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b
, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4
, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f
, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef
, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61
, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

// round constant
const unsigned char Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};


// multiply by 2 in the galois field
unsigned char galois_mul2(unsigned char value)
{
	if (value>>7)
	{
		return ((value << 1)^0x1b);
	} else
		return (value << 1);
}

// AES encryption and decryption function
// The code was optimized for memory (flash and ram)
// Combining both encryption and decryption resulted in a slower implementation
// but much smaller than the 2 functions separated
// This function only implements AES-128 encryption and decryption (AES-192 and 
// AES-256 are not supported by this code) 
void aes_enc_dec(unsigned char *state, unsigned char *key, unsigned char dir)
{
  unsigned char buf1, buf2, buf3, buf4, round, i;
   
  // In case of decryption
  if (dir) {
    // compute the last key of encryption before starting the decryption
    for (round = 0 ; round < 10; round++) {
      //key schedule
      key[0] = sbox[key[13]]^key[0]^Rcon[round];
      key[1] = sbox[key[14]]^key[1];
      key[2] = sbox[key[15]]^key[2];
      key[3] = sbox[key[12]]^key[3];
      for (i=4; i<16; i++) {
        key[i] = key[i] ^ key[i-4];
      }
    }
    
    //first Addroundkey
    for (i = 0; i <16; i++){
      state[i]=state[i] ^ key[i];
    }
  }
  
  // main loop
  for (round = 0; round < 10; round++){
    if (dir){
      //Inverse key schedule
      for (i=15; i>3; --i) {
	key[i] = key[i] ^ key[i-4];
      }  
      key[0] = sbox[key[13]]^key[0]^Rcon[9-round];
      key[1] = sbox[key[14]]^key[1];
      key[2] = sbox[key[15]]^key[2];
      key[3] = sbox[key[12]]^key[3]; 
    } else {
      for (i = 0; i <16; i++){
        // with shiftrow i+5 mod 16
	state[i]=sbox[state[i] ^ key[i]];
      }
      //shift rows
      buf1 = state[1];
      state[1] = state[5];
      state[5] = state[9];
      state[9] = state[13];
      state[13] = buf1;

      buf1 = state[2];
      buf2 = state[6];
      state[2] = state[10];
      state[6] = state[14];
      state[10] = buf1;
      state[14] = buf2;

      buf1 = state[15];
      state[15] = state[11];
      state[11] = state[7];
      state[7] = state[3];
      state[3] = buf1;
    }
    //mixcol - inv mix
    if ((round > 0 && dir) || (round < 9 && !dir)) {
      for (i=0; i <4; i++){
        buf4 = (i << 2);
        if (dir){
          // precompute for decryption
          buf1 = galois_mul2(galois_mul2(state[buf4]^state[buf4+2]));
          buf2 = galois_mul2(galois_mul2(state[buf4+1]^state[buf4+3]));
          state[buf4] ^= buf1; state[buf4+1] ^= buf2; state[buf4+2] ^= buf1; state[buf4+3] ^= buf2; 
        }
        // in all cases
        buf1 = state[buf4] ^ state[buf4+1] ^ state[buf4+2] ^ state[buf4+3];
        buf2 = state[buf4];
        buf3 = state[buf4]^state[buf4+1]; buf3=galois_mul2(buf3); state[buf4] = state[buf4] ^ buf3 ^ buf1;
        buf3 = state[buf4+1]^state[buf4+2]; buf3=galois_mul2(buf3); state[buf4+1] = state[buf4+1] ^ buf3 ^ buf1;
        buf3 = state[buf4+2]^state[buf4+3]; buf3=galois_mul2(buf3); state[buf4+2] = state[buf4+2] ^ buf3 ^ buf1;
        buf3 = state[buf4+3]^buf2;     buf3=galois_mul2(buf3); state[buf4+3] = state[buf4+3] ^ buf3 ^ buf1;
      }
    }
    
    if (dir) {
      //Inv shift rows
      // Row 1
      buf1 = state[13];
      state[13] = state[9];
      state[9] = state[5];
      state[5] = state[1];
      state[1] = buf1;
      //Row 2
      buf1 = state[10];
      buf2 = state[14];
      state[10] = state[2];
      state[14] = state[6];
      state[2] = buf1;
      state[6] = buf2;
      //Row 3
      buf1 = state[3];
      state[3] = state[7];
      state[7] = state[11];
      state[11] = state[15];
      state[15] = buf1;         
           
      for (i = 0; i <16; i++){
        // with shiftrow i+5 mod 16
        state[i]=rsbox[state[i]] ^ key[i];
      } 
    } else {
      //key schedule
      key[0] = sbox[key[13]]^key[0]^Rcon[round];
      key[1] = sbox[key[14]]^key[1];
      key[2] = sbox[key[15]]^key[2];
      key[3] = sbox[key[12]]^key[3];
      for (i=4; i<16; i++) {
        key[i] = key[i] ^ key[i-4];
      }
    }
  }
  if (!dir) {
  //last Addroundkey
    for (i = 0; i <16; i++){
      // with shiftrow i+5 mod 16
      state[i]=state[i] ^ key[i];
    } // enf for
  } // end if (!dir)
} // end function
 /////
 unsigned char pnr_data[] = "Saaaaaaaaaaaaaaa";
  //unsigned char ciphertext[] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
  //                              0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
 unsigned char key1[]   = "Saaaaaaaaaaaaaaa";
 unsigned char key2[]   = "Saaaaaaaaaaaaaaa";
////////////////////////////////////////end ASE-128

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
unsigned char enc_data[16];
int i;
//set 
char start_charactor='#';
char stop_charactor='*';

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
  lcd.clear();
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
  unsigned char parameter_value[]="****************";
  for(i=0;i<10;i++){
    parameter_value[i] = key();
    lcd.write(parameter_value[i]);  	
  }
  CIPSTART(host);
  Serial1.println("AT+CIPSEND");
  delay(500);
  Serial1.print("GET ");
  Serial1.print(query_url);
  for(i=0;i<16;i++){
    Serial1.write(parameter_value[i]);
  }
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
  unsigned char parameter_value[]="****************";
  for(i=0;i<10;i++){
    parameter_value[i] = key();
    lcd.write(parameter_value[i]);  	
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
  Serial1.println("Content-Length: 24");
  Serial1.println("");
  Serial1.print(parameter);
  for(i=0;i<16;i++){
    Serial1.write(parameter_value[i]);
  }
  Serial1.println("");
  Serial1.write(10);
  Serial1.write(26);
}
//saurav
void query_smart_post(String query_url, String query_statement,String parameter)
{
  lcd.clear();
  CIPSTART(host);
  lcd.setCursor(0,0);
  lcd.print(query_statement);
  lcd.setCursor(0,1);
  unsigned char parameter_value[]="****************";
  i=0;
  while(i<12){
    if (digitalRead(button3)==HIGH) loop();
    if(rfid.available()){
      parameter_value[i]=rfid.read();
      lcd.write(parameter_value[i]);
      i++;
    }
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
  Serial1.println("Content-Length: 24");
  Serial1.println("");
  Serial1.print(parameter);
  for(i=0;i<16;i++){
    Serial1.write(parameter_value[i]);
  }
  Serial1.println("");
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
      if (a==start_charactor){
        lcd.setCursor(0,i);
        Serial1.readBytesUntil('*',data[i], 20);
        i++;
      }
      if (a==stop_charactor){
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
    query_smart_post(posturl,pnrurl_statement,"pnrcard=");
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
    query_smart_post(posturl,pnrurl_statement,"ttecard=");
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
  lcd.print("Enter Sec:");
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
    internal_temp(); 
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
  lcd.setCursor(0, 0);
  lcd.print("Uptime:");
  while(digitalRead(button1)==HIGH){
    time=millis();// put your main code here, to run repeatedly:
    sec=time/1000;
    mint=sec/60;
    hrs=mint/60;
    mint=mint-(hrs*60);
    sec=sec-(mint*60);
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
    delay(400);
  }
  delay(100);
  function1();

}

void internal_temp(void){
  float temp;
  lcd.clear();
  lcd.print("Internal Temp");
  lcd.setCursor(0,1);
  lcd.print("Cel:");
  lcd.setCursor(0,2);
  lcd.print("Cel:");
  lcd.setCursor(0,3);
  lcd.print("Cel:");
  while(digitalRead(button1)==HIGH){
    temp=analogRead(TEMPSENSOR);
    temp=(temp*1.5)/4095;
    temp=(temp-.70)*100/.225;
    lcd.setCursor(4,1);
    lcd.print("    ");
    lcd.setCursor(4,1);
    lcd.print(temp);
    delay(800);
  }
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
   main_lock();
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











