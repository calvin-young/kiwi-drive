// Call libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Create the motor objects
Adafruit_DCMotor *DCRed = AFMS.getMotor(3); // Red
Adafruit_DCMotor *DCYellow = AFMS.getMotor(2); // Yellow
Adafruit_DCMotor *DCBlue = AFMS.getMotor(1); // Blue
  
// Declare variables
uint8_t RED    = 3;
uint8_t BLUE   = 1;
uint8_t YELLOW = 2;

void setup() {
  AFMS.begin();
  delay(2500);
  

}

void loop() {
  // Drive forward then backward at speed 50 for 1.5 seconds
  drive(RED, 75, 750);
  delay(1500);
  drive(BLUE, 75, 750);
  delay(1500);
  drive(YELLOW, 75, 750);
  delay(1500);
}

void drive(uint8_t DIRECTION, int SPEED, int DELAY) {
  if (DIRECTION == RED) {
   // Set direction and speed
   DCYellow->run(FORWARD);
   DCBlue->run(BACKWARD);
   DCYellow->setSpeed(SPEED);
   DCBlue->setSpeed(SPEED);
  }
  
  if (DIRECTION == BLUE) {
   // Set direction and speed
   DCYellow->run(BACKWARD);
   DCRed->run(FORWARD);
   DCYellow->setSpeed(SPEED);
   DCRed->setSpeed(SPEED);
  }
  
  if (DIRECTION == YELLOW) {
   // Set direction and speed
   DCRed->run(BACKWARD);
   DCBlue->run(FORWARD);
   DCRed->setSpeed(SPEED);
   DCBlue->setSpeed(SPEED);
  }
  
  // Allow to drive for DELAY miliseconds then kill motors
  delay(DELAY);
  halt();
}

void spin(uint8_t DIRECTION) {
  // Set direction
  DCRed->run(DIRECTION);
  DCYellow->run(DIRECTION);
  DCBlue->run(DIRECTION);
  
  // Ramp up
  for (int i=25; i<100; i++) {
    DCRed->setSpeed(i);
    DCBlue->setSpeed(i);
    DCYellow->setSpeed(i);
    delay(25);
  }  
  
  // Ramp down
  for (int i=100; i>25; i--) {
  DCRed->setSpeed(i);
  DCBlue->setSpeed(i);
  DCYellow->setSpeed(i);
  delay(25);
  }
}

void kill() {
  DCRed->run(RELEASE);
  DCYellow->run(RELEASE);
  DCBlue->run(RELEASE);
}

void halt() {
  DCRed->setSpeed(0);
  DCBlue->setSpeed(0);
  DCYellow->setSpeed(0);
}
