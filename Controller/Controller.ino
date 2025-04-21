// Sends a PPM signal on pin 2

#include <RCEncoder.h>
#include <Wire.h>


#define OUTPUT_PIN 2
#define PPM_CHANNEL 1
#define BUTTON 3

float RateRoll, RatePitch, RateYaw;

float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;

float maxup, maxdown;

void gyro_signals(void){
  Wire.beginTransmission(0x68);

  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(0x68,6);

  int16_t AccXLSB = Wire.read()<<8 | Wire.read();
  int16_t AccYLSB = Wire.read()<<8 | Wire.read();
  int16_t AccZLSB = Wire.read()<<8 | Wire.read();

  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();

  Wire.requestFrom(0x68,6);

  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();

  RateRoll = (float)GyroX/65.5;
  RatePitch = (float)GyroY/65.5;
  RateYaw = (float)GyroZ/65.5;

  AccX = (float)AccXLSB/4096-0.04; // CALIBRATION X
  AccY = (float)AccYLSB/4096+0.02; // CALIBRATION Y
  AccZ = (float)AccZLSB/4096+0.07; // CALIBRATION Z

  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.14159/180);
  AnglePitch=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.14159/180);
}

void angle_calibration(){
  digitalWrite(13, LOW);
  Serial.println("STARTING UPPER ANGLE CALIBRATION");
  encoderWrite(PPM_CHANNEL, 2000);
  delay(500);
  digitalWrite(13, HIGH);
  Serial.println("Position your head in the uppermost position and click the button when ready");
  while(digitalRead(BUTTON)==0){
  }
  digitalWrite(13, LOW);
  Serial.println("Calibrating upper angle...");
  for (int i=0; i < 2000; i++){
    gyro_signals();
    maxup+=AngleRoll;
    delay(1);
  }
  maxup/=2000;
  Serial.print("UPPER ANGLE CALIBRATED  >>>  ");
  Serial.println(maxup);

  Serial.println("STARTING LOWER ANGLE CALIBRATION");
  encoderWrite(PPM_CHANNEL, 1000);
  delay(500);
  digitalWrite(13, HIGH);
  Serial.println("Position your head in the lowermost position and click the button when ready");
  while(digitalRead(BUTTON)==0){
  }
  digitalWrite(13, LOW);
  Serial.println("Calibrating lower angle...");
  for (int i=0; i < 2000; i++){
    gyro_signals();
    maxdown+=AngleRoll;
    delay(1);
  }
  maxdown/=2000;
  encoderWrite(PPM_CHANNEL, 1500);
  digitalWrite(13, HIGH);
  Serial.print("LOWER ANGLE CALIBRATED  >>>  ");
  Serial.println(maxdown);
  delay(500);
  digitalWrite(13, LOW);
}

void setup ()
{
  Serial.begin(9600);
  Serial.println();
  encoderBegin(OUTPUT_PIN);
  pinMode(3, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  Serial.println("Click the button to start head angle calibration!");
  while(digitalRead(BUTTON)==0){
  }
  angle_calibration();
  digitalWrite(13, LOW);
}


void loop ()
{
  gyro_signals();

  int pulseWidth = constrain(map(AngleRoll, maxdown, maxup, 1000, 2000),1000,2000);  // Microseconds being sent to the RC (1000 to 2000)
  encoderWrite(PPM_CHANNEL, pulseWidth); // encoderWrite(PPM_CHANNEL, SIGNAL);

  Serial.print("Roll Angle = ");
  Serial.print(AngleRoll);
  Serial.print("      |      Signal = ");
  Serial.println(pulseWidth);

  if(digitalRead(BUTTON)==1){
    angle_calibration();
  }

    delay(20);
}