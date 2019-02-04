/****************
   (Feb05) Add the 2nd function: Water Changing (combine "_water_changing" & "realTime_clock_RtcDS1302")
   Use: Water-proof temperature sensor, Turbidity sensor, Pump (with MOS module),
        Real-time clock module.

   Brief Explanation:
   We will use real time to seperate various functions~
   EVEN Minute:
   Temp: We will get data from water temperature sensor, show it on OLED screen,
   and send real-time message to the website, to let owner take further actions themselves.
   ODD Minute:
   Water Change: If more turbid than "normal level", turn ON the pump.
   (More turbid, output VALUE is smaller.)

   Line CONNECTION:
   (OLED: VCC->3.3V;) Turbidity Sensor:Black->GND,Red->5V,Blue->AnalogIn;
   Pump with MOS module:{smaller}'+'->I/O,{larger}'+'->5V,{both}'-'->GND;
   Real-time clock: myWire(I/O, ~CLK, RST).
 ****************/

//WiFi Connection
#include <SPI.h>
#include <WiFiNINA.h>
#include "wifi_secret.h"
//Real-time Clock
#include <ThreeWire.h>
#include <RtcDS1302.h>

unsigned char temp_reset();
void temp_write(char WRT);
unsigned char temp_read();


//WiFi Connection
char ssid[] = SECRET_SSID; // CHANGE actual ssid & password in "wifi_secret.h"
char pass[] = SECRET_PASS;
int keyIndex = 0; //your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

//"Temperature"
const int tempPin = 12;
float temperature = 0.0;

//"Water Changing"
const int turbidityPin = A1;
const int pumpPin = 11;
const float turbidityStandard = 3; //(?) Check later! Ideally(scientifically) set "< 4.1-0.3=3.8 V".
float turbidityVol = 0.0;
boolean pumpOn = false; //use for website condition shown

//Real-time Clock
ThreeWire myWire(7, 6, 8); // Connection: myWire(I/O, CLK, RST)
RtcDS1302<ThreeWire> Rtc(myWire);


void setup() {
  //  Wire.begin();
  //  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (for the 128x32)(initializing the display)
  pinMode(pumpPin, OUTPUT);

  Serial.begin(9600); //(?) initialize serial and wait for port to open:

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

  //WiFi Connection
  while (!Serial) {
    ; //wait for serial port to connect (needed for native USB port only)
  }
  //check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true); //don't continue
  }
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  //attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass); //(if connect to WPA/WPA2 network, change this line~)
    delay(10000); //wait 10s for connection
  }
  server.begin();
  printWifiStatus(); //you're connected now, so print out the status

}


void loop() {
  WiFiClient client = server.available(); //listen for incoming clients

  //Real-time Clock
  RtcDateTime now = Rtc.GetDateTime();

  if (now.Minute() % 2 == 0) //"even" minute -> temperature, NO water changing
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
      temperature = (float)temp / 100; //Get the REAL "temperature" value~
      Serial.print("Temperature:");
      Serial.println(temperature);
    }
    
  }
  else //"odd" minute -> water changing, No temperature
  {
    //"Water Changing" function
    int turbidityValue = analogRead(turbidityPin);
    turbidityVol = turbidityValue * (5.0 / 1024.0); //convert the "read turbidity value" (0-1023) to voltage (0-5V)
    Serial.print("Turbidity Voltage:");
    Serial.print(turbidityVol);
    if (turbidityVol < turbidityStandard) {
      digitalWrite(pumpPin, HIGH); //turn ON pump
      Serial.println(" Pump ON");
      pumpOn = true;
    }
    else {
      digitalWrite(pumpPin, LOW); //turn OFF pump
      Serial.println(" Pump OFF");
      pumpOn = false;
    }
    
  }

  //WiFi Connection
  if (client) {
    boolean currentLineIsBlank = true; //a http request ends with a blank line
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank)
        { //if reaching the end of line and the line is blank, the http request has ended, you can send a reply
          client.println("HTTP/1.1 200 OK"); //send a standard http response header
          client.println("Content-Type: text/html");
          client.println("Connection: close");  //the connection will be closed after completion of the response
          client.println("Refresh: 15");  //(?adjust with the delay time) refresh the page automatically every 15s (?)
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

          //"Water Changing"
          client.println("<h2><mark><i>Water Changing:</i></mark></h2>");
          client.print("Current water turbidity voltage is ");
          client.print(turbidityVol);
          client.println("V; <br />");
          client.print("Standard water turbidity voltage is above ");
          client.print(turbidityStandard);
          client.println("V.");
          if (pumpOn) {
            client.println("<h3>Pump is currently ON!</h3>");
          }
          else {
            client.println("<h3>Pump is currently OFF.</h3>");
          }

          // END ~ ~ ~

          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          //you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          //you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    delay(1); //give the web browser time to receive data
    client.stop(); //close the connection:
  }
  delay(5000); //(?)
}


// All Helper Functions ~~~
//for WiFi connection
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); //print the SSID of the network you're attached to

  IPAddress ip = WiFi.localIP(); //print your board's IP address
  Serial.print("IP Address: ");
  Serial.println(ip);
}

//for water temperature sensor
unsigned char temp_reset()
{
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

void temp_write(char WRT)
{
  char i = 0, Cmd = 0;
  Cmd = WRT;
  pinMode(tempPin, INPUT);
  for (i = 0; i < 8; i++) {
    if ((Cmd & (1 << i)) != 0) //Bitshift Left : variable<<number_of_bits
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

unsigned char temp_read()
{
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
