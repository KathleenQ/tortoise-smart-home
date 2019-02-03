/****************
 * (Feb03) Add the 1st function: Temperature (combine "_temperature" file)
 * Use of Water-proof Temperature Sensor & OLED
 * We will get data from water temperature sensor, show it on OLED screen, and send real-time message to the website, 
 * to let owner take further actions themselves (open heater remotely...).
 * 
 * Line CONNECTION: OLED: VCC->3.3V.
 ****************/
//"WiFi Connection"
#include <SPI.h>
#include <WiFiNINA.h>
#include "wifi_secret.h"
//"Temperature"
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

unsigned char temp_reset();
void temp_write(char WRT);
unsigned char temp_read();

//"WiFi Connection"
char ssid[] = SECRET_SSID; // CHANGE actual ssid & password in "wifi_secret.h"
char pass[] = SECRET_PASS;
int keyIndex = 0; //your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);
//"Temperature"
//#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);
const int tempPin = 12;
float temperature = 0.0;

void setup() {
//  Wire.begin();
//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (for the 128x32)(initializing the display)
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for serial port to connect (needed for native USB port only)
  }

  //Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true); //don't continue
  }
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  //Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID:");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass); //(if connect to WPA/WPA2 network, change this line~)
    delay(10000); //wait 10s for connection
  }
  server.begin();
  printWifiStatus(); //you're connected now, so print out the status
}

void loop() {
  WiFiClient client = server.available(); //listen for incoming clients
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
          client.println("Refresh: 5");  //(?adjust with the delay time) refresh the page automatically every 5s
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          // REAL Contents part ! ! !
          client.println("<head>Kathleen's 2019 Summer <b>IoT</b> Project");
          client.println("<body>");
          client.println("<h1>Tortoise Smart Home</h1>");

          //"Temperture" function
          if(!temp_reset()) {
            temp_write(0xCC); //"Skip_ROM"
            temp_write(0x44); //"Convert_T"
            delay(50); //test(?)
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
//          displayTemp();
//          display.display();
          client.println("<h2><mark><i>Temperature:</i></mark></h2>");
          client.print("<h4>Current Water Temperature is ");
          client.print(temperature);
          client.println(" C.</h4>");
          
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
}


// Helper Function for "WiFi Connection"
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); //print the SSID of the network you're attached to
  
  IPAddress ip = WiFi.localIP(); //print your board's IP address
  Serial.print("IP Address: ");
  Serial.println(ip);
}

// Helper Function for "Temperature" function
//void displayTemp(){ //OLED shown
//  delay(2000); //(?) website test~
//  display.clearDisplay();
//  display.setTextColor(WHITE);
//  display.setTextSize(1);
//  display.setCursor(0,5);
//  display.print("Water Temperature:");
//  display.setTextSize(2);
//  display.setCursor(0,15);
//  display.print(temperature);
//  display.print(" C");
//}

unsigned char temp_reset() 
{
  pinMode(tempPin, OUTPUT);
  digitalWrite(tempPin, LOW);
  delayMicroseconds(480);
  pinMode(tempPin, INPUT);
  delayMicroseconds(60);
  if(digitalRead(tempPin) == 0)
  {
    delayMicroseconds(480);
    return 0; 
  }
  else
  {
    delayMicroseconds(480);
    return 1; 
  }
}

void temp_write(char WRT)
{
  char i = 0, Cmd = 0;
  Cmd = WRT;
  pinMode(tempPin, INPUT);
  for(i = 0; i< 8; i++)
  {
    if((Cmd & (1<<i)) != 0) //Bitshift Left : variable<<number_of_bits
    {
      pinMode(tempPin, OUTPUT);
      digitalWrite(tempPin, LOW);
      delayMicroseconds(1);
      pinMode(tempPin, INPUT);
      delayMicroseconds(60);
    }
    else
    {
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
  for(i = 0; i < 8; i++)
  {
    pinMode(tempPin, OUTPUT);
    digitalWrite(tempPin, LOW);
    delayMicroseconds(2);
    pinMode(tempPin, INPUT);
    if(digitalRead(tempPin) != 0)
      result |= 1<<i;
    delayMicroseconds(60);
  }
  return result;
}
