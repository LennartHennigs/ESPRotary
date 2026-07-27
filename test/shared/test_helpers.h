/////////////////////////////////////////////////////////////////
/*
  Shared test helpers for ESPRotary test suites.

  ESPRotary normally reads its pins via digitalRead(). For host tests
  we install a fake pin-read function (setPinReadFunction) and drive
  a simulated quadrature signal, so rotation can be exercised without
  hardware.
*/
/////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <ESPRotary.h>

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1     12
#define ROTARY_PIN2     13
#define CLICKS_PER_STEP 4

/////////////////////////////////////////////////////////////////
// Simulated pin state: bit0 = pin1 level, bit1 = pin2 level.

static uint8_t simPinState = 0;

inline int fakePinRead(uint8_t pin) {
  if (pin == ROTARY_PIN1) return simPinState & 1;
  if (pin == ROTARY_PIN2) return (simPinState >> 1) & 1;
  return 0;
}

/////////////////////////////////////////////////////////////////

// Create a rotary encoder wired to the simulated pins.
inline ESPRotary createTestRotary() {
  simPinState = 0;
  ESPRotary r;
  r.setPinReadFunction(fakePinRead);
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  return r;
}

/////////////////////////////////////////////////////////////////

// One detent right = the quadrature phases 2 -> 3 -> 1 -> 0
// (each transition decodes to +1, so a full detent is +CLICKS_PER_STEP).
inline void turnRight(ESPRotary& r, int detents = 1) {
  static const uint8_t seq[] = {2, 3, 1, 0};
  for (int d = 0; d < detents; d++) {
    for (uint8_t i = 0; i < 4; i++) {
      simPinState = seq[i];
      r.loop();
    }
  }
}

// One detent left = the reverse phases 1 -> 3 -> 2 -> 0 (each -1).
inline void turnLeft(ESPRotary& r, int detents = 1) {
  static const uint8_t seq[] = {1, 3, 2, 0};
  for (int d = 0; d < detents; d++) {
    for (uint8_t i = 0; i < 4; i++) {
      simPinState = seq[i];
      r.loop();
    }
  }
}

/////////////////////////////////////////////////////////////////
