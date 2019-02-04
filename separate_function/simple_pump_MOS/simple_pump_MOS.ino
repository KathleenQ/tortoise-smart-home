/*************
 * (Jan23) basic use of pump & MOS module
 * pay attention to the Line Connection:
 * {smaller}'+'->I/O,{larger}'+'->5V,{both}'-'->GND
 *************/

const int pumpPin = 11; //pump attaches to digital 11

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
