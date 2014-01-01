// -*- c -*-
// myRobotRemote Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013 Dave Sieh
//
// See LICENSE.txt for details.

#include <RobotMotor.h>
#include <LEDBlinker.h>
#include <SoftServo.h>
#include <IrSensors.h>
#include <IRremote.h>
#include <PingSensor.h>
#include <Look.h>
#include <Move.h>
#include "Remote.h"

// Analog Pins
#define LEFT_IR_SENSOR_PIN 0
#define RIGHT_IR_SENSOR_PIN 1
#define CENTER_IR_SENSOR_PIN 2

// Digital Pins
#define SWEEP_SERVO_PIN 9
#define PING_SENSOR_PIN 10
#define LED_PIN 13

const byte irReceivePin = A3;

IRrecv irrecv(irReceivePin);
LEDBlinker blinker(LED_PIN);
SoftServo sweepServo(SWEEP_SERVO_PIN);
IrSensors irSensors(LEFT_IR_SENSOR_PIN, CENTER_IR_SENSOR_PIN, RIGHT_IR_SENSOR_PIN);
PingSensor pingSensor(PING_SENSOR_PIN);
Look looker(&sweepServo, &irSensors, &pingSensor);
Move mover(&looker);
Remote remote(&irrecv, &mover);

void setup() {
  Serial.begin(9600);
  blinker.begin();

  blinker.blinkNumber(8); // For Leonardo

  // Prep the movement system
  mover.begin();

  // Set the initial speed of the robot - 10% above minimum
  mover.setSpeed(MIN_SPEED + 10);

  // Center the servo
  sweepServo.write(90, 2000);

  // Seed the random number generator from the ambient light
  // level on the left sensor
  randomSeed(irSensors.getAmbientLevel(IrLeft));

  remote.begin();

  Serial.println("Ready");

}

void loop() {
  remote.service();
}
