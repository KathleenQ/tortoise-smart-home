/***************
 * (Jan23) [for Water Changing] 
 * Combination of turbidity sensor & pump (with MOS module).
 * FILE "simple_pump_MOS" && "turbiditySensor".
 * if "turbidity voltage < 'normal level'" (more turbid), turn ON the pump
 * 'normol level' --> 4.1 +- 0.3 V (shown in shop's product information) 【？？？？？】
 * (more turbid, output VALUE is smaller)
 * turbity sensor connection: Black->GND, Red->5V, Blue->AnalogIn
 **************/

const int turbidityPin = A1;
const int pumpPin = 11; //pump attaches to digital 11
float turbidityVol = 0.0; //initialize

void setup() {
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600); //need to determine (9600?) later
  digitalWrite(pumpPin, LOW); //initialize
}

void loop() {
  int turbidityValue = analogRead(turbidityPin);
  turbidityVol = turbidityValue * (5.0 / 1024.0); //convert the "read turbidity value" (0-1023) to voltage (0-5V)
  Serial.print("Turbidity Voltage:");
  Serial.print(turbidityVol);
  if(turbidityVol < 1.8) // for test; [3.6V] ideally(scientifically) set "< 4.1-0.3=3.8 V"
  {
    digitalWrite(pumpPin, HIGH); //turn ON pump
    Serial.println(" Pump ON");
  }
  else
  {
    digitalWrite(pumpPin, LOW); //turn OFF pump
    Serial.println(" Pump OFF");
  }
  delay(5000);
}
