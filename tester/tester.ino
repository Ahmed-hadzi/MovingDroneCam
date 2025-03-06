#include <Servo.h>
#define camPin 12
Servo camServo;
void setup() {
    Serial.begin(115200);
      camServo.attach(camPin, 1000, 2000);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  camServo.writeMicroseconds(1000);
  delay(500);
  camServo.writeMicroseconds(1200);
  delay(500);
  camServo.writeMicroseconds(1500);
  delay(500);
  camServo.writeMicroseconds(1700);
  delay(500);
  camServo.writeMicroseconds(2000);
  delay(500);
}
