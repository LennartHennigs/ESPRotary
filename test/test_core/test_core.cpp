/////////////////////////////////////////////////////////////////
/*
  Core behavior tests for the ESPRotary library.
  Locks in the existing public API / state behavior (position,
  bounds, increment, direction, speedup config, IDs) as a safety
  net. Native testing via EpoxyDuino + AUnit, no hardware required.
*/
/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <AUnitVerbose.h>
#include "../shared/test_helpers.h"

using namespace aunit;

/////////////////////////////////////////////////////////////////

#define SERIAL_SPEED 115200

/////////////////////////////////////////////////////////////////
// IDs & equality
/////////////////////////////////////////////////////////////////

test(core, ids_are_unique) {
  ESPRotary a;
  ESPRotary b;
  assertNotEqual(a.getID(), b.getID());
}

test(core, set_id_overrides) {
  ESPRotary r;
  r.setID(4242);
  assertEqual(r.getID(), 4242);
}

test(core, equal_operator) {
  ESPRotary a = createTestRotary();
  ESPRotary b = createTestRotary();
  assertTrue(a == a);
  assertFalse(a == b);
}

/////////////////////////////////////////////////////////////////
// Position
/////////////////////////////////////////////////////////////////

test(core, reset_position) {
  ESPRotary r = createTestRotary();
  r.resetPosition(7, false);
  assertEqual(r.getPosition(), 7);
}

test(core, reset_position_clamps_to_bounds) {
  ESPRotary r;
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, 0, 10);
  r.resetPosition(25, false);
  assertEqual(r.getPosition(), 10);
}

/////////////////////////////////////////////////////////////////
// Bounds
/////////////////////////////////////////////////////////////////

test(core, set_and_get_bounds) {
  ESPRotary r = createTestRotary();
  r.setUpperBound(100);
  r.setLowerBound(10);
  assertEqual(r.getUpperBound(), 100);
  assertEqual(r.getLowerBound(), 10);
}

test(core, upper_bound_cannot_drop_below_lower) {
  ESPRotary r = createTestRotary();
  r.setLowerBound(10);
  r.setUpperBound(100);
  // Try to move the upper bound below the lower bound: it is refused.
  r.setUpperBound(5);
  assertEqual(r.getUpperBound(), 10);
}

/////////////////////////////////////////////////////////////////
// Increment & steps per click
/////////////////////////////////////////////////////////////////

test(core, increment) {
  ESPRotary r = createTestRotary();
  assertEqual(r.getIncrement(), 1);
  r.setIncrement(10);
  assertEqual(r.getIncrement(), 10);
}

test(core, steps_per_click_floors_at_one) {
  ESPRotary r = createTestRotary();
  assertEqual(r.getStepsPerClick(), CLICKS_PER_STEP);
  r.setStepsPerClick(0);
  assertEqual(r.getStepsPerClick(), 1);
}

/////////////////////////////////////////////////////////////////
// Direction
/////////////////////////////////////////////////////////////////

test(core, direction_default_is_undefined) {
  ESPRotary r = createTestRotary();
  assertTrue(r.getDirection() == rotary_direction::undefined);
}

test(core, direction_to_string) {
  ESPRotary r = createTestRotary();
  assertTrue(r.directionToString(rotary_direction::right) == "right");
  assertTrue(r.directionToString(rotary_direction::left) == "left");
  assertTrue(r.directionToString(rotary_direction::undefined) == "undefined");
}

/////////////////////////////////////////////////////////////////
// Speedup configuration
/////////////////////////////////////////////////////////////////

test(core, speedup_toggle) {
  ESPRotary r = createTestRotary();
  assertFalse(r.isSpeedupEnabled());
  r.enableSpeedup(true);
  assertTrue(r.isSpeedupEnabled());
}

test(core, speedup_interval_and_increment) {
  ESPRotary r = createTestRotary();
  r.setSpeedupInterval(120);
  r.setSpeedupIncrement(8);
  assertEqual(r.getSpeedupInterval(), 120);
  assertEqual(r.getSpeedupIncrement(), 8);
}

test(core, not_in_speedup_by_default) {
  ESPRotary r = createTestRotary();
  assertFalse(r.isInSpeedup());
}

/////////////////////////////////////////////////////////////////

void setup() {
  delay(100);
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {}
  Serial.println(F("\n\nESPRotary Core Tests"));
  Serial.println(F("Using EpoxyDuino + AUnit"));
  TestRunner::setVerbosity(Verbosity::kDefault);
  TestRunner::setTimeout(90);
}

/////////////////////////////////////////////////////////////////

void loop() {
  aunit::TestRunner::run();
}

/////////////////////////////////////////////////////////////////
