// Call libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Create the motor objects
Adafruit_DCMotor *DCRed = AFMS.getMotor(1); // Red
Adafruit_DCMotor *DCGreen = AFMS.getMotor(2); // Green
Adafruit_DCMotor *DCBlue = AFMS.getMotor(3); // Blue

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  
  delay(2500);
  
  // Drive forward then backward at speed 50 for 1.5 seconds
  drive(FORWARD, 150, 750);
  delay(1500);
  drive(BACKWARD, 150, 750);
  delay(1500);  
}

void loop() {
  spin(FORWARD);
  delay(500);
  spin(BACKWARD);
  delay(500);
}

void kill() {
  DCRed->run(RELEASE);
  DCGreen->run(RELEASE);
  DCBlue->run(RELEASE);
}

void drive(uint8_t DIRECTION, int SPEED, int DELAY) {
  //
  // I need to figure out how to do this section properly
  //
  
  if (DIRECTION == FORWARD) {
   // Set direction
   DCGreen->run(FORWARD);
   DCBlue->run(BACKWARD);
  
   // Drive in Red direction
   DCGreen->setSpeed(SPEED);
   DCBlue->setSpeed(SPEED);
  }
  
  if (DIRECTION == BACKWARD) {
   // Set direction
   DCGreen->run(BACKWARD);
   DCBlue->run(FORWARD);
  
   // Drive in Red direction
   DCGreen->setSpeed(SPEED);
   DCBlue->setSpeed(SPEED);
  }
  
  // Pause then kill motors
  delay(DELAY);
  kill();
}

void spin(uint8_t DIRECTION) {
  // Set direction
  DCRed->run(DIRECTION);
  DCGreen->run(DIRECTION);
  DCBlue->run(DIRECTION);
  
  // Ramp up
  for (int i=25; i<150; i++) {
    DCRed->setSpeed(i);
    DCBlue->setSpeed(i);
    DCGreen->setSpeed(i);
    delay(10);
  }  
  
  // Ramp down
  for (int i=150; i>25; i--) {
  DCRed->setSpeed(i);
  DCBlue->setSpeed(i);
  DCGreen->setSpeed(i);
  }
}
