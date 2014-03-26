void setup()
{
  // put your setup code here, to run once:
  pinMode(P3_5,OUTPUT);
}

void loop()
{
  digitalWrite(P3_5,HIGH);// put your main code here, to run repeatedly:
  delay(500);
  digitalWrite(P3_5,LOW);
  delay(500);
}
