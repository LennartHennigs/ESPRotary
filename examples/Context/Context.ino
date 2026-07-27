/////////////////////////////////////////////////////////////////
/*
  Shows how to use setContext() / getContext() to route a global
  callback back to the object that "owns" the encoder.

  The callback functions are plain C functions (no captures), so we
  attach a pointer to the owning object as the encoder's context and
  recover it inside the handler via the passed ESPRotary reference.
*/
/////////////////////////////////////////////////////////////////

#include "ESPRotary.h"

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1 D1
#define ROTARY_PIN2 D2

// this number depends on your rotary encoder
#define CLICKS_PER_STEP 4

#define SERIAL_SPEED 115200

/////////////////////////////////////////////////////////////////

// An object that owns a rotary encoder and reacts to its rotation.
class Dial {
 public:
  Dial(const char* name) : name(name) {}

  void begin(byte pin1, byte pin2) {
    r.begin(pin1, pin2, CLICKS_PER_STEP);
    // hand the encoder a pointer back to us...
    r.setContext(this);
    r.setChangedHandler(Dial::onRotate);
  }

  void loop() {
    r.loop();
  }

  // static, so it matches the plain-function callback signature
  static void onRotate(ESPRotary& sender) {
    // ...and recover the owning Dial from the context
    Dial* self = static_cast<Dial*>(sender.getContext());
    Serial.print(self->name);
    Serial.print(": ");
    Serial.println(sender.getPosition());
  }

 private:
  const char* name;
  ESPRotary r;
};

/////////////////////////////////////////////////////////////////

Dial volume("Volume");

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(SERIAL_SPEED);
  delay(50);
  Serial.println("\n\nContext Example");

  volume.begin(ROTARY_PIN1, ROTARY_PIN2);
}

void loop() {
  volume.loop();
}

/////////////////////////////////////////////////////////////////
