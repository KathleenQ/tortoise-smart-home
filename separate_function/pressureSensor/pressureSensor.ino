/****************
 * (Jan28) Use of diaphragm pressure sensor
 * may use to better improve “land-water swap” function：
 * when tortoises are on the flat part, the board will not be dropped down.
 * Line Connection: TTL-->HIGH/LOW (digital I/O), AD0-->Analog In (for 'voltage' output)
 *                  (be able to not connect both ~).
 ****************/

const int pressurePin = A2; //AD0 attaches to A2 port
float pressureValue = 0.0;
 
void setup() {
  pinMode(pressurePin, OUTPUT);
  Serial.begin(9600); //(?)
}

void loop() {
  pressureValue = analogRead(pressurePin);
  Serial.print("Pressure Value:");
  Serial.println(pressureValue);
  delay(1500);
}
