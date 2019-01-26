/***************
 * (Jan22) For "land-water swap"
 * use 1 photoresistor (light sensor) & 2 relay modules
 * if "brightness < " && condition "board down", open "pull-up relay" for a while;
 * if "brightness >= " && condition "board up", open "drop-down relay" for a while
 ***************/
 
const int brightnessPin = A0;
const int upRelayPin = 2; //"pull-on relay" attach to digital 2
const int downRelayPin = 4; //"drop-down relay" attach to digital 4
boolean boardUp = false; //board condition: "true" for board up, "false" for board down
int brightnessValue = 0; //initialize

void setup() {
  pinMode(upRelayPin, OUTPUT);
  pinMode(downRelayPin, OUTPUT);
  Serial.begin(9600); //initialize the serial communication as 9600bps(?)
}

void loop() {
  brightnessValue = analogRead(brightnessPin); //read the value of photoresistor
  Serial.print("Brightness Level:");
  Serial.println(brightnessValue);
  
  if(brightnessValue < 200 && !boardUp) //small value --> very bright: board UP
  {
    digitalWrite(upRelayPin, HIGH); //turn on the pull-up relay
    Serial.println("board up");
    delay(3000); //time for pulley work
    digitalWrite(upRelayPin, LOW);
    boardUp = true;
    Serial.println("board stop up");
  }
  else if(brightnessValue >= 200 && boardUp) //board DOWN
  {
    digitalWrite(downRelayPin, HIGH); //turn on the drop-down relay
    Serial.println("board down");
    delay(3000);
    digitalWrite(downRelayPin, LOW);
    boardUp = false;
    Serial.println("board stop down");
  }
  delay(1000);
}
