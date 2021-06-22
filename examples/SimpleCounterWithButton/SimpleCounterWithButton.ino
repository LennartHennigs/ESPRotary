/////////////////////////////////////////////////////////////////

#include "Button2.h" //  https://github.com/LennartHennigs/Button2
#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1	D1
#define ROTARY_PIN2	D2
#define BUTTON_PIN	D4

#define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 


/////////////////////////////////////////////////////////////////

ESPRotary r;
Button2 b;

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSimple Counter");
  
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  b.begin(BUTTON_PIN);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);
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
