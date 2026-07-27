/////////////////////////////////////////////////////////////////
/*
  Rotation / event-engine tests for ESPRotary.

  Uses the setPinReadFunction() seam (see test_helpers.h) to drive a
  simulated quadrature signal, so loop(), the decode table, direction
  detection, bound overflow events and speedup can be exercised on the
  host. Also contains regression tests for reported bugs.
*/
/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <AUnitVerbose.h>
#include "../shared/test_helpers.h"

using namespace aunit;

/////////////////////////////////////////////////////////////////

#define SERIAL_SPEED 115200

/////////////////////////////////////////////////////////////////
// Callback counters

int changeCount = 0;
int leftCount = 0;
int rightCount = 0;
int upperCount = 0;
int lowerCount = 0;

void onChange(ESPRotary&) { changeCount++; }
void onLeft(ESPRotary&) { leftCount++; }
void onRight(ESPRotary&) { rightCount++; }
void onUpper(ESPRotary&) { upperCount++; }
void onLower(ESPRotary&) { lowerCount++; }

void resetCounters() {
  changeCount = leftCount = rightCount = upperCount = lowerCount = 0;
}

/////////////////////////////////////////////////////////////////
// Characterization: basic rotation
/////////////////////////////////////////////////////////////////

test(rotation, turn_right_increments_position) {
  ESPRotary r = createTestRotary();
  turnRight(r, 1);
  assertEqual(r.getPosition(), 1);
  assertTrue(r.getDirection() == rotary_direction::right);
}

test(rotation, turn_left_decrements_position) {
  ESPRotary r = createTestRotary();
  turnLeft(r, 1);
  assertEqual(r.getPosition(), -1);
  assertTrue(r.getDirection() == rotary_direction::left);
}

test(rotation, multiple_detents) {
  ESPRotary r = createTestRotary();
  turnRight(r, 5);
  assertEqual(r.getPosition(), 5);
  turnLeft(r, 2);
  assertEqual(r.getPosition(), 3);
}

test(rotation, change_callback_fires_once_per_detent) {
  ESPRotary r = createTestRotary();
  resetCounters();
  r.setChangedHandler(onChange);
  r.setRightRotationHandler(onRight);
  turnRight(r, 3);
  assertEqual(changeCount, 3);
  assertEqual(rightCount, 3);
}

/////////////////////////////////////////////////////////////////
// Characterization: bounds & overflow callbacks
/////////////////////////////////////////////////////////////////

test(rotation, clamps_at_upper_bound_and_fires_overflow) {
  ESPRotary r;
  simPinState = 0;
  r.setPinReadFunction(fakePinRead);
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, 0, 2);
  resetCounters();
  r.setUpperOverflowHandler(onUpper);
  turnRight(r, 5);
  assertEqual(r.getPosition(), 2);
  assertMoreOrEqual(upperCount, 1);
}

/////////////////////////////////////////////////////////////////
// Probe: does speedup reverse direction? (review finding #1)
// Net position change per detent under speedup should stay positive
// for a rightward turn even when increment > speedup_increment.
/////////////////////////////////////////////////////////////////

test(rotation, speedup_does_not_reverse_direction) {
  ESPRotary r = createTestRotary();
  r.enableSpeedup(true);
  r.setSpeedupInterval(100000);  // force the speedup path
  r.setSpeedupIncrement(5);
  r.setIncrement(10);
  turnRight(r, 3);
  assertMore(r.getPosition(), 0);
  assertTrue(r.getDirection() == rotary_direction::right);
}

/////////////////////////////////////////////////////////////////
// Regression: no phantom event after resetPosition / initial_pos
// (review finding #2 — last_steps left stale)
/////////////////////////////////////////////////////////////////

test(rotation, no_phantom_event_after_reset) {
  ESPRotary r = createTestRotary();
  r.setChangedHandler(onChange);
  r.setRightRotationHandler(onRight);
  resetCounters();
  r.resetPosition(50, false);  // reposition without firing a callback
  r.loop();                    // no rotation happened
  assertEqual(changeCount, 0);
  assertEqual(rightCount, 0);
  assertEqual(r.getPosition(), 50);
}

test(rotation, no_phantom_event_after_begin_with_initial_pos) {
  simPinState = 0;
  ESPRotary r;
  r.setPinReadFunction(fakePinRead);
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, INT16_MIN, INT16_MAX, 50);
  r.setChangedHandler(onChange);
  resetCounters();
  r.loop();  // first loop after begin, no rotation
  assertEqual(changeCount, 0);
  assertEqual(r.getPosition(), 50);
}

/////////////////////////////////////////////////////////////////

void setup() {
  delay(100);
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {}
  Serial.println(F("\n\nESPRotary Rotation Tests"));
  Serial.println(F("Using EpoxyDuino + AUnit"));
  TestRunner::setVerbosity(Verbosity::kDefault);
  TestRunner::setTimeout(90);
}

/////////////////////////////////////////////////////////////////

void loop() {
  aunit::TestRunner::run();
}

/////////////////////////////////////////////////////////////////
