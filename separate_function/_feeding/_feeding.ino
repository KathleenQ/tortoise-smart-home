/**********************
 * (Jan28) [for Feeding]
 * Combination use of real-time clock, micro servo, (turbidity sensor) and other corresponding physical feeding devices
 * Feed at pre-set fixed time point (not decide whether can be changed through website~)
 * if the water is not too turbid (decide to add currently),
 * the food will be released, depending on whether the hole of the food container will be hid 
 * with the help of micro servo (connection with other hand-made physical device).
 * CODEs union of: "realTime_clock", "turbiditySensor", "microServo".
 * 
 * "realTime_clock":
 * SET the current time and date in specific FORMAT: e.g
 * | Seconds 00 | Minutes 39 | Hours 22 [0-24] | DayOfWeek 07 (i.e.Sunday) | DayOfMonth 27 | Month 01 | Year 2019 |
 *    --> Real Time: 27/1/2019 Sun 22:39:0
 *    
 * "microServo":
 * (ADJUSTed to suitable delay time & rotating angles) always put the rotating board to the most central position
 * Line Connection: Gery->GND, Red->VCC(+5V), Orange->Digital.
 * 
 * "turbiditySensor":
 * Less turbid, output VALUE is larger!
 * Line Connection: Black->GND, Red->5V, Blue->AnalogIn.
 **********************/

#include <MyRealTimeClock.h>
#include <Servo.h>

//String weekDay(uint8_t dayofweek); //NOT used for current simple "time point" test 

MyRealTimeClock RTC(6, 7, 8); // Assign digital pins (CLK~, I/O, RST)
Servo myservo;

const int turbidityPin = A1;
float turbidityVol = 0.0; //convert the "turbidity value" to a voltage (0-5V)

void setup() {
  Serial.begin(9600); //(?)
  RTC.setDS1302Time(40, 59, 10, 07, 27, 01, 2019); //RESET real time!
  myservo.attach(9); //attaches servo to digital 9~
  myservo.write(0); //back to 0 degrees
}

void loop() {
  RTC.updateTime();
  int turbidityValue = analogRead(turbidityPin);
  turbidityVol = turbidityValue * (5.0 / 1024.0);
  
  Serial.print("Turbidity Voltage:");
  Serial.print(turbidityVol);
  
  if(turbidityVol > 1.0) //(1.0 just for TEST) NOT very turbid (i.e. with large turbidity voltage) --> judge turbidity first~
  {
    Serial.print(" || Real Time:"); 
    Serial.print(RTC.dayofmonth);
    Serial.print("/"); 
    Serial.print(RTC.month);
    Serial.print("/");
    Serial.print(RTC.year);
//    Serial.print(" ");
//    Serial.print(weekDay(RTC.dayofweek));
    Serial.print(" ");
    Serial.print(RTC.hours);
    Serial.print(":");
    Serial.print(RTC.minutes);
    Serial.print(":");
    Serial.print(RTC.seconds);

    if(RTC.minutes == 0 && RTC.seconds < 30 && (RTC.hours == 11 || RTC.hours == 20)) //judge time then~
    { //(test) plan to judge feeding at 11:00:00 - 11:00:30 and 20:00:00 - 20:00:30 everyday!
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
      Serial.println(" || FEED !");
    }
    else
    {
      delay(2000); //(?) future decision ('unify' with the FEED condition~)
      Serial.println(" || NOT TIME to feed");
    }
  }
  else
  {
    Serial.println(" || TURBID, NO feed");
  }
  delay(1000);
}

//String weekDay(uint8_t dayofweek)
//{
//  if(dayofweek == 1){
//    return "Mon";
//  }
//  else if(dayofweek == 2){
//    return "Tue";
//  }
//  else if(dayofweek == 3){
//    return "Wed";
//  }
//  else if(dayofweek == 4){
//    return "Thu";
//  }
//  else if(dayofweek == 5){
//    return "Fri";
//  }
//  else if(dayofweek == 6){
//    return "Sat";
//  }
//  else if(dayofweek == 7){
//    return "Sun";
//  }
//}
