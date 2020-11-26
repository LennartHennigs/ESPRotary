/////////////////////////////////////////////////////////////////
/*
  ESP8266/Arduino Library for reading rotary encoder values.
  Copyright 2017 Lennart Hennigs
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
    int pin1, pin2, position, last_position, steps_per_click, lower_bound, upper_bound;
    unsigned long last_read_ms;
    byte direction;
    byte state;

    typedef void (*CallbackFunction) (ESPRotary&);
    CallbackFunction change_cb = NULL;
    CallbackFunction right_cb = NULL;
    CallbackFunction left_cb = NULL;

 public:
    ESPRotary(int pin1, int pin2, int steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX);

    int getPosition();
    void resetPosition(int p = 0);
    byte getDirection();

    void setStepsPerClick(int steps);
    int getStepsPerClick();

    String directionToString(byte direction);
    void setChangedHandler(CallbackFunction f);
    void setRightRotationHandler(CallbackFunction f);
    void setLeftRotationHandler(CallbackFunction f);

    void loop();
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
