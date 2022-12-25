/////////////////////////////////////////////////////////////////
/*
  ESP8266/Arduino Library for reading rotary encoder values.
  Copyright 2017-2021 Lennart Hennigs.
*/
/////////////////////////////////////////////////////////////////

#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////
// initialize static counter

int ESPRotary::_nextID = 0;

/////////////////////////////////////////////////////////////////

ESPRotary::ESPRotary() {
  id = _nextID++;
}

/////////////////////////////////////////////////////////////////

ESPRotary::ESPRotary(byte pin1, byte pin2, byte steps_per_click /* = 1 */,  int lower_bound /* = INT16_MIN */, int upper_bound /* = INT16_MAX */, int inital_pos /* = 0 */, int increment /* = 1 */) {
  id = _nextID++;
  begin(pin1, pin2, steps_per_click, lower_bound, upper_bound, inital_pos, increment);
}

/////////////////////////////////////////////////////////////////

void ESPRotary::begin(byte pin1, byte pin2, byte steps_per_click /* = 1 */,  int lower_bound /* = INT16_MIN */, int upper_bound /* = INT16_MAX */, int inital_pos /* = 0 */, int increment /* = 1 */) {
  this->pin1 = pin1;
  this->pin2 = pin2;
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);

  setUpperBound(upper_bound);
  setLowerBound(lower_bound);
  setIncrement(increment);
  setStepsPerClick(steps_per_click);
  
  loop();
  resetPosition(inital_pos, false);
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setUpperBound(int upper_bound) {
  this->upper_bound = (lower_bound < upper_bound) ? upper_bound: lower_bound;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setLowerBound(int lower_bound) {
  this->lower_bound = (lower_bound < upper_bound) ? lower_bound : upper_bound;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getUpperBound() const {
  return this->upper_bound;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getLowerBound() const {
  return this->lower_bound;
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

void ESPRotary::setUpperOverflowHandler(CallbackFunction f) {
  upper_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setLowerOverflowHandler(CallbackFunction f) {
  lower_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::resetPosition(int p /* = 0 */, bool fireCallback /* = true */) {
  if (p > upper_bound) {
    last_position = upper_bound * steps_per_click;
  } else {
    last_position = (lower_bound > p) ? lower_bound * steps_per_click : p * steps_per_click;
  }

  if (position != last_position) {
    position = last_position;
    if (fireCallback && change_cb != NULL) change_cb (*this);
  }
  dir = undefined;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setIncrement(int inc) {
  increment = inc;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getIncrement() const {
  return increment;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setStepsPerClick(int steps) {
  steps_per_click = (steps < 1) ? 1 : steps;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getStepsPerClick() const {
  return steps_per_click;
}

/////////////////////////////////////////////////////////////////

direction ESPRotary::getDirection() const {
  return dir;
}

/////////////////////////////////////////////////////////////////

String ESPRotary::directionToString(direction dir) const {
  if (dir == right) {
    return "left";
  } else {
    return "right";
  }
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getPosition() const {
  return position / steps_per_click;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getID() const {
  return id;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setID(int newID) {
  id = newID;
}

/////////////////////////////////////////////////////////////////

bool ESPRotary::operator == (ESPRotary &rhs) {
  return (this == &rhs);
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
        position += increment; break;
    case 2: case 4: case 11: case 13:
      position -= increment; break;
    case 3: case 12:
      position += 2 * increment; break;
    default:
      position -= 2 * increment; break;
  }
  state = (s >> 2);
  // did it change?
  if (position == last_position) return;
  // did it change enough?
  if (abs(position - last_position) < steps_per_click * increment) return;

  int current_position = getPosition();
  
  // are we within bounds?
  if (current_position < lower_bound) {
    if (lower_cb != NULL) lower_cb (*this);
    return;
  }
  if (current_position > upper_bound) {
    if (upper_cb != NULL) upper_cb (*this);
    return;
  }

  if (position > last_position) {
    dir = right;
    if (right_cb != NULL) right_cb (*this);
  } else {
    dir = left;
    if (left_cb != NULL) left_cb (*this);
  }
  last_position = position;
  if (change_cb != NULL) change_cb (*this);
}

/////////////////////////////////////////////////////////////////
