#define RedLed   P1_0
#define GreenLed P1_6
#define S2       P1_3
void setup()
{
  Serial.begin(9600);
  delay(200);
  Serial.println("\nG'Day LPMGrant1");
  pinMode(RedLed, OUTPUT);
  digitalWrite(RedLed, HIGH);
  pinMode(GreenLed, OUTPUT);
  digitalWrite(GreenLed, HIGH);
  pinMode(S2,INPUT_PULLUP);
  digitalWrite(S2, HIGH);
  attachInterrupt(S2, Wake, RISING);
}
 
void loop()
{ 
  //Serial.print("About to sleep"); // when this line is added LPM4 instruction is ignored ????
  digitalWrite(GreenLed, LOW);
  LPM4; //_BIS_SR(LPM4_bits | GIE);
  Serial.println(" Now awake");
  digitalWrite(GreenLed, HIGH);
  delay(1000);
  digitalWrite(RedLed, HIGH);
  
}
void Wake()
{
  digitalWrite(RedLed, LOW);
}
