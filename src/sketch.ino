// -*- c -*-
// myRobotRemote Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#include <RobotMotor.h> //include RobotMotor.h library zip file.
#include <LEDBlinker.h> //include LEDBlinker.h library zip file.
#include <IRremote.h> //include IRremote.h library zip file.
#include "Move.h"     //include Move.h library zip file.
#include <pspc_support.h> //include pspc_support.h library zip file.
#include "Remote.h"     //include Remote.h library zip file.

#define LED_PIN 13      //connect positive pin of LED on 13 PIN of arduino

const byte irReceivePin = A3;  //connect receiver pin of IR receiver on A3 of arduino

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
