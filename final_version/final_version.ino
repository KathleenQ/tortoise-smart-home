/**********************
   (Feb20) Add ALL 5 functions for "Tortoise Smart Home" artefact:
        Temperature, Water Changing, Lighting, Feeding, Land-water Swap.

   Use: Water-proof temperature sensor / Turbidity sensor, Pump (with MOS module), Real-time clock module /
        LED lamp / Micro servo / Photoresistor (light sensor), Relay modules (*2), Pressure sensor.

   Brief Explanation:
    (We will use real time to seperate various functions~)
   EVEN Minute:
     Temp: We will get data from water temperature sensor and send real-time message to the website, to let owner take further actions themselves.
     Swap: If (NOT bright && condition "board down") && NO pressure, open "pull-up relay" for a while;
           if bright && condition "board up", open "drop-down relay" for a while. (Smaller brightness value, brighter.)
   ODD Minute:
     Water Change: If more turbid than "normal level", turn ON the pump. (More turbid, output VALUE is smaller.)
   REMOTELY control:
     Light: Remotely button controlled through website. (Have 3 basic led brightness levels: "low", "medium", "high".)
     Feed: Feed at 2 remotely-set time points if the water is not too turbid.

   Line CONNECTION:
     Turbidity Sensor:Black->GND,Red->5V,Blue->AnalogIn; Pump with MOS module:{smaller}'+'->I/O,{larger}'+'->5V,{both}'-'->GND;
     Real-time clock: myWire(I/O, ~CLK, RST); LED: DIM->brightness level "PWM~" control or HIGH/LOW digital control;
     Micro servo: Gery->GND,Red->VCC(+5V),Orange->Digital; Pressure sensor: AD0->Analog In.
 ***********************/

//WiFi Connection
#include <SPI.h>
#include <WiFiNINA.h>
#include "wifi_secret.h"
//Real-time Clock
#include <ThreeWire.h>
#include <RtcDS1302.h>
//Feeding Device
#include <Servo.h>

unsigned char temp_reset();
void temp_write(char WRT);
unsigned char temp_read();

//WiFi Connection
char ssid[] = SECRET_SSID; //CHANGE actual SSID (name) & password in "wifi_secret.h"
char pass[] = SECRET_PASS; //(use for WPA, or use as key for WEP)
int keyIndex = 0; //your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

//"Temperature"
const int tempPin = 12;
float temperature = 0.0; //real-time temperature
float Temperature = 0.0; //final shown temperature on html

//"Land-water Swap"
const int brightnessPin = A0;
const int pressurePin = A2;
const int downRelayPin = 2; //"drop-down relay"
const int upRelayPin = 4; //"pull-up relay"
boolean boardUp = false; //board condition: "true" for board up, "false" for board down
int brightnessValue = 0;
int pressureValue = 0;
const int brightStandard = 200;

//"Water Changing"
const int turbidityPin = A4;
const int pumpPin = 11;
const float turbidWaterChanging = 1.1; //"standard"
float turbidityVol = 0.0;
boolean pumpOn = false; //using for website condition shown

//Real-time Clock
ThreeWire myWire(7, 6, 8); //Connection: myWire(I/O, CLK, RST)
RtcDS1302<ThreeWire> Rtc(myWire);

//"Feeding"
Servo myservo;
const float turbidFeeding = 0.6; //"standard", not too turbid to feed
int feedHr1 = 10; //(default) the pre-set feeding hour (8-20)
int feedHr2 = 18;
int feedMin = 0; //(default) (0,10,...50)
boolean isFeeding = false; ///using for website condition shown

//"Lighting"
const int ledPin = 5; //LED attaches to PWM(Pulse Width Modulation) ~5
const int noLED = 0; //Higher "ledLevel" value, Brighter!
const int lowLED = 1;
const int mediumLED = 2;
const int highLED = 4; //(with real TEST) reasonable brightness levels in RANGE 0-255
String ledCondition = "OFF";


