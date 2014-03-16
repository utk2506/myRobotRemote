// myRobotRemote sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
// See LICENSE.txt for details

#include <Arduino.h>
#include "RobotMotor.h"
#include "Move.h"
#include "pspc_support.h"

#ifdef MOVE_DEBUG
// Build up the string table for the states
#define STATE_NAME(i) STRING_FROM_TABLE(states,i)

// Build up a string table for the sensor names
const char left_state[] PROGMEM = "Left";
const char right_state[] PROGMEM = "Right";
const char forward_state[] PROGMEM = "Forward";
const char back_state[] PROGMEM = "Back";
const char rotate_state[] PROGMEM = "Rotate";
const char stop_state[] PROGMEM = "Stop";

PGM_P const states[] PROGMEM = {
  left_state,
  right_state,
  forward_state,
  back_state,
  rotate_state,
  stop_state
};
#endif

Move::Move() {
  state = MOV_STOP;
  speed = 0;
  speedIncrement = 10;
}

void Move::begin() {
  motorBegin(MOTOR_LEFT);
  motorBegin(MOTOR_RIGHT);
  stop();
}

void Move::left() {
  changeMoveState(MOV_LEFT);
  motorForward(MOTOR_LEFT,  0);
  motorForward(MOTOR_RIGHT, speed);
}

void Move::right() {
  changeMoveState(MOV_RIGHT);
  motorForward(MOTOR_LEFT,  speed);
  motorForward(MOTOR_RIGHT, 0);
}

void Move::forward() {
  changeMoveState(MOV_FORWARD);
  motorForward(MOTOR_LEFT,  speed);
  motorForward(MOTOR_RIGHT, speed);
}

void Move::backward() {
  changeMoveState(MOV_BACK);
  motorReverse(MOTOR_LEFT, speed);
  motorReverse(MOTOR_RIGHT, speed);  
}

void Move::rotate(int angle) {
  changeMoveState(MOV_ROTATE);

#ifdef MOVE_DEBUG
  Serial.print(P("Rotating ")); 
  Serial.println(angle);
#endif

  if (angle < 0) {
#ifdef MOVE_DEBUG
    Serial.println(P(" (left)")); 
#endif
    motorReverse(MOTOR_LEFT,  speed); 
    motorForward(MOTOR_RIGHT, speed);  
  } else if (angle > 0) {
#ifdef MOVE_DEBUG
    Serial.println(P(" (right)"));
#endif
    motorForward(MOTOR_LEFT,  speed);
    motorReverse(MOTOR_RIGHT, speed);
  }  

  int ms = rotationAngleToTime(angle);

  movingDelay(ms); 
  brake();   
}

void Move::stop() {
  changeMoveState(MOV_STOP);
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
}

void Move::brake() {
  changeMoveState(MOV_STOP);
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
}

void Move::setSpeed(int newSpeed) {
  motorSetSpeed(MOTOR_LEFT,  newSpeed) ;
  motorSetSpeed(MOTOR_RIGHT, newSpeed) ;
  speed = newSpeed; // save the value
}

void Move::slower(int decrement) {
#ifdef MOVE_DEBUG
  Serial.print(P(" Slower: ")); 
#endif

  if (speed >= speedIncrement + MIN_SPEED) {
    speed -= speedIncrement;
  } else {
    speed = MIN_SPEED;
  }

  setSpeed(speed); 
}

void Move::faster(int increment) {
#ifdef MOVE_DEBUG
  Serial.print(P(" Faster: "));
#endif
  speed += speedIncrement; 

  if (speed > 100) {
    speed = 100;
  }

  setSpeed(speed);      
}

long Move::rotationAngleToTime(int angle) {
  int fullRotationTime; // time to rotate 360 degrees at given speed

  if (speed < MIN_SPEED) {
    return 0; // ignore speeds slower then the first table entry
  }

  angle = abs(angle);

  if (speed >= 100) {

    fullRotationTime = rotationTime[NBR_SPEEDS - 1]; // the last entry is 100%

  } else { 

    // index into speed and time tables
    int index = (speed - MIN_SPEED) / SPEED_TABLE_INTERVAL ; 

    int t0 =  rotationTime[index];
    int t1 = rotationTime[index+1];    // time of the next higher speed 

    fullRotationTime = map(speed, speedTable[index], speedTable[index+1], t0, t1);

  }

  long result = map(angle, 0, 360, 0, fullRotationTime);

  return result; 
}

void Move::changeMoveState(MoveState newState) {
  if (newState != state) {
#ifdef MOVE_DEBUG
    Serial.print(P("Changing move state from ")); Serial.print(STATE_NAME(state));
    Serial.print(P(" to ")); Serial.println(STATE_NAME(newState));
#endif
    state = newState;
  } 
}

MoveState Move::getState() {
  return state;
}

void Move::timedMove(MoveState direction, int duration) {
#ifdef MOVE_DEBUG
  Serial.print(P("Timed move "));
#endif
  if (direction == MOV_FORWARD) {

#ifdef MOVE_DEBUG
    Serial.println(P("forward"));
#endif
    forward();    

  } else if (direction == MOV_BACK) {

#ifdef MOVE_DEBUG
    Serial.println(P("back"));
#endif
    backward();     

  } else {

#ifdef MOVE_DEBUG
    Serial.println(P("?"));
#endif

  }
    
  movingDelay(duration);

  stop();
}

void Move::movingDelay(long duration) {
#ifdef MOVE_DEBUG
  Serial.print(P("Duration: ")); Serial.println(duration);
#endif
  long startTime = millis();
  long elapsedTime = 0;
  
  while (elapsedTime < duration) {
    elapsedTime = millis() - startTime;
  }  
#ifdef MOVE_DEBUG
  Serial.print(P("Elapsed Time: ")); Serial.println(elapsedTime);
#endif
}

int Move::chanceRotationAngle() {
  return ((random(300) % 2) == 0) ? 90 : -90;
}

int Move::appropriateRotationAngle(int leftDistance, int rightDistance) {
  int angle = 90; // Turn right
  if (leftDistance == rightDistance) {
    // Roll the dice to see which way we should turn...
    angle = chanceRotationAngle();
  } else if (leftDistance > rightDistance) {
    angle = -90; // Turn left
  }
  return angle;
}

