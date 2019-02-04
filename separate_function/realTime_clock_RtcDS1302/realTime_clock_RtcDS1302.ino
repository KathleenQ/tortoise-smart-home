/***************
 * (Feb04) Try new library for Real-time Clock module, to enable use on UNO WIFI R2 board
 * Using "reference" DS1302 Library: "Rtc_by_Makuna" folder
 ***************/

#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(7,6,8); // Connection: I/O, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

void setup () 
{
  Serial.begin(9600);
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
}

void loop () 
{
  RtcDateTime now = Rtc.GetDateTime();
  Serial.print("Real Time: "); 
  Serial.print(now.Day());
  Serial.print("/"); 
  Serial.print(now.Month());
  Serial.print("/");
  Serial.print(now.Year());
  Serial.print(" ");
  Serial.print(now.Hour());
  Serial.print(":");
  Serial.print(now.Minute());
  Serial.print(":");
  Serial.println(now.Second());
  delay(5000); //(?)
}
