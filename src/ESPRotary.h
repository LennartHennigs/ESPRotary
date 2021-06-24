/////////////////////////////////////////////////////////////////
/*
  ESP8266/Arduino Library for reading rotary encoder values.
  Copyright 2017-2021 Lennart Hennigs
*/
/////////////////////////////////////////////////////////////////
#pragma once

#ifndef ESPRotary_h
#define ESPRotary_h

/////////////////////////////////////////////////////////////////

#include "Arduino.h"

/////////////////////////////////////////////////////////////////

#define RE_RIGHT            1
#define RE_LEFT             255

/////////////////////////////////////////////////////////////////

class ESPRotary {
 protected:
   byte pin1, pin2;
   byte steps_per_click;
   int lower_bound, upper_bound;
   int position, last_position;
   byte direction;
   byte state;
   int increment;

    typedef void (*CallbackFunction) (ESPRotary&);
    CallbackFunction change_cb = NULL;
    CallbackFunction right_cb = NULL;
    CallbackFunction left_cb = NULL;
    CallbackFunction lower_cb = NULL;
    CallbackFunction upper_cb = NULL;

 public:
   ESPRotary();
   ESPRotary(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int inital_pos = 0, int increment = 1);

   void begin(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int inital_pos = 0, int increment = 1);

   int getPosition() const;
   void resetPosition(int p = 0, bool fireCallback = true);

   byte getDirection() const;
   String directionToString(byte direction) const;

   void setIncrement(int inc);
   int getIncrement() const;

   void setUpperBound(int upper_bound);
   void setLowerBound(int lower_bound);
   int getUpperBound() const;
   int getLowerBound() const;

   void setStepsPerClick(int steps);
   int getStepsPerClick() const;
   
   void setChangedHandler(CallbackFunction f);
   void setRightRotationHandler(CallbackFunction f);
   void setLeftRotationHandler(CallbackFunction f);
   void setUpperOverflowHandler(CallbackFunction f);
   void setLowerOverflowHandler(CallbackFunction f);

   void loop();
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
