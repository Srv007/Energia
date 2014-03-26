
 
#if defined(__MSP430G2452__) || defined(__MSP430G2553__) || defined(__MSP430G2231__)|| defined(__MSP430F5529__) // LaunchPad specific
#include "Energia.h"
#else
#error Board not supported
#endif

#include "TwoMsTimer.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(P2_6,P2_3);
int a;
void setup()  { 
  mySerial.begin(9600);
  TwoMsTimer::set(1000, process_step);
//  analogReference(DEFAULT);
//  TwoMsTimer::start(); 
} 

// main loop that runs as long as Arduino has power
void loop()                     
{
  pinMode(A2,INPUT);
  a=analogRead(A3);
  mySerial.println(a);
  delay(100);
}


/*=============================================
 * interrupt handler section
 *=============================================*/

// Interrupt handler that smoothly moves the servo
void process_step(){
  mySerial.println("saurav");
}




