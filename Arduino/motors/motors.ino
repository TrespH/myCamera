#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#define SW_pin 7 
#define X_pin A1 
#define Y_pin A0

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);

long x_pad, y_pad;
int rightSpeed, leftSpeed;
void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);           
  Serial.println("Motor Test - Pad Analogico");
  AFMS.begin(); 
}

void loop() {
  x_pad = map(analogRead(X_pin), 0, 1023, 0, 500) - 247;
  y_pad = map(analogRead(Y_pin), 0, 1023, 0, 500) - 245;
  Serial.print(x_pad);
  Serial.print("\t");
  Serial.print(y_pad);
  Serial.print("\t");
  //1
  if (y_pad > 10 && x_pad > 10) {
    if (y_pad > x_pad) {
      leftSpeed = y_pad;
      rightSpeed = 0;
    }
    else {
      leftSpeed = x_pad;
      rightSpeed = abs(x_pad - y_pad);
    }
  }
  //2
  else if (y_pad < -10 && x_pad > 10) {
    if (abs(y_pad) > x_pad) {
      rightSpeed = abs(y_pad);
      leftSpeed = 0;
    }
    else {
      rightSpeed = x_pad;
      leftSpeed = abs(x_pad + y_pad);
    }
  }
  //3
  else if (y_pad < -10 && x_pad < -10) {
    rightSpeed = abs(y_pad) - abs(x_pad);
    if (abs(y_pad) >  abs(x_pad)) leftSpeed = 0;
    else leftSpeed = x_pad - y_pad;
  }
  //4
  else if (y_pad > 10 && x_pad < -10) {
    leftSpeed = y_pad - abs(x_pad);
    if (y_pad > abs(x_pad)) rightSpeed = 0;
    else rightSpeed = y_pad + x_pad;
  }
  
  else {
    rightSpeed = x_pad;
    leftSpeed = rightSpeed;
  }
  
  
  if (x_pad < 10 && x_pad > -10) {
    if (y_pad > 10) {
      rightSpeed = 0;
      leftSpeed = abs(y_pad);
    }
    else if (y_pad < -10) {
      rightSpeed = abs(y_pad);
      leftSpeed = 0;
    }
    else leftSpeed = 0;
  }
  
  rightMotor->setSpeed(abs(rightSpeed));
  leftMotor->setSpeed(abs(leftSpeed));
  
  if (rightSpeed > 0) rightMotor->run(FORWARD);
  else rightMotor->run(BACKWARD);
  if (leftSpeed > 0) leftMotor->run(FORWARD);
  else leftMotor->run(BACKWARD);
  
  Serial.print(leftSpeed);
  Serial.print("\t");
  Serial.println(rightSpeed);
}
