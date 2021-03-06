/********************
 * (Jan29) [for "Temperature"]
 * Use of Water-proof Temperature Sensor & OLED
 * In the "Temperature" function, we will get temperature sensor data of water and show it on the OLED screen,
 * we will later send real-time message to the website, to let owner take further actions themselves (open heater remotely...).
 * (OLED brings convenience for daily life, if the owner can read water temperature immediately ~)
 * 
 * Temperature sensor:
 * Line Connection: S->digital port, '-'->GND, '+'->5V.
 * REFERENCE: "example1_Arduino" in http://www.alsrobot.cn/wiki/index.php/SKU:RB-02S160_DS18B20_%E6%B0%B4%E6%B8%A9%E4%BC%A0%E6%84%9F%E5%99%A8
 * OLED:
 * REFERENCE: https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
 * Line Connection: VCC->3.3V, GND, SCL, SDA.
 ********************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

unsigned char temp_reset();
void temp_write(char WRT);
unsigned char temp_read();

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
const int tempPin = 12;
float temperature = 0.0;

void setup()
{
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (for the 128x32)(initializing the display)
  Serial.begin(9600); //(?)
}

void displayTemp(){
  delay(2000); //(?) website test~
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,5);
  display.print("Water Temperature:");
  display.setTextSize(2);
  display.setCursor(0,15);
  display.print(temperature);
  display.print(" C");
}
void loop()
{
  if(!temp_reset())
  {
    temp_write(0xCC); //"Skip_ROM"
    temp_write(0x44); //"Convert_T"
    delay(750); //test(?)
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
  displayTemp();
  display.display();
}

// All HELPER methods~

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
