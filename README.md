# ESPRotary

Arduino/ESP library to simplify reading rotary encoder data.

- Author: Lennart Hennigs (<https://www.lennarthennigs.de>)
- Copyright (C) 2017-2023 Lennart Hennigs.
- Released under the MIT license.

## Description

This library allows you read out interactions with a rotary encoder and act on them.
It uses callback functions to be notified when the rotary encoder changes.

It has been tested with Arduino, ESP8266 and ESP32 devices.

To see the latest changes to the library please take a look at the [Changelog](https://github.com/LennartHennigs/ESPRotary/blob/master/CHANGELOG.md).

If you find this library helpful please consider giving it a ⭐️ at [GitHub](https://github.com/LennartHennigs/ESPRotary) and/or [buy me a ☕️](https://ko-fi.com/lennart0815). Thanks!

Some of the code based of this library is based on code from [PJRC](https://www.pjrc.com/teensy/td_libs_Encoder.html).

## How to Use

### Definition

- Use the `begin()` or the parameterized constructor to create a new instance of the class
- Encoder produce different numbers of "click" on a single turn.
- You can specify the number of clicks in the constructor, or via a setter function
  - `void setStepsPerClick(int steps)`
  - `int getStepsPerClick() const`

### Callback Handlers

- The library provides several callback handlers to detect events
  - `void setChangedHandler(CallbackFunction f)`
  - `void setRightRotationHandler(CallbackFunction f)`
  - `void setLeftRotationHandler(CallbackFunction f)`
  - `void setUpperOverflowHandler(CallbackFunction f)`
  - `void setLowerOverflowHandler(CallbackFunction f)`
- The library does not detect button clicks. You have to use a separate library for this, e.g. my [Button2](https://github.com/LennartHennigs/Button2) library.

### Ranges

- In the constructor you can define an upper and a lower threshold. The encoder value will not be bypass these values.
- There are also getter and setter functions the these values
  - `void setUpperBound(int upper_bound)`
  - `void setLowerBound(int lower_bound)`
  - `int getUpperBound() const`
  - `int getLowerBound() const`
- See the [RangedCounter](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/RangedCounter/RangedCounter.ino) example for details.

### Retrigger (out of bound) events?

- If you want that out of bound events are only triggered once a bound is reached, you can set:
  - `retriggerEvent(false)`
- Otherwise each (out of bounds) turn will re-trigger the event.
- If you want to disable rotate events that would go beyond your defined boundary you can use:
  - `void triggerOnBounds(false)`

### Reading out information

- The class allows you the get the position and the direction after a click using these function:
  - `int getPosition() const`
  - `byte getDirection() const`
  - `String directionToString(byte direction) const`

### Speed

- You can define the speed, i.e. the increment the a single click in the constructor
- There is also a getter and setter function for this
  - `void setIncrement(int inc)`
  - `int getIncrement() const`

### Speedup Mode

- You can also define, that the increment shall change when the encoder is turned fast.
- Use these functions to define the interval (between clicks) and the increment:
  - `void setSpeedupInterval(int time)`
  - `void setSpeedupIncrement(int inc)`
- Per default the interval is set to `75ms` and the increment is set to `5`.
- There are also getter functions for both parameters:
  - `int getSpeedupInterval() const`
  - `int getSpeedupIncrement() const`
- To enable the speedup mode use this member function:
  - `void enableSpeedup(bool enable)`
- ...and to check:
  - `bool isSpeedupEnabled() const`
- There are also event handlers that let you track if the speedup mode was entered or exited:
  - `void setSpeedupStartedHandler(CallbackFunction f)`
  - `void setSpeedupEndedHandler(CallbackFunction f)`
- Alternatively, you can use this function to determine its state:
  - `bool isInSpeedup() const`
- If you have bound defined, the speedup mode will be automatically ended when you hit it.

### The Loop

- For the class to work, you need to call the `loop()` member function in your sketch's `loop()` function.
- Alternatively, you can use a timer interrupt to call the member `loop()`function.
- See the examples for more details.

### Using it with a timer interrupt

- Instead of using the main loop() of your sketch you can also use an interrupt timer function.
- See [ESP32Interrupt](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/ESP32Interrupt/ESP32Interrupt.ino) or [ESP8266Interrupt](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/ESP8266Interrupt/ESP8266Interrupt.ino) to see how

### IDs for Encoder Instances

- Each encoder instance gets a unique (auto incremented) ID upon creation.
- You can get a encoders' ID via `getID()`.
- Alternatively, you can use `setID(int newID)` to set a new one. But then you need to make sure that they are unique.
- You can also use the `==` operator to compare encoders

## Notes

- To see the latest changes to the library please take a look at the [Changelog](https://github.com/LennartHennigs/ESPRotary/blob/master/CHANGELOG.md).
- And if you find this library helpful, please consider giving it a star at [GitHub](https://github.com/LennartHennigs/ESPRotary).  Thanks!

## Examples

- [SimpleCounter](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/SimpleCounter/SimpleCounter.ino) - basic example
- [SimpleCounterWithButton](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/SimpleCounterWithButton/SimpleCounterWithButton.ino) - basic example with a button handler
- [RangedCounter](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/RangedCounter/RangedCounter.ino) - shows how to define ranges
- [Speedup](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/Speedup/Speedup.ino) - shows how to implement the speedup mode
- [ESP8266Interrupt](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/ESP8266Interrupt/ESP8266Interrupt.ino) - uses an ESP8266 interrupt instead of the main loop
- [ESP32Interrupt](https://github.com/LennartHennigs/ESPRotary/blob/master/examples/ESP32Interrupt/ESP32Interrupt.ino) - uses an ESP32 interrupt instead of the main loop

## Class Definition

These are the constructor and the member functions the library provides:

``` c++
  ESPRotary();
  ESPRotary(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int iniital_pos = 0, int increment = 1);

  void begin(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int initial_pos = 0, int increment = 1);

  int getPosition() const;
  void resetPosition(int p = 0, bool fireCallback = true);

  rotary_direction getDirection() const;
  String directionToString(rotary_direction dir) const;

  void setIncrement(int inc);
  int getIncrement() const;

  void triggerOnBounds(bool triggerEvents = true);

  void enableSpeedup(bool enable);
  void setSpeedupInterval(int time);
  void setSpeedupIncrement(int inc);

  bool isSpeedupEnabled() const;
  int getSpeedupInterval() const;
  int getSpeedupIncrement() const;
  bool isInSpeedup() const;

  rotary_event getLastEvent() const;
  void retriggerEvent(bool retrigger);

  void setUpperBound(int upper_bound);
  void setLowerBound(int lower_bound);
  int getUpperBound() const;
  int getLowerBound() const;

  void setStepsPerClick(int steps);
  int getStepsPerClick() const;

  void setChangedHandler(CallbackFunction f);
  void setRightRotationHandler(CallbackFunction f);
  void setLeftRotationHandler(CallbackFunction f);
  void setUpperOverflowHandler(CallbackFunction f);
  void setLowerOverflowHandler(CallbackFunction f);
  void setSpeedupStartedHandler(CallbackFunction f);
  void setSpeedupEndedHandler(CallbackFunction f);

  int getID() const;
  void setID(int newID);

  bool operator == (ESPRotary &rhs);

  void loop();
```

## Installation

Open the Arduino IDE choose "Sketch > Include Library" and search for "ESP Rotary".
Or download the ZIP archive (<https://github.com/lennarthennigs/ESPRotary/zipball/master>), and choose "Sketch > Include Library > Add .ZIP Library..." and select the downloaded file.

## License

MIT License

Copyright (c) 2017-2023 Lennart Hennigs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
