# Changelog

## Unreleased
- added speedup functionality
- refactored loop and events triggering
- multiple similar events can now be surpressed
- Added ESP8266 and ESP32 interrupt examples
- fixed bug with ID

## [1.6.1] - 2022-12-24

- replaced direction constants with enum
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
- removed ```#pragma``` message for ```moves_per_click``` contructor parameter
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
