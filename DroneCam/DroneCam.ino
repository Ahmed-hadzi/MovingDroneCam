#include <Servo.h>

#define ch5input 22

volatile uint16_t ch5;
uint16_t ch5_start;

#define camPin 12
int desiredCamAngle = 0;
int actualCamAngle=0;

Servo camServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(ch5input, INPUT);
  pinMode(camPin, OUTPUT);

  attachInterrupt(ch5input, RCchannel5, CHANGE);  
  camServo.attach(camPin, 1000, 2000);
}

void loop() {
  
  moveCamera();
  delay(10);
}

void moveCamera(){
  desiredCamAngle = constrain(ch5, 1000, 2000);
  Serial.println(desiredCamAngle);

  if(abs(desiredCamAngle-actualCamAngle)<10){
    return;
  } else{
  actualCamAngle = desiredCamAngle;
  if(desiredCamAngle < 950){ // NO SIGNAL
    desiredCamAngle = 1500;
  } else
  if(desiredCamAngle >= 950){ // Normal signal
    desiredCamAngle=map(desiredCamAngle, 1000, 2000, 1000, 2000);
  }
  Serial.println("Writing");
  camServo.writeMicroseconds(desiredCamAngle);
  }
}

/*void moveCamera(){
  desiredCamAngle = constrain(ch5, 1000, 2000);
  Serial.println(desiredCamAngle);
  camServo.writeMicroseconds(desiredCamAngle);
}*/

void RCchannel5() {
// If the pin is HIGH, start a timer
if (digitalRead(ch5input) == HIGH) {
ch5_start = micros();
} else {
// The pin is now LOW so output the difference
// between when the timer was started and now
ch5 = (uint16_t) (micros() - ch5_start);
}
}
