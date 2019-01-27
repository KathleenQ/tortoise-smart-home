/******************
 * (Jan27) LED lamp [for 'level' lighting]
 * For "lighting" function, 
 * plan to remotely controlled by the website/html ("button") commands，
 * (rather than controlled by some sensors' data) 
 * --> following daily-life feeding logic ~
 * Line Connection: VIN -> '+'5V, GND, 
 *      DIM -> light control (HIGH/LOW digital control, light level "PWM~" control).          
 * Plan to have 3 basic led brightness level (low, medium. high)
 ******************/

const int ledPin = 5; //LED attaches to PWM(Pulse Width Modulation) ~5

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); //?
}

void loop() { // (?) just TEST 3 basic LED levels [Range 0 - 255=3*85] --> Higher "ledLevel" value, Brighter. 
  int noLED = 0;
  int lowLED = 10;
  int mediumLED = 40;
  int highLED = 150; //(real test) reasonable brightness levels~
//  if(ledLevel = 0){
    analogWrite(ledPin, noLED);
    Serial.println("NO light");
    delay(1000); //finally unify for all functions~
//  }
//  if(ledLevel = 85){
    analogWrite(ledPin, lowLED);
    Serial.println("LOW light");
    delay(1000);
//  }
//  if(ledLevel = 170){
    analogWrite(ledPin, mediumLED);
    Serial.println("MEDIUM light");
    delay(1000);
//  }
//  if(ledLevel = 255){
    analogWrite(ledPin, highLED);
    Serial.println("HIGH light");
    delay(1000);
//  }
}
