# Changelog

## Unreleased

- Fixed parameterized constructor delegation — `ESPRotary(pin1, pin2, ...)` now correctly assigns an ID to `this` instead of a discarded temporary
- Added default member initializers for `lower_bound`, `upper_bound`, `state`, `increment`, `last_event`, `dir`, and `steps_per_click` to prevent undefined behavior when `begin()` is called without a prior `ESPRotary()` constructor
- Fixed `directionToString()` returning `"left"` for `rotary_direction::undefined`; it now returns `"undefined"`
- Fixed `operator==` to be `const`-correct: `bool operator==(const ESPRotary& rhs) const`
- Changed `in_speedup` member type from `int` to `bool`
- Fixed typo `inital_pos` → `initial_pos` in constructor and `begin()` signatures
- Fixed invalid escape sequence `"\n\R"` in `Speedup.ino` (was garbling the title line)
- Fixed wrong title `"Simple Counter"` in `SimpleCounterWithButton.ino`
- Fixed inverted comment on `triggerOnBounds` in `RangedCounter.ino` (rotation callbacks are **on** by default; pass `false` to disable them)

**Note**: Unreleased changes are checked in but not part of an official release (available through the Arduino IDE or PlatfomIO) yet. This allows you to test WiP features and give feedback to them.

## [2.1.1] - 2023-04-27

- fixed bug in `setIncrement()` as pointed out by WJurasek-KV
- updated `SimpleCounter.ino` example

## [2.1.0] - 2023-04-13

- updated enums with class feature
- Added `triggerOnBounds(bool triggerEvents = true)` method to be able to suppress rotate events when the boundaries are reached
- Made `now` and time related attributes to `unsigned long`
- Replaced cases with lookup table to simplify `loop()`
- Simplified state change in the `loop()`
- Positions are only set within the defined boundaries
- some refactoring suggested by GPT-4

Most changes were suggested in [#43](https://github.com/LennartHennigs/ESPRotary/pull/43) by [Alex Herman](https://github.com/gaaf). Thank you.

## [2.0.0] - 2022-12-27

- added speedup functionality as suggested in [#37](https://github.com/LennartHennigs/ESPRotary/discussions/37)

  ```c++
  void setSpeedupInterval(int time);
  void setSpeedupIncrement(int inc);
  void enableSpeedup(bool enable);
  
  bool isSpeedupEnabled() const;
  int getSpeedupInterval() const;
  int getSpeedupIncrement() const;

  void setSpeedupStartedHandler(CallbackFunction f);
  void setSpeedupEndedHandler(CallbackFunction f);
  bool isInSpeedup() const;
  ```

- multiple similar events can now be surpressed
  - `retriggerEvent(bool retrigger)`
- Added ESP8266 and ESP32 interrupt examples
- refactored loop and event triggering
- fixed bug with ID

## [1.6.1] - 2022-12-24

- replaced direction constants with enum, now use it like this: `rotary_direction::right`
- refactored `loop()`

## [1.6.0] - 2022-05-26

- added ID feature to the class as suggested by [Jon](https://github.com/mscreations) in [#34](https://github.com/LennartHennigs/ESPRotary/pull/34)
- added == operator as suggested by [Jon](https://github.com/mscreations) in [#34](<https://github.com/LennartHennigs/>
- Defined getter methods as ```const```

## [1.5.0] - 2021-06-22

- Added option to define increment as suggested by [sblaisot](https://github.com/sblaisot) in pull request [#32](https://github.com/LennartHennigs/ESPRotary/pull/32)
- Added a `begin()` function and a default constructor the be able to instantiate an object within a function
- Updated the `RangedCounter` and `SimpleCounterWithButton` examples. They now use `begin()`
- Added getter and setter functions for the upper and lower bounds
  - ```void setUpperBound(int upper_bound);```
  - ```void setLowerBound(int lower_bound);```
  - ```int getUpperBound();```
  - ```int getLowerBound();```

## [1.4.2] - 2020-11-29

- remove unused variable `last_read_ms` as pointed out by [ornea](https://github.com/ornea) in [#26](https://github.com/LennartHennigs/ESPRotary/issues/26)
- cleaned up the protected vars as suggested by [Jim-2249](https://github.com/Jim-2249) in [#30](https://github.com/LennartHennigs/ESPRotary/issues/30)
- simplified the `loop()` function

## [1.4.1] - 2020-11-29

- reordered the constructor parameters to mitigate a code breaking change as suggested by [ab-it](https://github.com/ab-it) in pull request [#25](https://github.com/LennartHennigs/ESPRotary/pull/25)
- made ```resetPosition()``` callback trigger optional as suggested by [ab-it](https://github.com/ab-it)
- fixed bug in ```directionToString()``` introduced in the 1.4 version, found by [ab-it](https://github.com/ab-it)

## [1.4] - 2020-11-26

- added callback to ```resetPosition()``` as suggested by [ab-it](https://github.com/ab-it) in [#17](https://github.com/LennartHennigs/ESPRotary/issues/17)
- Fixed warning in ```directionToString()``` as suggested by [ab-it](https://github.com/ab-it) in [#18](https://github.com/LennartHennigs/ESPRotary/issues/18)
- Added option to define initial value in the constructor as suggested by [ab-it](https://github.com/ab-it) in [#20](https://github.com/LennartHennigs/ESPRotary/issues/20)
- Fixed upper_bound overflow preventing exemple to work on arduino Uno as suggested by [sblaisot](https://github.com/sblaisot) in pull request [#22](https://github.com/LennartHennigs/ESPRotary/pull/22)
- Avoid triggering change_cb after resetPosition to other position than 0

## [1.3] - 2020-06-05

- Fixed typo in example
- updated ```README.md```
- added function ```setStepsPerClick()```, as requested by [asalillo](https://github.com/lasalillo) in [#12](https://github.com/LennartHennigs/ESPRotary/issues/12)
- Fixed bug [#13](https://github.com/LennartHennigs/ESPRotary/issues/13) in ```resetPosition()```, as pointed out by [jjarp](https://github.com/jjarp)
- removed ```#pragma``` message for ```moves_per_click``` constructor parameter
- renamed  variable ```moves_per_click``` to ```steps_per_click```

## [1.2.1] - 2020-04-20

- Fixed bug related to minimum and maximum bounds
- Added ```RangedCounter.ino``` example
- Added optional parameter to  ```resetPosition()```

## [1.2.0] - 2020-04-20

- Created and added CHANGELOG.md
- Added optional minimum and maximum bounds to constructor (as suggested by [cornfeedhobo](https://github.com/cornfeedhobo) in issue [#9](https://github.com/LennartHennigs/ESPRotary/issues/9))

## [1.1.2] - 2020-03-31

- Simplified example

## [1.1.1] - 2020-03-30

- Removed ```yield()``` in main ```loop()``` since it caused some problems

## [1.1.0] - 2020-03-27

- Added ```yield()``` to the main ```loop()```
- Updated the example (as suggested by [per1234](https://github.com/per1234))

## [1.0.0] - 2017-11-14

- Added option to set number of clicks to constructor (as suggested by [svititom](https://github.com/LennartHennigs/ESPRotary/commit/95e86b171e8d1489cd603d493898c5ea516935db))

## Note

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
