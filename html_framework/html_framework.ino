/*
 * (Feb02) Beginning of "framework": WiFi connection and basic HTML
 * REFERENCE: "WiFiWebServer" in WiFiNINA library (for the Arduino Uno WiFi R2 board) 
 */

#include <SPI.h>
#include <WiFiNINA.h>
#include "wifi_secret.h"

char ssid[] = SECRET_SSID; // CHANGE actual ssid & password in "wifi_secret.h"
char pass[] = SECRET_PASS;
int keyIndex = 0; //your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
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
          client.println("Refresh: 5");  //(?adjust with the delay time) refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          // REAL Contents part!
          client.println("<head>Kathleen's 2019 Summer <b>IoT</b> Project");
          client.println("<body>");
          client.println("<h1>Tortoise Smart Home</h1>");

          client.println("<h2><i>Function:</i></h2>");
          client.println("</body>");
          // END~

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


void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); //print the SSID of the network you're attached to
  
  IPAddress ip = WiFi.localIP(); //print your board's IP address
  Serial.print("IP Address: ");
  Serial.println(ip);
}
