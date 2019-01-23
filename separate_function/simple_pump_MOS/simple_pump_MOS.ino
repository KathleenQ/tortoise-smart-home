/*************
 * (Jan23) basic use of pump & MOS module
 * (pay attention to the line connection ~ )
 *************/

const int pumpPin = 13; //pump attaches to digital 13

void setup() {
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600); //later change (9600?)
}

void loop() {
  digitalWrite(pumpPin, HIGH); //turn on pump
  Serial.println("Pump ON");
  delay(5000);
  digitalWrite(pumpPin, LOW); //turn off pump
  Serial.println("Pump OFF");
  delay(3000);
}
