# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased
- Added option to define initial value in the constructor as suggested by [ab-it](https://github.com/ab-it) in [#20] (https://github.com/LennartHennigs/ESPRotary/issues/20)
- Fixed upper_bound overflow preventing exemple to work on arduino Uno as suggested by [sblaisot](https://github.com/sblaisot) in pull request [#22] (https://github.com/LennartHennigs/ESPRotary/pull/22) 
- Avoid triggering change_cb after resetPosition to other position than 0

## [1.4] - 2020-06-26

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
