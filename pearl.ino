/*
PEARL - The omnidirectional ball-bot

ME 122 Summer 2015

Steven Ortiz
Abdulqader Alqadri
Clay Johnson
Calvin Young

VERSION HISTORY

0.01 15-7-28
Basic DC motor controls

*/

#include <Adafruit_MotorShield.h>


int motor = 13;

void setup() {
  pinMode(motor, OUTPUT);
}

void loop() {
  digitalWrite(motor, HIGH);
  delay(5000);
  digitalWrite(motor, LOW);
  delay(5000);
}
