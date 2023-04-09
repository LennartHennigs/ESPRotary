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

ESPRotary::ESPRotary(byte pin1, byte pin2, byte steps_per_click /* = 1 */, int lower_bound /* = INT16_MIN */, int upper_bound /* = INT16_MAX */, int inital_pos /* = 0 */, int increment /* = 1 */) {
  ESPRotary();
  begin(pin1, pin2, steps_per_click, lower_bound, upper_bound, inital_pos, increment);
}

/////////////////////////////////////////////////////////////////

void ESPRotary::begin(byte pin1, byte pin2, byte steps_per_click /* = 1 */, int lower_bound /* = INT16_MIN */, int upper_bound /* = INT16_MAX */, int inital_pos /* = 0 */, int increment /* = 1 */) {
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
  last_event = rotary_event::none;
  dir = rotary_direction::undefined;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setUpperBound(int upper) {
  upper_bound = (lower_bound < upper) ? upper : lower_bound;
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
  if (fireCallback) _callCallback(change_cb);
  last_event = rotary_event::none;
  dir = rotary_direction::undefined;
  in_speedup = false;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setIncrement(int increment) {
  increment = increment;
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
  return (dir == rotary_direction::right) ? "right" : "left";
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

bool ESPRotary::operator==(ESPRotary &rhs) {
  return (this == &rhs);
}

/////////////////////////////////////////////////////////////////

void ESPRotary::loop() {
  unsigned long now = millis();
  // did it change (enough)?
  if (!_wasRotated()) return;
  dir = (steps > last_steps) ? rotary_direction::right : rotary_direction::left;
  // shall I speedup things
  if (enable_speedup) _checkForSpeedup(now);
  // are we out of bounds?
  if (_isWithinBounds(true)) {
    // trigger rotation event
    _setEvent((dir == rotary_direction::right) ? rotary_event::right_rotation : rotary_event::left_rotation);
    last_turn = now;
  }
  last_steps = steps;
}

/////////////////////////////////////////////////////////////////

bool ESPRotary::_wasRotated() {
  static const int8_t factors[] = {0, 1, -1, 2, -1, 0, -2, 1, 1, -2, 0, -1, 2, -1, 1, 0};
  int encoderState = (state & 3) | digitalRead(pin1) << 2 | digitalRead(pin2) << 3 ;
  steps += factors[encoderState] * increment;
  state = (encoderState >> 2);
  int stepDifference = abs(steps - last_steps);
  return stepDifference >= (steps_per_click * increment);
}

/////////////////////////////////////////////////////////////////

void ESPRotary::_checkForSpeedup(unsigned long now) {
  if (now - last_turn > speedup_interval) {
    if (in_speedup) _setEvent(rotary_event::speedup_ended);
    return;
  }
  steps += ((dir == rotary_direction::right ? 1 : -1) * (speedup_increment - increment) * steps_per_click);
  int pos = getPosition();
  // only trigger speedup when you are not "on a wall"
  if (pos > lower_bound && pos < upper_bound) {
    if (!in_speedup) _setEvent(rotary_event::speedup_started);
  }
}

/////////////////////////////////////////////////////////////////

void ESPRotary::triggerOnBounds(bool triggerEvents /* = false */) {
  boundsTrigger = triggerEvents;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::_callCallback(CallbackFunction callback) {
  if (callback != NULL) callback(*this);
}

/////////////////////////////////////////////////////////////////

bool ESPRotary::_isWithinBounds(bool triggerAlerts /* = false */) {
  int pos = getPosition();
  if (pos > lower_bound && pos < upper_bound) return true;

  if (pos >= upper_bound) {
    steps = upper_bound * steps_per_click;
    if (in_speedup) _setEvent(rotary_event::speedup_ended);
    if (triggerAlerts) _setEvent(rotary_event::upper_bound_hit);

  } else if (pos <= lower_bound) {
    steps = lower_bound * steps_per_click;
    if (in_speedup) _setEvent(rotary_event::speedup_ended);
    if (triggerAlerts) _setEvent(rotary_event::lower_bound_hit);
  }

  return false;
}
/////////////////////////////////////////////////////////////////

void ESPRotary::_setEvent(rotary_event event) {
  switch (event) {
    case rotary_event::left_rotation:
      _callCallback(left_cb);
      _callCallback(change_cb);
      break;

    case rotary_event::right_rotation:
      _callCallback(right_cb);
      _callCallback(change_cb);
      break;

    case rotary_event::speedup_started:
      _callCallback(speedup_start_cb);
      in_speedup = true;
      break;

    case rotary_event::speedup_ended:
      _callCallback(speedup_end_cb);
      in_speedup = false;
      break;

    case rotary_event::upper_bound_hit:
      if (last_event == rotary_event::upper_bound_hit && !retrigger_event) return;
      if (boundsTrigger) {
        _callCallback(right_cb);
        _callCallback(change_cb);
      }
      _callCallback(upper_cb);
      break;

    case rotary_event::lower_bound_hit:
      if (last_event == rotary_event::lower_bound_hit && !retrigger_event) return;
      if (boundsTrigger) {
        _callCallback(left_cb);
        _callCallback(change_cb);
      }
      _callCallback(lower_cb);
      break;

    case rotary_event::none:
      break;
  }
  last_event = event;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setSpeedupInterval(int interval) {
  speedup_interval = interval;
}

/////////////////////////////////////////////////////////////////

int ESPRotary::getSpeedupInterval() const {
  return speedup_interval;
}

/////////////////////////////////////////////////////////////////

void ESPRotary::setSpeedupIncrement(int increment) {
  speedup_increment = increment;
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