// myRobotRemote sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013,2014 Dave Sieh
// See LICENSE.txt for details

#ifndef _Move_h_
#define _Move_h_

enum MoveState {
  MOV_LEFT, 
  MOV_RIGHT, 
  MOV_FORWARD, 
  MOV_BACK, 
  MOV_ROTATE, 
  MOV_STOP
};

class Move {
 public:

  Move();

  /**
     Initialize the motor system.
  */
  void begin();

  /**
     Turn left.
   */
  void left();

  /**
     Turn right.
  */
  void right();
  
  /**
     Move forward.
  */
  void forward();
  
  /**
     Move backward.
  */
  void backward();
  
  /**
     Rotate a particular angle
  */
  void rotate(int angle);

  /**
     Stop the motors.
  */
  void stop();

  /**
     Stop the motors, using brakes if available.
  */
  void brake();
  
  /**
     Sets the current speed.
   */
  void setSpeed(int newSpeed);

  /**
     Move slower.
   */
  void slower(int decrement);

  /**
     Move faster.
  */
  void faster(int decrement);

  /**
     Returns the current motion state.
  */
  MoveState getState();

  /**
     Perform a timed move in the specified direction for the
     specified duration.
   */
  void timedMove(MoveState direction, int duration);

  /**
     Delay while watching for collisions.
   */
  void movingDelay(long duration);

 private:
  /**
     Returns the number of milliseconds it takes to rotate
     to a particular angle at the current speed.
   */
  long rotationAngleToTime(int angle);

  /**
     Sets the new motion state.
   */
  void changeMoveState(MoveState newState);

  int chanceRotationAngle();
  int appropriateRotationAngle(int leftDistance, int rightDistance);

  MoveState state;
  int speed;
  int speedIncrement;
};

#endif
