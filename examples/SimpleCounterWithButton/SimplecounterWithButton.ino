/////////////////////////////////////////////////////////////////
// Uses https://github.com/LennartHennigs/Button2
/////////////////////////////////////////////////////////////////

#include "Button2.h";
#include "ESPRotary.h";

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1	14
#define ROTARY_PIN2	12
#define BUTTON_PIN	2

/////////////////////////////////////////////////////////////////

ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2);
Button2 b = Button2(BUTTON_PIN);

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSimple Counter");
  
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  b.setClickHandler(showPosition);
  b.setLongClickHandler(resetPosition);
}

void loop() {
  r.loop();
  b.loop();
}

/////////////////////////////////////////////////////////////////

void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
}


void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}


void showPosition(Button2& btn) {
  Serial.println(r.getPosition());
}

void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
  Serial.println(r.getPosition());  
}

/////////////////////////////////////////////////////////////////