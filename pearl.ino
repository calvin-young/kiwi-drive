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
uint8_t RED     = 3;
uint8_t BLUE    = 1;
uint8_t YELLOW  = 2;
int input_speed = 0;
boolean autopilot_truth = 0;

// Analog pins
int proxy_sensor_reading = A3;

// Digital pins
int proxy_sensor_control = 7;

/******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ******** SETUP ********/

void setup() {
  XBee.begin(9600); // Initialize XBee
  AFMS.begin();     // Initialize motor shield
  pinMode(proxy_sensor_control,OUTPUT);     // Initialize IR proximity sensor
  digitalWrite(proxy_sensor_control,HIGH);  // Power IR proximity sensor
  XBee.println(F("Awwwww YEA we WORKIN!")); // Notify operability
}

/******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ******** LOOP ********/

void loop() {
  proximity_check();
  
  // Determine and control speed and direction
  if (XBee.available()) {
    char input = XBee.read(); // Retrieve user input from XBee
    switch (input) {          // Translate user input into command
      // Speed control
      case '1':
        input_speed = 50;
        break;
      case '2':
        input_speed = 75;
        break;
      case '3':
        input_speed = 100;
        break;
      case '4':
        input_speed = 125;
        break;
      case '5':
        input_speed = 150;
        break;
      case '[':
        input_speed -= 10;
        break;
      case ']':
        input_speed += 10;
        break;
      // Direction control
      case 'w':
        kill();
        DCBlue->run(FORWARD);
        DCBlue->setSpeed(input_speed);
        DCYellow->run(BACKWARD);
        DCYellow->setSpeed(input_speed);
        break;
      case 's':
        kill();
        DCBlue->run(BACKWARD);
        DCBlue->setSpeed(input_speed);
        DCYellow->run(FORWARD);
        DCYellow->setSpeed(input_speed);
        break;
      case 'a':
        kill();
        DCRed->run(BACKWARD);
        DCRed->setSpeed(input_speed);
        DCBlue->run(FORWARD);
        DCBlue->setSpeed(input_speed * 0.55);
        DCYellow->run(FORWARD);
        DCYellow->setSpeed(input_speed * 0.55);
        break;
      case 'd':
        kill();
        DCRed->run(FORWARD);
        DCRed->setSpeed(input_speed);
        DCBlue->run(BACKWARD);
        DCBlue->setSpeed(input_speed * 0.55);
        DCYellow->run(BACKWARD);
        DCYellow->setSpeed(input_speed * 0.55);
        break;
      case 'e':
        kill();
        DCRed->run(FORWARD);
        DCRed->setSpeed(input_speed * 0.40);
        DCBlue->run(FORWARD);
        DCBlue->setSpeed(input_speed * 0.40);
        DCYellow->run(FORWARD);
        DCYellow->setSpeed(input_speed * 0.40);
        break;
      case 'q':
        kill();
        DCRed->run(BACKWARD);
        DCRed->setSpeed(input_speed * 0.40);
        DCBlue->run(BACKWARD);
        DCBlue->setSpeed(input_speed * 0.40);
        DCYellow->run(BACKWARD);
        DCYellow->setSpeed(input_speed * 0.40);
        break;
      // Autopilot or manual override
      case 'z':
        halt();
        autopilot_truth = false;
        break;
      case 'p':
        autopilot_truth = true;
        autopilot();
        break;
      // Default behavior if any other key is pressed
      default:
        kill();
    }
  }
}

/******** PROXIMITY CHECK ******** PROXIMITY CHECK ******** PROXIMITY CHECK ******** PROXIMITY CHECK ********/

void proximity_check() {
  // Avoid hitting head-on obstacles
  // Compute distance as voltage (range approximately 0.5 to 3.1)
  int n     = 10;
  float sum = 0.0;
  for (int i=0; i<n; i++) sum += analogRead(proxy_sensor_reading);
  float proxy_voltage = sum / float(n) / 1023.0 * 5.0;
  
  // Back up if too close
  if (proxy_voltage >= 1.65) {
    kill();
    DCBlue->run(BACKWARD);
    DCBlue->setSpeed(111);
    DCYellow->run(FORWARD);
    DCYellow->setSpeed(111);
    delay(300);
    kill();
    if (autopilot_truth) autopilot();
  }
}

/******** AUTOPILOT ******** AUTOPILOT ******** AUTOPILOT ******** AUTOPILOT ******** AUTOPILOT ********/

void autopilot() {
  // Drive around aimlessly
  kill();

  // Randomize rotation direction (left or right)
  uint8_t autopilot_direction = random(0,2);
  if (autopilot_direction == 1) autopilot_direction = FORWARD;  // right
  else                          autopilot_direction = BACKWARD; // left
  DCRed->run(autopilot_direction);
  DCBlue->run(autopilot_direction);
  DCYellow->run(autopilot_direction);
  
  // Randomize speed (within a range of input_speed
  int autopilot_speed = random(input_speed - 10, input_speed + 10);
  DCRed->setSpeed(input_speed - 10);
  DCBlue->setSpeed(input_speed - 10);
  DCYellow->setSpeed(input_speed - 10);
  
  // Randomize rotation duration
  delay(random(75,350));
  kill();
  
  // Drive forward
  DCBlue->run(FORWARD);
  DCYellow->run(BACKWARD);
  DCBlue->setSpeed(autopilot_speed);
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
