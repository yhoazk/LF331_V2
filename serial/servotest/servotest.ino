#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  Serial.begin(115200);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  while(Serial.available() > 0) {
    int pos = Serial.parseInt();
    if (Serial.read() == '\n') {
      pos = constrain(pos, 0, 180);
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position      
    }
  }
}
