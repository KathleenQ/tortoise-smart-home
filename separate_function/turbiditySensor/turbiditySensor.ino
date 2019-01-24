/*************
 * (Jan23) First, test Turbidity Sensor (value) seperately
 * Line Connection: Black->GND, Red->5V, Blue->AnalogIn
 * (http://wiki.dfrobot.com.cn/index.php?title=(SKU:SEN0189)_Turbidity_sensor%E6%B5%8A%E5%BA%A6%E4%BC%A0%E6%84%9F%E5%99%A8%E6%A8%A1%E5%9D%97)
 * More turbid, output VALUE is smaller!
 * theory: more turbid, less light goes through, smaller I, smaller V
 * (STANDARD voltage: 4.1 +- 0.3 V)
 *************/

const int turbidityPin = A1;
int turbidityValue = 0; //[Range 0-1023] initialize
float turbidityVol = 0; //convert the turbidityValue to a voltage (0-5V)

void setup() {
  Serial.begin(9600); //need to test the frequency(9600?) later
}

void loop() {
  turbidityValue = analogRead(turbidityPin);
  turbidityVol = turbidityValue * (5.0 / 1024.0);
  Serial.print("Turbidity Level:");
  //Serial.println(turbidityValue);
  Serial.println(turbidityVol);
  delay(2000);
}
