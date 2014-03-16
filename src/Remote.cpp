// myRobotRemote Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
//
// See LICENSE.txt for details.

#include <Arduino.h>
#include "Remote.h"
#include <IRremote.h>
#include <pspc_support.h>
#include "Move.h"

// IR remote keycodes:replace this with codes for your remote
// These are the codes from the Gray RCA remote
//const long IR_MOVE_FORWARD = 50190375;
//const long IR_MOVE_BACK    = 50198535;
//const long IR_MOVE_LEFT    = 50165895;
//const long IR_MOVE_RIGHT   = 50157735;
//const long IR_PIVOT_CW     = 50149575;
//const long IR_PIVOT_CCW    = 50194455;
//const long IR_HALT         = 50137335;

// These are the codes from the Polaroid remote
const long IR_MOVE_FORWARD = 284123295;
const long IR_MOVE_BACK    = 284155935;
const long IR_MOVE_LEFT    = 284131965;
const long IR_MOVE_RIGHT   = 284148285;
const long IR_PIVOT_CW     = 284106975;
const long IR_PIVOT_CCW    = 284113095;
const long IR_HALT         = 284115645;

// Command constants 

const char MOVE_FORWARD = 'f'; // move forward
const char MOVE_BACK    = 'b'; // move backward
const char MOVE_LEFT    = 'l'; // move left
const char MOVE_RIGHT   = 'r'; // move right 
const char PIVOT_CCW    = 'C'; // rotate 90 degrees CCW
const char PIVOT_CW     = 'c'; // rotate 90 degrees CW
const char PIVOT        = 'p'; // rotation angle (minus rotates CCW)
const char HALT         = 'h'; // stop

// not used in this example
const char MOVE_SPEED        = 's'; 
const char MOVE_SLOWER       = 'v'; // reduce speed 
const char MOVE_FASTER       = '^'; // increase speed 

#ifdef REMOTE_DEBUG
#define STATE_NAME(i) STRING_FROM_TABLE(moveStates,i)

const char moveLeft[] PROGMEM = "Left";
const char moveRight[] PROGMEM = "Right";
const char moveForward[] PROGMEM = "Forward";
const char moveBack[] PROGMEM = "Back";
const char moveRotate[] PROGMEM = "Rotate";
const char moveStop[] PROGMEM = "Stop";

PGM_P const moveStates[] PROGMEM = {
  moveLeft,
  moveRight,
  moveForward,
  moveBack,
  moveRotate,
  moveStop
};

#endif


Remote::Remote(IRrecv *irReceiver, Move *move) {
  irrecv = irReceiver;
  mover = move;
  commandState = MOV_STOP;
}

void Remote::begin() {
  irrecv->enableIRIn(); // Start the receiver
}

void Remote::service() {
  decode_results results;
  
  if (irrecv->decode(&results)) {
    if (results.decode_type != UNKNOWN) {
#ifdef REMOTE_DEBUG
      Serial.println(P("Raw IR value: "));
      Serial.println(results.value); 
#endif
      convertIrToCommand(results.value);
    }
    irrecv->resume(); // Receive the next value
  }  
}

void Remote::convertIrToCommand(long value) {
    switch(value) {
    case  IR_MOVE_LEFT    :  processCommand(MOVE_LEFT);    break;
    case  IR_MOVE_RIGHT   :  processCommand(MOVE_RIGHT);   break;
    case  IR_MOVE_FORWARD :  processCommand(MOVE_FORWARD); break;
    case  IR_MOVE_BACK    :  processCommand(MOVE_BACK);    break;
    case  IR_PIVOT_CCW    :  processCommand(PIVOT_CCW);    break;
    case  IR_PIVOT_CW     :  processCommand(PIVOT_CW);     break;
    case  IR_HALT         :  processCommand(HALT);         break;
//    case  IR_SLOWER       :  processCommand(SLOWER);       break;
//    case  IR_FASTER       :  processCommand(FASTER);       break;
    }  
}

void Remote::changeCmdState(int newState) {
  if(newState != commandState) {
#ifdef REMOTE_DEBUG
    Serial.print(P("Changing Cmd state from ")); Serial.print(STATE_NAME(commandState));
    Serial.print(P(" to ")); Serial.println(STATE_NAME(newState));
#endif
    commandState = newState;
  } 
}

void Remote::processCommand(int cmd) {
  int val = 0;
  if( cmd == MOVE_SPEED) {
    val =  Serial.parseInt();
  } else if( cmd == PIVOT) {
    val =  Serial.parseInt();
  }
  processCommand(cmd, val);
}

void Remote::processCommand(int cmd, int val) {
  byte speed;

  //Serial.write(cmd); // uncomment to echo
  switch(cmd) { 
  case MOVE_LEFT    : changeCmdState(MOV_LEFT);    mover->left();      break;
  case MOVE_RIGHT   : changeCmdState(MOV_RIGHT);   mover->right();     break;
  case MOVE_FORWARD : changeCmdState(MOV_FORWARD); mover->forward();   break;
  case MOVE_BACK    : changeCmdState(MOV_BACK);    mover->backward();  break;
  case PIVOT_CCW    : changeCmdState(MOV_ROTATE);  mover->rotate(-90); break;
  case PIVOT_CW     : changeCmdState(MOV_ROTATE);  mover->rotate(90);  break;
  case PIVOT        : changeCmdState(MOV_ROTATE);  mover->rotate(val); break; 
  case HALT         : changeCmdState(MOV_STOP);    mover->stop();      break;
  case MOVE_SPEED   : speed = val;    mover->setSpeed(speed);         break;
//   case SLOWER       : moveSlower(speedIncrement);                  break;
//   case FASTER       : moveFaster(speedIncrement);                  break;
  case '\r' : case '\n': break; // ignore cr and lf
  default :  
#ifdef REMOTE_DEBUG
                         Serial.print(P("[")); Serial.write(cmd); Serial.println(P("] Ignored"));  
#endif
                         break;
  }    
}

