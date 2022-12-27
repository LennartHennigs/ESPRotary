/////////////////////////////////////////////////////////////////

#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1 D1
#define ROTARY_PIN2 D2

#define CLICKS_PER_STEP 4   // this number depends on your rotary encoder
#define MIN_POS         0
#define MAX_POS         100
#define START_POS       50
#define INCREMENT       1   // this number is the counter increment on each step

#define SPEEDUP_STEPS   5

#define SERIAL_SPEED    115200


/////////////////////////////////////////////////////////////////

ESPRotary r;

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(SERIAL_SPEED);
  delay(50);

  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, MIN_POS, MAX_POS, START_POS, INCREMENT);
  
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);
  r.setLowerOverflowHandler(lower);
  r.setUpperOverflowHandler(upper);
  r.setSpeedupStartedHandler(speedupStarted);
  r.setSpeedupEndedHandler(speedupEnded);

  r.retriggerEvent(false);
  r.enableSpeedup(true);
  r.setSpeedupIncrement(SPEEDUP_STEPS);


  Serial.println("\n\Ranged & Speedup");
  Serial.println("You can only set values between " + String(MIN_POS) + " and " + String(MAX_POS) +".");
  Serial.print("Current position: ");
  Serial.println(r.getPosition());
  Serial.println();
}

void loop() {
  r.loop();
}

/////////////////////////////////////////////////////////////////

void speedupStarted(ESPRotary& r) {
   Serial.println("Speedup started");
}

void speedupEnded(ESPRotary& r) {
   Serial.println("Speedup ended");
}

// on change
void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
}


// out of bounds event
void upper(ESPRotary& r) {
   Serial.println("upper bound hit");
}

// out of bounds event
void lower(ESPRotary& r) {
   Serial.println("lower bound hit");
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}

/////////////////////////////////////////////////////////////////