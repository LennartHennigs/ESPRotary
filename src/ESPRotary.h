/////////////////////////////////////////////////////////////////
/*
  ESP8266/Arduino Library for reading rotary encoder values.
  Copyright 2017-2026 Lennart Hennigs
*/
/////////////////////////////////////////////////////////////////
#pragma once

#ifndef ESPRotary_h
#define ESPRotary_h

/////////////////////////////////////////////////////////////////

#include "Arduino.h"

/////////////////////////////////////////////////////////////////

enum class rotary_direction {
  undefined = 0,
  right = 1,
  left = 255
};

enum class rotary_event {
  left_rotation,
  right_rotation,
  speedup_started,
  speedup_ended,
  upper_bound_hit,
  lower_bound_hit,
  none
};

/////////////////////////////////////////////////////////////////

class ESPRotary {
 protected:
  int id;
  byte pin1, pin2;
  byte steps_per_click;
  int lower_bound = INT16_MIN;
  int upper_bound = INT16_MAX;
  byte state = 0;
  int increment = 1;
  int steps = 0;
  int last_steps = 0;
  rotary_event last_event = rotary_event::none;
  rotary_direction dir = rotary_direction::undefined;

  bool boundsTrigger = true;
  bool retrigger_event = true;
  bool enable_speedup = false;
  unsigned int speedup_increment = 5;
  unsigned int speedup_interval = 75;
  bool in_speedup = false;
  unsigned long last_turn = 0;

  using CallbackFunction = void (*)(ESPRotary&);
  
  CallbackFunction change_cb = NULL;
  CallbackFunction right_cb = NULL;
  CallbackFunction left_cb = NULL;
  CallbackFunction lower_cb = NULL;
  CallbackFunction upper_cb = NULL;

  CallbackFunction speedup_start_cb = NULL;
  CallbackFunction speedup_end_cb = NULL;

 public:
  ESPRotary();
  ESPRotary(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int initial_pos = 0, int increment = 1);

  void begin(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int initial_pos = 0, int increment = 1);

  int getPosition() const;
  void resetPosition(int p = 0, bool fireCallback = true);

  void triggerOnBounds(bool triggerEvents = true);
  rotary_direction getDirection() const;
  String directionToString(rotary_direction dir) const;

  void setIncrement(int increment);
  int getIncrement() const;

  void enableSpeedup(bool enable);
  void setSpeedupInterval(int interval);
  void setSpeedupIncrement(int increment);

  bool isSpeedupEnabled() const;
  int getSpeedupInterval() const;
  int getSpeedupIncrement() const;
  bool isInSpeedup() const;

  rotary_event getLastEvent() const;
  void retriggerEvent(bool retrigger);

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
  void setSpeedupStartedHandler(CallbackFunction f);
  void setSpeedupEndedHandler(CallbackFunction f);

  int getID() const;
  void setID(int newID);

  bool operator==(const ESPRotary& rhs) const;

  void loop();

 private:
  static int _nextID;

  void _callCallback(CallbackFunction callback);
  void _setEvent(rotary_event e);
  bool _wasRotated();
  bool _isWithinBounds(bool triggerAlerts = false);
  void _checkForSpeedup(unsigned long now);
  void _setID();
};

/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
