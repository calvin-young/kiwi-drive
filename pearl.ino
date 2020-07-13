// Call libraries
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Assign wireless pins [RX -> Dout, TX -> Din]
SoftwareSerial XBee(2, 3);

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

/******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ********/

void setup() {
  XBee.begin(9600);  // Initialize XBee
  AFMS.begin();      // Initialize motor shield
  pinMode(7,OUTPUT); // Initialize motor control pin
  
  XBee.println(F("Awwwww YEA we WORKIN!"));
  delay(2500);
}

/******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ********/

void loop() {
    if (XBee.available())
  {
    char beekey = XBee.read();
    switch (beekey)
    {
    case 'w':
      kill();
      DCBlue->run(FORWARD);
      DCBlue->setSpeed(250);
      DCYellow->run(BACKWARD);
      DCYellow->setSpeed(250);
      break;
    case 's':
      kill();
      DCBlue->run(BACKWARD);
      DCBlue->setSpeed(50);
      DCYellow->run(FORWARD);
      DCYellow->setSpeed(250);
      break;
    case 'a':
      kill();
      DCRed->run(BACKWARD);
      DCRed->setSpeed(250);
      DCBlue->run(FORWARD);
      DCBlue->setSpeed(125);
      DCYellow->run(FORWARD);
      DCYellow->setSpeed(125);
      break;
    case 'd':
      kill();
      DCRed->run(FORWARD);
      DCRed->setSpeed(250);
      DCBlue->run(BACKWARD);
      DCBlue->setSpeed(125);
      DCYellow->run(BACKWARD);
      DCYellow->setSpeed(125);
      break;
    case 'e':
      kill();
      DCRed->run(FORWARD);
      DCRed->setSpeed(125);
      DCBlue->run(FORWARD);
      DCBlue->setSpeed(125);
      DCYellow->run(FORWARD);
      DCYellow->setSpeed(125);
      break;
    case 'q':
      kill();
      DCRed->run(BACKWARD);
      DCRed->setSpeed(125);
      DCBlue->run(BACKWARD);
      DCBlue->setSpeed(125);
      DCYellow->run(BACKWARD);
      DCYellow->setSpeed(125);
      break;
    case 'z':
      kill();
      break;
    default:
      kill();
    }
  }
}

/******** KILL ******** KILL ******** KILL ******** KILL ******** KILL ******** KILL ******** KILL ********/

void kill() {
  DCRed->run(RELEASE);
  DCYellow->run(RELEASE);
  DCBlue->run(RELEASE);
}
