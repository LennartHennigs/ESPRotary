/////////////////////////////////////////////////////////////////
/*
  ESP8266/Arduino Library for reading rotary encoder values.
  Copyright 2017-2022 Lennart Hennigs.
*/
/////////////////////////////////////////////////////////////////

#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////
// initialize static counter

int ESPRotary::_nextID = 0;

/////////////////////////////////////////////////////////////////

void ESPRotary::_setID() {
  id = _nextID;
  _nextID++;
}

/////////////////////////////////////////////////////////////////

ESPRotary::ESPRotary() {
  _setID();
}

/////////////////////////////////////////////////////////////////

ESPRotary::ESPRotary(byte pin1, byte pin2, byte steps_per_click /* = 1 */,  int lower_bound /* = INT16_MIN */, int upper_bound /* = INT16_MAX */, int inital_pos /* = 0 */, int increment /* = 1 */) {
  ESPRotary();
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
  steps = inital_pos * steps_per_click;
  last_event = none;
  dir = undefined;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setUpperBound(int upper) {
  upper_bound = (lower_bound < upper) ? upper: lower_bound;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setLowerBound(int lower) {
  lower_bound = (lower < upper_bound) ? lower : upper_bound;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getUpperBound() const {
  return upper_bound;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getLowerBound() const {
  return lower_bound;
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

void ESPRotary::setSpeedupStartedHandler(CallbackFunction f) {
  speedup_start_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setSpeedupEndedHandler(CallbackFunction f) {
  speedup_end_cb = f;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::resetPosition(int p /* = 0 */, bool fireCallback /* = true */) {
  // change position?
  if (p == getPosition()) return;
  // yes...
  steps = p * steps_per_click;
  _isWithinBounds();
  if (fireCallback && change_cb != NULL) change_cb (*this);
  last_event = none;
  dir = undefined;
  in_speedup = false;
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

rotary_direction ESPRotary::getDirection() const {
  return dir;
}

/////////////////////////////////////////////////////////////////

String ESPRotary::directionToString(rotary_direction dir) const {
  return (dir == right) ? "right" : "left";
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getPosition() const {
  return steps / steps_per_click;
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
  long now = millis();
  // did it change (enough)?
  if (!_wasRotated()) return;
  dir = (steps > last_steps) ? right : left;
  // shall I speedup things
  if (enable_speedup) _checkForSpeedup(now);
  // are we out of bounds?
  if (_isWithinBounds(true)) {
    // trigger rotation event  
    _setEvent((dir == right) ? right_rotation : left_rotation);
    last_turn = now;
    }
  last_steps = steps;
}

/////////////////////////////////////////////////////////////////

bool ESPRotary::_wasRotated() {
  int s = state & 3;
  if (digitalRead(pin1)) s |= 4;
  if (digitalRead(pin2)) s |= 8;
  switch (s) {
    case 0: case 5: case 10: case 15:
      break;
    case 1: case 7: case 8: case 14:
        steps += increment; break;
    case 2: case 4: case 11: case 13:
      steps -= increment; break;
    case 3: case 12:
      steps += 2 * increment; break;
    default:
      steps -= 2 * increment; break;
  }
  state = (s >> 2);
  return (abs(steps - last_steps) >= steps_per_click * increment);
}

/////////////////////////////////////////////////////////////////

void ESPRotary::_checkForSpeedup(long now) {
  if (now - last_turn > speedup_interval) {
    if (in_speedup) _setEvent(speedup_ended);
    return;
  }
  steps += (dir == right ? 1 : -1) * (speedup_increment - increment) * steps_per_click;
  int pos = getPosition();
  // only trigger speedup when you are not "on a wall"
  if (pos > lower_bound && pos < upper_bound) {
    if (!in_speedup) _setEvent(speedup_started);
  }
}

/////////////////////////////////////////////////////////////////

bool ESPRotary::_isWithinBounds(bool alert /* = false */) {
  int pos = getPosition();
  if (pos > lower_bound && pos < upper_bound) return true;

  if (pos >= upper_bound) {
    if (in_speedup) _setEvent(speedup_ended);
    if (alert) _setEvent(upper_bound_hit);
    steps = upper_bound * steps_per_click;

  } else if (pos <= lower_bound) {
    if (in_speedup) _setEvent(speedup_ended);
    if (alert)  _setEvent(lower_bound_hit);
    steps = lower_bound * steps_per_click;
  }
  return false;
}
/////////////////////////////////////////////////////////////////

void ESPRotary::_setEvent(rotary_event e) {
  switch (e) {
    case left_rotation:
      if (left_cb != NULL) left_cb (*this);
      if (change_cb != NULL) change_cb (*this);
    break;

    case right_rotation:
      if (right_cb != NULL) right_cb (*this);
      if (change_cb != NULL) change_cb (*this);
    break;

    case speedup_started:
      if (speedup_start_cb != NULL) speedup_start_cb (*this);
      in_speedup = true;
    break;

    case speedup_ended:
      if (speedup_end_cb != NULL) speedup_end_cb (*this);
      in_speedup = false;
    break;

    case upper_bound_hit:
      if (last_event == upper_bound_hit && !retrigger_event) return;
      if (upper_cb != NULL) upper_cb (*this);
    break;

    case lower_bound_hit:
      if (last_event == lower_bound_hit && !retrigger_event) return;
      if (lower_cb != NULL) lower_cb (*this);
    break;

    case none:
    break;
  }
  last_event = e;
}
/////////////////////////////////////////////////////////////////

  void ESPRotary::setSpeedupInterval(int time) {
    speedup_interval = time;
  }

/////////////////////////////////////////////////////////////////

  int ESPRotary::getSpeedupInterval() const {
    return speedup_interval;
  }

/////////////////////////////////////////////////////////////////

  void ESPRotary::setSpeedupIncrement(int inc) {
    speedup_increment = inc;
  }

/////////////////////////////////////////////////////////////////

  int ESPRotary::getSpeedupIncrement() const {
    return speedup_increment;
  }

/////////////////////////////////////////////////////////////////

  void ESPRotary::enableSpeedup(bool enable) {
    enable_speedup = enable;
  }

/////////////////////////////////////////////////////////////////

  bool ESPRotary::isSpeedupEnabled() const {
    return enable_speedup;
  }

/////////////////////////////////////////////////////////////////

  rotary_event ESPRotary::getLastEvent() const {
    return last_event;
  }

/////////////////////////////////////////////////////////////////

void ESPRotary::retriggerEvent(bool retrigger) {
  retrigger_event = retrigger;
}

/////////////////////////////////////////////////////////////////

bool ESPRotary::isInSpeedup() const {
  return in_speedup;
}

/////////////////////////////////////////////////////////////////