 /***************
 * (CHINA stage) simple use of micro servo
 * Specially used for the Feeding device (have TESTED actual effect～)
 * ADJUSTed to the suitable delay time and rotating angles!
 * Line Connection: Gery->GND, Red->VCC(+5V), Orange->Digital.
 * FIRST, always put the rotating board to the most central position
 * (i.e. parallel to the long side of the plastic "blue "board)
 ***************/
#include <Servo.h>

Servo myservo; //create servo object to control a servo

void setup() {
  myservo.attach(9); //attaches the servo on pin 9 to servo object
  myservo.write(0); //back to 0 degrees
  delay(1500); //for 1.5 seconds
}

void loop() {
  myservo.write(15); //goes to 15 degrees
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
  delay(1200);
}
