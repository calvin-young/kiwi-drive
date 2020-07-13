// Note: FORWARD is counter-clockwise

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
Adafruit_DCMotor *DCRed    = AFMS.getMotor(3); // Red
Adafruit_DCMotor *DCYellow = AFMS.getMotor(2); // Yellow
Adafruit_DCMotor *DCBlue   = AFMS.getMotor(1); // Blue
  
// Declare variables
uint8_t RED    = 3;
uint8_t BLUE   = 1;
uint8_t YELLOW = 2;
boolean autopilot_truth = 0;

// Analog pins
int proxy_sensor_reading = A3;

// Digital pins
int proxy_sensor_control = 7;

/******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ********/

void setup() {
  XBee.begin(9600); // Initialize XBee
  AFMS.begin();     // Initialize motor shield
  pinMode(proxy_sensor_control,OUTPUT);    // Initialize IR proximity sensor
  digitalWrite(proxy_sensor_control,HIGH); // Power IR proximity sensor
  
  // Notify operability
  XBee.println(F("Awwwww YEA we WORKIN!"));
}

/******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ********/

void loop() {
  proxy();
  drive();
}

/******** PROXY ******** PROXY ******** PROXY ******** PROXY ******** PROXY ******** PROXY ******** PROXY ********/

void proxy() {
  // Avoid hitting head-on obstacles
  
  // Compute distance as voltage (range approximately 0.5 to 3.1)
  int n     = 10;
  float sum = 0.0;
  for (int i=0; i<n; i++) sum += analogRead(proxy_sensor_reading);
  float proxy_voltage = sum / float(n) / 1023.0 * 5.0;
  
  // Back off if too close
  if (proxy_voltage >= 1.65) {
    kill();
    DCBlue->run(BACKWARD);
    DCBlue->setSpeed(150);
    DCYellow->run(FORWARD);
    DCYellow->setSpeed(150);
    delay(300);
    if (autopilot_truth) autopilot();
    else kill();
  }
}
  
  
/******** DRIVE ******** DRIVE ******** DRIVE ******** DRIVE ******** DRIVE ******** DRIVE ******** DRIVE ********/

void drive() {
  // Determine and control speed and direction
  if (XBee.available()) {
    char input = XBee.read(); // Retrieve user input from XBee
    switch (input) {          // Translate user input into command
      case 'w':
        kill();
        DCBlue->run(FORWARD);
        DCBlue->setSpeed(100);
        DCYellow->run(BACKWARD);
        DCYellow->setSpeed(100);
        break;
      case 's':
        kill();
        DCBlue->run(BACKWARD);
        DCBlue->setSpeed(100);
        DCYellow->run(FORWARD);
        DCYellow->setSpeed(100);
        break;
      case 'a':
        kill();
        DCRed->run(BACKWARD);
        DCRed->setSpeed(100);
        DCBlue->run(FORWARD);
        DCBlue->setSpeed(55);
        DCYellow->run(FORWARD);
        DCYellow->setSpeed(55);
        break;
      case 'd':
        kill();
        DCRed->run(FORWARD);
        DCRed->setSpeed(100);
        DCBlue->run(BACKWARD);
        DCBlue->setSpeed(55);
        DCYellow->run(BACKWARD);
        DCYellow->setSpeed(55);
        break;
      case 'e':
        kill();
        DCRed->run(FORWARD);
        DCRed->setSpeed(40);
        DCBlue->run(FORWARD);
        DCBlue->setSpeed(40);
        DCYellow->run(FORWARD);
        DCYellow->setSpeed(40);
        break;
      case 'q':
        kill();
        DCRed->run(BACKWARD);
        DCRed->setSpeed(40);
        DCBlue->run(BACKWARD);
        DCBlue->setSpeed(40);
        DCYellow->run(BACKWARD);
        DCYellow->setSpeed(40);
        break;
      case 'z':
        halt();
        autopilot_truth = false;
        break;
      case 'p':
        autopilot_truth = true;
        autopilot();
        break;
      default:
        kill();
    }
  }
}

/******** AUTO ******** AUTO ******** AUTO ******** AUTO ******** AUTO ******** AUTO ******** AUTO ********/

void autopilot() {
  // Drive around aimlessly
  int autopilot_speed = random(50,125);
  uint8_t autopilot_direction = random(0,2);
  if (autopilot_direction == 1) autopilot_direction = FORWARD;
  else autopilot_direction = BACKWARD;

  // Pick direction
  kill();
  DCRed->run(autopilot_direction);
  DCRed->setSpeed(autopilot_speed);
  DCBlue->run(autopilot_direction);
  DCBlue->setSpeed(autopilot_speed);
  DCYellow->run(autopilot_direction);
  DCYellow->setSpeed(autopilot_speed);
  delay(random(75,250));
  
  // Drive forward
  kill();
  DCBlue->run(FORWARD);
  DCBlue->setSpeed(autopilot_speed);
  DCYellow->run(BACKWARD);
  DCYellow->setSpeed(autopilot_speed);
}
  
/******** KILL ******** KILL ******** KILL ******** KILL ******** KILL ******** KILL ******** KILL ********/

void kill() {
  // Enable all motors to coast
  DCRed->run(RELEASE);
  DCYellow->run(RELEASE);
  DCBlue->run(RELEASE);
}

/******** HALT ******** HALT ******** HALT ******** HALT ******** HALT ******** HALT ******** HALT ********/

  
void halt() {
  // Engage the brakes on all motors
  DCRed->setSpeed(0);
  DCYellow->setSpeed(0);
  DCBlue->setSpeed(0);
}
