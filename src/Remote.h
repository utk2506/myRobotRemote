// myRobotRemote Arduino sketch
//
// Copyright (c) 2012 Michael Margolis
// Copyright (c) 2013 Dave Sieh
//
// See LICENSE.txt for details.

#ifndef _Remote_h_
#define _Remote_h_

class IRrecv;
class Move;

class Remote {
 public:
  Remote(IRrecv *irReceiverSensor, Move *move);

  void begin();

  void service();

 private:
  void convertIrToCommand(long value);
  void changeCmdState(int newState);
  void processCommand(int cmd);
  void processCommand(int cmd, int val);

  IRrecv *irrecv;
  Move *mover;
  int commandState;
};

#endif
