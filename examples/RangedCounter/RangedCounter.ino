/////////////////////////////////////////////////////////////////

#include "Button2.h" //  https://github.com/LennartHennigs/Button2
#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1 D1
#define ROTARY_PIN2 D2
#define BUTTON_PIN  D4

#define CLICKS_PER_STEP 4   // this number depends on your rotary encoder
#define MIN_POS         4
#define MAX_POS         20
#define START_POS       10
#define INCREMENT       2   // this number is the counter increment on each step

/////////////////////////////////////////////////////////////////

ESPRotary r;
Button2 b;    // https://github.com/LennartHennigs/Button2

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  delay(50);

  b.begin(BUTTON_PIN);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);

  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, MIN_POS, MAX_POS, START_POS, INCREMENT);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  Serial.println("\n\nRanged Counter");
  Serial.println("You can only set values between " + String(MIN_POS) + " and " + String(MAX_POS) +".");
  Serial.print("Increment: ");
  Serial.println(r.getIncrement());
  Serial.print("Current position: ");
  Serial.println(r.getPosition());
  Serial.println();
}

void loop() {
  r.loop();
  b.loop();
}

/////////////////////////////////////////////////////////////////

// on change
void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}

// single click
void click(Button2& btn) {
  Serial.println("Click!");
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}

/////////////////////////////////////////////////////////////////