void setup() {
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(upRelayPin, OUTPUT);
  pinMode(downRelayPin, OUTPUT);
  pinMode(pressurePin, OUTPUT);

  Serial.begin(9600); //initialize serial and wait for port to open

  //Real-time Clock
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (Rtc.GetIsWriteProtected()) { //RTC was write protected, enabling writing now
    Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning()) { //RTC was not actively running, starting now
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) { //RTC is older than compile time -> Updating DateTime
    Rtc.SetDateTime(compiled);
  }

  //"Feeding"
  myservo.attach(9); //attaches servo to I/O 9
  myservo.write(0); //back to 0 degrees

  //WiFi Connection
  while (!Serial) {
    ; //wait for serial port to connect (needed for native USB port only)
  }
  if (WiFi.status() == WL_NO_MODULE) { //check for the WiFi module
    Serial.println("Communication with WiFi module failed!");
    while (true); //don't continue
  }
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  while (status != WL_CONNECTED) { //attempt to connect to Wifi network
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    delay(10000); //wait 10s for connection
  }
  server.begin();
  printWifiStatus(); //you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available(); //listen for incoming clients

  //Real-time Clock
  RtcDateTime now = Rtc.GetDateTime();

  //"Feeding" function
  if (turbidityVol > turbidFeeding) { //NOT very turbid (with large turbidity voltage)-> judge turbidity first
    if (now.Minute() == feedMin && (now.Hour() == feedHr1 || now.Hour() == feedHr2)) //judge time then
    {
      isFeeding = true;
      myservo.write(15);
      delay(1000);
      myservo.write(20);
      delay(1000);
      myservo.write(25);
      delay(1000);
      myservo.write(30);
      delay(1000);
      myservo.write(35);
      delay(1000);
      myservo.write(40);
      delay(1200);
      myservo.write(35);
      delay(1000);
      myservo.write(30);
      delay(1000);
      myservo.write(25);
      delay(1000);
      myservo.write(20);
      delay(1000);
      myservo.write(15);
      delay(1000);
      myservo.write(0);
    }
    else {
      isFeeding = false;
    }
  }
  else {
    isFeeding = false;
  }

  if (now.Minute() % 2 == 0) //"EVEN" minute -> temperature & land-water swap, NO water changing
  {
    digitalWrite(pumpPin, LOW); //turn OFF pump
    pumpOn = false;

    //"Temperture" function
    if (!temp_reset()) {
      temp_write(0xCC); //"Skip_ROM"
      temp_write(0x44); //"Convert_T"
      delay(750);
      temp_reset();
      temp_write(0xCC); //"Skip_ROM"
      temp_write(0xBE); //"Read_scratchpad"
      unsigned short tempL = temp_read();
      unsigned short tempH = temp_read();
      unsigned int temp = ((unsigned int)tempH << 8) + (unsigned int)tempL;
      temp = (float)temp * 6.25;
      temperature = (float)temp / 100; //get the REAL "temperature" value
      if (temperature != 0) {
        Temperature = temperature;
      }

      //"Land-water Swap" function
      brightnessValue = analogRead(brightnessPin);
      pressureValue = analogRead(pressurePin);
      if (brightnessValue >= brightStandard && !boardUp && pressureValue < 1)
      { //board UP with NO pressure
        digitalWrite(upRelayPin, HIGH); //turn on the "pull-up relay"
        delay(3000); //time for pulley system works
        digitalWrite(upRelayPin, LOW);
        boardUp = true;
      }
      else if (brightnessValue < brightStandard && boardUp) //board DOWN as bright enough
      { //Smaller brightness value, Brighter!
        digitalWrite(downRelayPin, HIGH); //turn on the "drop-down relay"
        delay(3000); //(same, with physical TEST)
        digitalWrite(downRelayPin, LOW);
        boardUp = false;
      }
    }
  }
  else //"ODD" minute -> water changing, NO temperature & NO land-water swap
  {
    //"Water Changing" function
    int turbidityValue = analogRead(turbidityPin);
    turbidityVol = turbidityValue * (5.0 / 1024.0); //convert the "read turbidity value" (0-1023) to voltage
    if (turbidityVol < turbidWaterChanging) {
      digitalWrite(pumpPin, HIGH); //turn ON pump
      pumpOn = true;
    }
    else {
      digitalWrite(pumpPin, LOW); //turn OFF pump
      pumpOn = false;
    }
  }

  //WiFi Connection
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) { //if reaching the end of line and the line is blank, the http request has ended, you can send a reply
            client.println("HTTP/1.1 200 OK"); //send a standard http response header
            client.println("Content-Type: text/html");
            client.println("Connection: close"); //the connection will be closed after completion of the response
            client.println("Refresh: 15"); //refresh the page automatically every 15s
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");

            // REAL Contents part ! ! !
            client.println("<head>Kathleen's 2019 Summer <b>IoT</b> Project -- ");
            client.println("<body>");
            client.println("<h1>Tortoise Smart Home</h1>");

            //Real-time Clock
            client.print("<h3><i>Current Time:</i>   ");
            client.print(now.Day());
            client.print("/");
            client.print(now.Month());
            client.print("/");
            client.print(now.Year());
            client.print(" ");
            client.print(now.Hour());
            client.print(":");
            client.print(now.Minute());
            client.print(":");
            client.print(now.Second());
            client.println("</h3>");

            //"Temperature"
            client.println("<h2><mark><i>Temperature:</i></mark></h2>");
            client.print("<h3>Current water temperature is ");
            client.print(temperature);
            client.println(" C.</h3>");

            //"Land-water Swap"
            client.println("<h2><mark><i>Land-water Swap:</i></mark></h2>");
            client.print("Current brightness level is ");
            client.print(brightnessValue);
            client.println("; <br />");
            client.print("Board-down brightness level is below ");
            client.print(brightStandard);
            client.println(". <br />");

            client.print("Current pressure value is ");
            client.print(pressureValue);
            client.println(".<br />");
            if (pressureValue < 1) {
              client.println("NO tortoise is on board.");
            }
            else {
              client.println("Some tortoises are on board!");
            }

            client.print("<h3>The ladder board is currently ");
            if (boardUp) {
              client.println("UP and NOT time for climbing.</h3>");
            }
            else {
              client.println("DOWN and prepares for tortoises' climbing!</h3>");
            }

            //"Water Changing"
            client.println("<h2><mark><i>Water Changing:</i></mark></h2>");
            client.print("Current water turbidity voltage is ");
            client.print(turbidityVol);
            client.println("V; <br />");
            client.print("Standard water turbidity voltage is above ");
            client.print(turbidWaterChanging);
            client.println("V.");
            if (pumpOn) {
              client.println("<h3>Pump is currently ON!</h3>");
            }
            else {
              client.println("<h3>Pump is currently OFF.</h3>");
            }

            //"Feeding"
            client.println("<h2><mark><i>Feeding:</i></mark></h2>");

            client.print("<i><b>Reset Feeding Time:</b></i> <i>(choosing below)</i><br>");
            client.print("<i>Hour1:</i> <a href=\"/8\">8</a> <a href=\"/9\">9</a> <a href=\"/h10\">10</a> <a href=\"/11\">11</a> <a href=\"/12\">12</a> <a href=\"/13\">13</a> <a href=\"/14\">14</a><br>");
            client.print("<i>Hour2:</i> <a href=\"/15\">15</a> <a href=\"/16\">16</a> <a href=\"/17\">17</a> <a href=\"/18\">18</a> <a href=\"/19\">19</a> <a href=\"/h20\">20</a> <a href=\"/21\">21</a><br>");
            client.print("<i>Minute:</i> <a href=\"/m0\">00</a> <a href=\"/m10\">10</a> <a href=\"/m20\">20</a> <a href=\"/30\">30</a> <a href=\"/40\">40</a> <a href=\"/50\">50</a><br>");
            client.print("<h3>Feeding Time is ");
            client.print(feedHr1);
            client.print(":");
            if (feedMin == 0) { //"time format"
              client.print("00");
            }
            else {
              client.print(feedMin);
            }
            client.print(" & ");
            client.print(feedHr2);
            client.print(":");
            if (feedMin == 0) {
              client.print("00");
            }
            else {
              client.print(feedMin);
            }
            client.println(".</h3>");

            client.print("Feeding turbidity voltage is above ");
            client.print(turbidFeeding);
            client.println("V. <i>(The water is not too turbid to feed.)</i>");
            client.print("<h3>Feeding device is currently ");
            if (isFeeding) {
              client.println("ON!</h3>");
            }
            else {
              client.println("OFF.</h3>");
            }

            //"Lighting" function
            client.println("<h2><mark><i>Lighting:</i></mark></h2>");
            client.print("Turn <a href=\"/off\">OFF</a> LED lamp immediately<br>");
            client.print("Turn on <a href=\"/low\">LOW-level</a> light<br>");
            client.print("Turn on <a href=\"/medium\">MEDIUM-level</a> light<br>");
            client.print("Turn on <a href=\"/high\">HIGH-level</a> light<br>");
            client.print("<h3>LED is currently ");
            client.print(ledCondition);
            client.println(".</h3>");

            // END ~ ~ ~

            client.println("</body></html>");
            break;
          } else { //if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') { //if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        //remotely set "Feeding" time
        if (currentLine.endsWith("GET /8") > 0) {
          feedHr1 = 8;
        }
        if (currentLine.endsWith("GET /9") > 0) {
          feedHr1 = 9;
        }
        if (currentLine.endsWith("GET /h10") > 0) {
          feedHr1 = 10;
        }
        if (currentLine.endsWith("GET /11") > 0) {
          feedHr1 = 11;
        }
        if (currentLine.endsWith("GET /12") > 0) {
          feedHr1 = 12;
        }
        if (currentLine.endsWith("GET /13") > 0) {
          feedHr1 = 13;
        }
        if (currentLine.endsWith("GET /14") > 0) {
          feedHr1 = 14;
        }
        if (currentLine.endsWith("GET /15") > 0) {
          feedHr2 = 15;
        }
        if (currentLine.endsWith("GET /16") > 0) {
          feedHr2 = 16;
        }
        if (currentLine.endsWith("GET /17") > 0) {
          feedHr2 = 17;
        }
        if (currentLine.endsWith("GET /18") > 0) {
          feedHr2 = 18;
        }
        if (currentLine.endsWith("GET /19") > 0) {
          feedHr2 = 19;
        }
        if (currentLine.endsWith("GET /h20") > 0) {
          feedHr2 = 20;
        }
        if (currentLine.endsWith("GET /21") > 0) {
          feedHr2 = 21;
        }
        if (currentLine.endsWith("GET /m0") > 0) {
          feedMin = 0;
        }
        if (currentLine.endsWith("GET /m10") > 0) {
          feedMin = 10;
        }
        if (currentLine.endsWith("GET /m20") > 0) {
          feedMin = 20;
        }
        if (currentLine.endsWith("GET /30") > 0) {
          feedMin = 30;
        }
        if (currentLine.endsWith("GET /40") > 0) {
          feedMin = 40;
        }
        if (currentLine.endsWith("GET /50") > 0) {
          feedMin = 50;
        }

        //remotely control for "Lighting"
        if (currentLine.endsWith("GET /off") > 0) {
          analogWrite(ledPin, noLED);
          ledCondition = "OFF";
        }
        if (currentLine.endsWith("GET /low") > 0) {
          analogWrite(ledPin, lowLED);
          ledCondition = "in LOW brightness level";
        }
        if (currentLine.endsWith("GET /medium") > 0) {
          analogWrite(ledPin, mediumLED);
          ledCondition = "in MEDIUM brightness level";
        }
        if (currentLine.endsWith("GET /high") > 0) {
          analogWrite(ledPin, highLED);
          ledCondition = "in HIGH brightness level";
        }

      }
    }
    delay(1); //give the web browser time to receive data
    client.stop(); //close the connection:
  }
  delay(5000);
}


