// -*- c -*-
// myRobotRemote Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#include <RobotMotor.h>
#include <LEDBlinker.h>
#include <IRremote.h>
#include "Move.h"
#include <pspc_support.h>
#include "Remote.h"

#define LED_PIN 13

const byte irReceivePin = A3;

IRrecv irrecv(irReceivePin);
LEDBlinker blinker(LED_PIN);
Move mover;
Remote remote(&irrecv, &mover);

void setup() {
  Serial.begin(9600);
  blinker.begin();

  blinker.blinkNumber(8); // For Leonardo

  // Prep the movement system
  mover.begin();

  // Set the initial speed of the robot - 10% above minimum
  mover.setSpeed(MIN_SPEED + 10);

  remote.begin();

  Serial.println(P("Ready"));

}

void loop() {
  remote.service();
}
