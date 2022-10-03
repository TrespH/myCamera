#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>
//#include <Servo.h>

#define m1  3     
#define m2  4    
#define m3  5   
#define m4  6    

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
//Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
Adafruit_StepperMotor *stepper = AFMS.getStepper(4096, 2);
//Servo pitch_servo, yaw_servo;
/*AccelStepper stepper1(AccelStepper::FULL4WIRE, m1, m3, m2, m4);

int current_position = 0; 
int total_positions = 3;
int positions_to_go[3] =
{ 0,
  500,
  0,
};*/
int i = 0;

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  stepper->setSpeed(1000);
  /*stepper1.setMaxSpeed(800);
  stepper1.setSpeed(800);    
  stepper1.setAcceleration(900.0); 
  stepper1.setCurrentPosition(0);
  stepper1.moveTo(positions_to_go[current_position++]);*/
  //yaw_servo.attach(9);
  //pitch_servo.attach(9);
}

void loop() {
  stepper->step(50, FORWARD, DOUBLE);
  delay(1000);
  stepper->step(500, BACKWARD, DOUBLE);
  stepper->step(500, FORWARD, DOUBLE);
  stepper->step(500, BACKWARD, DOUBLE);
  stepper->step(500, FORWARD, DOUBLE);
  /*if (stepper1.distanceToGo() == 0) 
    stepper1.moveTo(positions_to_go[current_position++]); 
  stepper1.run();
  if (current_position > total_positions) { 
    current_position = 0;              
    delay(1000);                     
  }*/
  
  //rightMotor->setSpeed(70);
  //rightMotor->run(FORWARD);
  //leftMotor->setSpeed(50);
  //leftMotor->run(FORWARD);
  /*i++;
  for (int i = 0; i < 180; i++) {
    delay(100);
    yaw_servo.write(i);
  }
  for (int i = 180; i > 0; i--) {
    delay(100);
    yaw_servo.write(i);
  }*/
  /*yaw_servo.write(10); delay(4000);
  Serial.println(90);
  yaw_servo.write(90); delay(3000);
  Serial.println(92);
  yaw_servo.write(92); delay(3000);
  Serial.println(88);
  yaw_servo.write(88); delay(3000);
  yaw_servo.write(0); delay(4000);
  yaw_servo.write(500); delay(4000);
  pitch_servo.write(i);  */
}
