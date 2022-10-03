#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
//Adafruit_StepperMotor *stepper = AFMS.getStepper(4096, 2);
Servo yaw_servo, pitch_servo;

String controllo [4];
int i = 0, fg = 0, yaw = 0, pitch = 0, right = 0, left = 0;
int pitch_old = 0, diff = 0;
bool show_serial = false;
void setup() {
  Serial.begin(9600);
  AFMS.begin();
  stepper->setSpeed(100);
  yaw_servo.attach(9);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == "") if (show_serial) Serial.println("Nada");
    if (c != '+') {
      if (c != ';') controllo[i] += c;
      else {
        i = 0;
        muovi();
      }
    } else i++;
  }
}

void muovi() {
  yaw = controllo[1].toInt();
  pitch = controllo[0].toInt();
  right = controllo[2].toInt();
  left = controllo[3].toInt();

  /*if (fg == 0) {
    fg = 1;
    diff = 0;
  } else diff = (pitch - pitch_old) * 2.5;
  if (diff > 0) stepper->step(diff, FORWARD, SINGLE);
  else if (diff < 0) stepper->step(-diff, BACKWARD, SINGLE);
  pitch_old = pitch;*/
  
  pitch_servo.write(pitch);
  yaw_servo.write(yaw);
  dcs_start();
  
  if (show_serial) {
    Serial.println(pitch);
    Serial.println(yaw);
    Serial.println();
  }
  for (int l = 0; l < 4 ; l++) controllo[l] = "";
}

void dcs_start() {
  right = (right - 127) * 2;
  left = (left - 127) * 2;
  if (right > -10 && right < 10) right = 0;
  if (left > -10 && left < 10) left = 0;
  if (right > 254) right = 254; if (right < -254) right = -254;
  if (left > 254) left = 254; if (left < -254) left = -254;
  rightMotor->setSpeed(abs(right));
  leftMotor->setSpeed(abs(left));
  if (right > 0) rightMotor->run(FORWARD);
  else rightMotor->run(BACKWARD);
  if (left > 0) leftMotor->run(FORWARD);
  else leftMotor->run(BACKWARD);
  if (show_serial) {
    Serial.print(left);
    Serial.print("\t");
    Serial.println(right);
  }
}
