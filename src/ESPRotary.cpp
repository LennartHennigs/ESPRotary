/////////////////////////////////////////////////////////////////
/*
  ESP8266/Arduino Library for reading rotary encoder values.
  Copyright 2017-2020 Lennart Hennigs.
*/
/////////////////////////////////////////////////////////////////

#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////

ESPRotary::ESPRotary(int pin1, int pin2, int steps_per_click /* = 1 */,  int inital_pos /* = 0 */, int lower_bound /* = INT16_MIN */, int upper_bound /* = INT16_MAX */) {
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->lower_bound = (lower_bound < upper_bound) ? lower_bound : upper_bound;
  this->upper_bound = (lower_bound < upper_bound) ? upper_bound: lower_bound;
  setStepsPerClick(steps_per_click);

  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);

  loop();
  resetPosition(inital_pos);
  last_read_ms = 0;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setChangedHandler(CallbackFunction f) {
  change_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setRightRotationHandler(CallbackFunction f) {
  right_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setLeftRotationHandler(CallbackFunction f) {
  left_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::resetPosition(int p /* = 0 */) {
  if (p > upper_bound) {
    last_position = upper_bound * steps_per_click;
  } else {
    last_position = (lower_bound > p) ? lower_bound * steps_per_click : p * steps_per_click;
  }

if (position != last_position) {
  position = last_position;
  if (change_cb != NULL) change_cb (*this);
}
  direction = 0;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setStepsPerClick(int steps) {
  steps_per_click = (steps < 1) ? 1 : steps;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getStepsPerClick() {
  return steps_per_click;
}

/////////////////////////////////////////////////////////////////

byte ESPRotary::getDirection() {
  return direction;
}

/////////////////////////////////////////////////////////////////

String ESPRotary::directionToString(byte direction) {
  if (direction == RE_LEFT) {
    return "LEFT";
  } else {
    return "RIGHT";
  }
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getPosition() {
  return position / steps_per_click;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::loop() {
  int s = state & 3;
  if (digitalRead(pin1)) s |= 4;
  if (digitalRead(pin2)) s |= 8;

  switch (s) {
    case 0: case 5: case 10: case 15:
      break;
    case 1: case 7: case 8: case 14:
        position++; break;
    case 2: case 4: case 11: case 13:
      position--; break;
    case 3: case 12:
      position += 2; break;
    default:
      position -= 2; break;
  }
  state = (s >> 2);

  if (getPosition() >= lower_bound && getPosition() <= upper_bound) {
    if (position != last_position) {
      if (abs(position - last_position) >= steps_per_click) {
        if (position > last_position) {
          direction = RE_RIGHT;
          if (right_cb != NULL) right_cb (*this);
        } else {
          direction = RE_LEFT;
          if (left_cb != NULL) left_cb (*this);
        }
        last_position = position;
        if (change_cb != NULL) change_cb (*this);
      }
    }
  } else position = last_position;
}

/////////////////////////////////////////////////////////////////
