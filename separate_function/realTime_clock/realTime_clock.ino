/*****************
 * (Jan27) basic use of Real-time Clock module [For periodic "Feeding" & website message uploading]
 * REFERENCE: https://www.14core.com/the-ds1302-real-time-clock-on-arduino/
 * (Also get "MyRealTimeClock" DS1302 library from the above website~)
 * Line Connection: VCC(+5V); GND; RTC(~CLK, I/O, RST) (e.g. RTC(6, 7, 8)).
 * SET the current time and date in specific FORMAT: 
 * | Seconds 00 | Minutes 39 | Hours 22 [0-24] | DayOfWeek 7 (i.e.Sunday) | DayOfMonth 27 | Month 01 | Year 2019 |
 * --> print out as "Real Time: 27/1/2019 Sun 22:39:0"
 *****************/

#include <MyRealTimeClock.h>

String weekDay(uint8_t dayofweek);

MyRealTimeClock RTC(6, 7, 8); // Assign digital pins (CLK, I/O, RST)

void setup() {
Serial.begin(9600); //(?)
RTC.setDS1302Time(00, 39, 22, 07, 27, 01, 2019); //RESET real time!
}

void loop() {
RTC.updateTime(); //Allow update of variables for time / accessing individual element
Serial.print("Real Time: "); 
Serial.print(RTC.dayofmonth);
Serial.print("/"); 
Serial.print(RTC.month);
Serial.print("/");
Serial.print(RTC.year);
Serial.print(" ");
Serial.print(weekDay(RTC.dayofweek));
Serial.print(" ");
Serial.print(RTC.hours);
Serial.print(":");
Serial.print(RTC.minutes);
Serial.print(":");
Serial.println(RTC.seconds);
delay(5000); //(?)
}

String weekDay(uint8_t dayofweek) // "helper function" to show dayOfWeek in short-form string 
{
  if(dayofweek == 1){
    return "Mon";
  }
  else if(dayofweek == 2){
    return "Tue";
  }
  else if(dayofweek == 3){
    return "Wed";
  }
  else if(dayofweek == 4){
    return "Thu";
  }
  else if(dayofweek == 5){
    return "Fri";
  }
  else if(dayofweek == 6){
    return "Sat";
  }
  else if(dayofweek == 7){
    return "Sun";
  }
}
