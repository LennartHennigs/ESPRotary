/////////////////////////////////////////////////////////////////
/*
  Shared test helpers for ESPRotary test suites.

  ESPRotary reads its pins directly via digitalRead() (there is no
  injectable pin-state function like Button2 has), so the native
  suites focus on public API / state behavior rather than simulating
  quadrature edges.
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

// Create a rotary encoder configured for tests.
inline ESPRotary createTestRotary() {
  ESPRotary r;
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  return r;
}

/////////////////////////////////////////////////////////////////
