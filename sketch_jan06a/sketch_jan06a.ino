int ledState = HIGH;
uint8_t i = 0;
uint32_t average = 0;
uint32_t values[NUMBER];
uint8_t j = 0;
boolean flag = false;
#include <LiquidCrystal.h>
LiquidCrystal lcd(P2_2, P3_0, P3_1, P2_6, P2_3, P8_1);
void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  analogReference(INTERNAL1V5);
  analogRead(TEMPSENSOR); // first reading usually wrong

  Serial.begin(9600);
  pinMode(PUSH2, INPUT_PULLUP);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  lcd.begin( 20, 4);
  lcd.clear();

  Serial.print("\n\n\n*** MSP430 Thermometer \n");
  Serial.print("Press PUSH2 to end\n");
  Serial.print("instant\taverage\n");

  for (j=0; j<NUMBER; j++) values[j]=0;
  average = 0;
  j=0;
}

void printDec(uint32_t ui) {
  Serial.print(ui/10, DEC);
  Serial.print(".");
  Serial.print(ui%10, DEC);
}


void loop() {
  ledState = !ledState;

  // LEDs: green = ready; red = acquisition
  digitalWrite(flag ? GREEN_LED : RED_LED, ledState);

  if (i == 10) {
    i = 0;

    // Formula: http://www.43oh.com/forum/viewtopic.php?p=18248#p18248
    average -= values[j];
    values[j] = ((uint32_t)analogRead(TEMPSENSOR)*10667 - 19773668) * 10 >> 16;
    average += values[j];

    // Print measure
    printDec(values[j]);
    Serial.print("\t");

    // Print average
    if (flag) printDec(average/NUMBER);
    Serial.print("\n");
    delay(100);
    lcd.clear();
    lcd.print(average/NUMBER);
    j++;
    if (j==NUMBER) flag=true;
    j %= NUMBER;
  }

  if (digitalRead(PUSH2)==LOW) {
    Serial.print("\n\n*** End \n");
    Serial.end();
    while(true); // endless loop
  }
  delay(100);

  i++;
}
