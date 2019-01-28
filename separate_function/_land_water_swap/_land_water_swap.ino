/***************
 * (Jan28) For "land-water swap"
 * Combination of "land_water_swap_without_pressure" and "pressureSensor"
 * use 1 photoresistor (light sensor) & 2 relay modules & 1 pressure sensor
 * if (NOT bright && condition "board down") && NO pressure, open "pull-up relay" for a while;
 * if bright && condition "board up", open "drop-down relay" for a while
 * --> here, 'NO pressure' means no tortoises currently on the flat board
 * Smaller brightness value, Brighter!
 * 
 * Use of diaphragm pressure sensor: when tortoises are on the flat part, the board will not be dropped down~
 * Line Connection: AD0-->Analog In (for 'voltage' output), {TTL-->HIGH/LOW (digital I/O)}.
 ***************/
 
const int brightnessPin = A0;
const int pressurePin = A2; //pressure sensor AD0 attaches to A2
const int upRelayPin = 2; //"pull-up relay"
const int downRelayPin = 4; //"drop-down relay"
boolean boardUp = false; //board condition: "true" for board up, "false" for board down
int brightnessValue = 0;
float pressureValue = 0.0;

void setup() {
  pinMode(upRelayPin, OUTPUT);
  pinMode(downRelayPin, OUTPUT);
  pinMode(pressurePin, OUTPUT);
  Serial.begin(9600); //initialize the serial communication as 9600bps(?)
}

void loop() {
  brightnessValue = analogRead(brightnessPin);
  pressureValue = analogRead(pressurePin);
  
  Serial.print("Brightness Level:");
  Serial.print(brightnessValue);
  Serial.print(" Pressure Value:");
  Serial.println(pressureValue);
  
  if(brightnessValue >= 500 && !boardUp && pressureValue < 1) //board UP with NO pressure {brightness value 500 just for TEST}
  {
    digitalWrite(upRelayPin, HIGH); //turn on the pull-up relay
    Serial.print(" board up ");
    delay(3000); //time for pulley work
    digitalWrite(upRelayPin, LOW);
    boardUp = true;
    Serial.println("board stop up");
  }
  else if(brightnessValue < 500 && boardUp) //board DOWN
  {
    digitalWrite(downRelayPin, HIGH); //turn on the drop-down relay
    Serial.print(" board down ");
    delay(3000);
    digitalWrite(downRelayPin, LOW);
    boardUp = false;
    Serial.println("board stop down");
  }
  delay(1000);
}
