#include <SoftwareSerial.h>
SoftwareSerial rfid(P4_1,P4_2);
void setup()
{
  rfid.begin(14400);// put your setup code here, to run once:
  
}

void loop()
{
  if(rfid.available()) rfid.println("t");
}

