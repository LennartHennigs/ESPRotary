/////////////////////////////////////////////////////////////////

#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1 D1
#define ROTARY_PIN2 D2

#define CLICKS_PER_STEP 4   // this number depends on your rotary encoder
#define MIN_POS         0
#define MAX_POS         40
#define START_POS       20
#define INCREMENT       2   // this number is the counter increment on each step

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
  
  // enable to be only notified one about an out-of-bounds hit
  // r.retriggerEvent(false);

  Serial.println("\n\nRanged Counter");
  Serial.println("You can only set values between " + String(MIN_POS) + " and " + String(MAX_POS) +".");
  Serial.print("Current position: ");
  Serial.println(r.getPosition());
  Serial.println();
}

void loop() {
  r.loop();
}

/////////////////////////////////////////////////////////////////

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