// All Helper Functions ~~
//for WiFi connection
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); //print the SSID of the network you're attached to
  IPAddress ip = WiFi.localIP(); //print your board's IP address
  Serial.print("IP Address: ");
  Serial.println(ip);
}

//for water temperature sensor
unsigned char temp_reset() {
  pinMode(tempPin, OUTPUT);
  digitalWrite(tempPin, LOW);
  delayMicroseconds(480);
  pinMode(tempPin, INPUT);
  delayMicroseconds(60);
  if (digitalRead(tempPin) == 0) {
    delayMicroseconds(480);
    return 0;
  }
  else {
    delayMicroseconds(480);
    return 1;
  }
}

void temp_write(char WRT) {
  char i = 0, Cmd = 0;
  Cmd = WRT;
  pinMode(tempPin, INPUT);
  for (i = 0; i < 8; i++) {
    if ((Cmd & (1 << i)) != 0) //bitshift left : variable<<number_of_bits
    {
      pinMode(tempPin, OUTPUT);
      digitalWrite(tempPin, LOW);
      delayMicroseconds(1);
      pinMode(tempPin, INPUT);
      delayMicroseconds(60);
    }
    else {
      pinMode(tempPin, OUTPUT);
      digitalWrite(tempPin, LOW);
      delayMicroseconds(60);
      pinMode(tempPin, INPUT);
    }
  }
}

unsigned char temp_read() {
  char i = 0, result = 0;
  pinMode(tempPin, INPUT);
  for (i = 0; i < 8; i++) {
    pinMode(tempPin, OUTPUT);
    digitalWrite(tempPin, LOW);
    delayMicroseconds(2);
    pinMode(tempPin, INPUT);
    if (digitalRead(tempPin) != 0)
      result |= 1 << i;
    delayMicroseconds(60);
  }
  return result;
}
