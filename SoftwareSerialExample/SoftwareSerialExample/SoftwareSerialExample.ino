
#include <SoftwareSerial.h>

SoftwareSerial mySerial(P1_4, P1_3); // RX, TX

void setup()  
{
  mySerial.begin(9600);
}

void loop() // run over and over
{
  if (mySerial.available())
    mySerial.write(mySerial.read());
}

