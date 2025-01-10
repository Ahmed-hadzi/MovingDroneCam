#include <Servo.h>

Servo cam;
int readChannel(int channelInput){
  int ch = pulseIn(channelInput, HIGH, 30000);
  return constrain(ch, 1000, 2000);
}
#define camPin 3 // Pin of the camera servo
#define camCh 17 // Camera channel pin on the receiver
int camAngle = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(camCh, INPUT);
  pinMode(camPin, OUTPUT);
  
  digitalWrite(13, HIGH);
  cam.attach(camPin, 1000, 2000);
  digitalWrite(13, LOW);
}

void loop() {
  camAngle = readChannel(camCh);
  Serial.println(camAngle);

  if(camAngle == 1000){ // NO SIGNAL
    camAngle = 1500;
  } else
  if(camAngle > 1100){ // Normal signal
    camAngle=map(camAngle, 1100, 2000, 1000, 2000);
  }

  cam.writeMicroseconds(camAngle);
  delay(25);
}
