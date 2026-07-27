/////////////////////////////////////////////////////////////////
/*
  Tests for the setContext() / getContext() feature (issue #48).
  A void* context is stored on the encoder and retrieved inside a
  callback via the passed ESPRotary&, letting a global callback be
  routed back to the object that owns the encoder.
*/
/////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <AUnitVerbose.h>
#include "../shared/test_helpers.h"

using namespace aunit;

/////////////////////////////////////////////////////////////////

#define SERIAL_SPEED 115200

/////////////////////////////////////////////////////////////////

// Stand-in for the "object that owns the rotary" from issue #48.
struct Owner {
  int handled = 0;
};

static Owner callbackOwner;

// Free-function callback that recovers its owner through the context.
void onChange(ESPRotary& r) {
  Owner* owner = static_cast<Owner*>(r.getContext());
  if (owner != nullptr) owner->handled++;
}

/////////////////////////////////////////////////////////////////

test(context, defaults_to_null) {
  ESPRotary r = createTestRotary();
  assertTrue(r.getContext() == nullptr);
}

test(context, set_and_get) {
  ESPRotary r = createTestRotary();
  Owner owner;
  r.setContext(&owner);
  assertTrue(r.getContext() == &owner);
}

test(context, retrievable_inside_callback) {
  ESPRotary r = createTestRotary();
  callbackOwner.handled = 0;
  r.setContext(&callbackOwner);
  r.setChangedHandler(onChange);

  // resetPosition(..., true) fires the changed handler.
  r.resetPosition(3, true);

  assertEqual(callbackOwner.handled, 1);
}

/////////////////////////////////////////////////////////////////

void setup() {
  delay(100);
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {}
  Serial.println(F("\n\nESPRotary Context Tests"));
  Serial.println(F("Using EpoxyDuino + AUnit"));
  TestRunner::setVerbosity(Verbosity::kDefault);
  TestRunner::setTimeout(90);
}

/////////////////////////////////////////////////////////////////

void loop() {
  aunit::TestRunner::run();
}

/////////////////////////////////////////////////////////////////
