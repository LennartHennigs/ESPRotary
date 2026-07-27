# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESPRotary is an Arduino/ESP library (v2.2.1) for reading rotary encoder values via callback functions. It targets Arduino, ESP8266, and ESP32 devices and is distributed through the Arduino IDE Library Manager and PlatformIO.

## Architecture

The library is a single class (`ESPRotary`) split into two files:

- `src/ESPRotary.h` — class definition, two enums (`rotary_direction`, `rotary_event`), and the `CallbackFunction` typedef
- `src/ESPRotary.cpp` — implementation

**Core flow:** The sketch calls `r.loop()` on every iteration (or via a timer interrupt). Inside `loop()`, `_wasRotated()` reads the hardware pins using a 16-entry lookup table (`factors[]`) to decode the quadrature signal into step increments. If a threshold is crossed, direction is determined, optional speedup logic runs, bounds are checked, and the appropriate callbacks are fired via `_setEvent()`.

**Key design points:**
- Position is tracked internally as `steps` (raw step count); `getPosition()` divides by `steps_per_click`
- `_isWithinBounds()` clamps `steps` to the boundary and optionally fires overflow callbacks
- Speedup mode (`enableSpeedup`) increases the effective increment when turns happen faster than `speedup_interval` ms
- `retriggerEvent(false)` suppresses repeated boundary events; `triggerOnBounds(false)` suppresses rotation callbacks when at a boundary
- Each instance gets a static auto-incremented `id`
- `setContext(void*)` / `getContext()` store an arbitrary pointer on the instance (usually the owning object) so the plain-function callbacks can recover custom state via the passed `ESPRotary&`
- **Invariant:** `_wasRotated()` fires on `abs(steps - last_steps)` crossing a threshold, so anywhere `steps` is assigned directly (outside `loop()` — i.e. `resetPosition()` and `begin()`'s `initial_pos`) must also set `last_steps = steps`, or the next `loop()` decodes a phantom rotation/change event
- `setPinReadFunction(fn)` overrides how the pins are read (defaults to `digitalRead`); it exists as a test seam so rotation can be simulated on the host

## Development

Development is done in the Arduino IDE or VS Code with the Arduino extension.

The `.vscode/arduino.json` is configured for an ESP8266 D1 Mini on `/dev/tty.usbserial-1410`. To verify/upload a sketch, open one of the examples and use the Arduino IDE or VS Code Arduino extension commands.

**Native tests** run on the host via PlatformIO + EpoxyDuino + AUnit (no hardware needed), mirroring the sibling Button2 library:

```
pio test -e test_core      # core behavior (position, bounds, direction, speedup, IDs)
pio test -e test_context   # setContext / getContext
pio test -e test_rotation  # decode/event engine via the setPinReadFunction() seam
```

The `test_rotation` suite drives a simulated quadrature signal through `setPinReadFunction()` (a settable pin-read override that defaults to `digitalRead`), letting `loop()`, the decode table, and event callbacks be tested without hardware.

Test sources live in `test/test_*/` with shared helpers in `test/shared/test_helpers.h`; env config is in `platformio.ini`.

**Project skills** (`.claude/skills/`, mirrored from the Button2 library) automate the common workflows — `test`, `compile`, `commit`, `readme`, and `release`. Prefer them over ad-hoc commands; the `release` skill in particular encodes the exact publish procedure below. Note: PlatformIO reports "0 test cases" because it doesn't parse AUnit's output — a green (exit 0) run means the AUnit assertions passed; run `.pio/build/<env>/program` directly to see the per-test summary.

**To test changes**, upload an example sketch to hardware:
- `examples/SimpleCounter` — basic rotation and direction callbacks
- `examples/RangedCounter` — bounds and overflow callbacks
- `examples/Speedup` — speedup mode
- `examples/Context` — passing an owning object to callbacks via `setContext()`
- `examples/ESP32Interrupt` / `examples/ESP8266Interrupt` — timer interrupt usage instead of `loop()`

## Release Process

Follow semver: a new public method (e.g. `setContext`) is a minor bump; a bug fix is a patch.

1. Bump the version in **both** `library.properties` (`version=`) and `library.json` (`"version"`) — they must match.
2. Add a dated entry to `CHANGELOG.md` (American English spelling).
3. Make sure all native tests pass (`pio test -e test_core -e test_context -e test_rotation`) and an example still compiles for ESP8266.
4. Merge to `master`.
5. Tag + create a GitHub release. Tags use the **bare version number** (e.g. `2.2.1`, no `v` prefix):
   ```
   gh release create 2.2.1 --target master --title "2.2.1" --notes-file <notes> --latest
   ```
   The **Arduino Library Manager** auto-indexes new release tags — no further action.
6. Publish to the **PlatformIO registry** (requires `pio account login` first — interactive, must be done by the maintainer):
   ```
   pio pkg publish --no-interactive
   ```
   `pio pkg publish` packages the current working directory (the `/.pio/` build dir and `.git` are excluded automatically) and uses the version from `library.json`. A published version is immutable — it can only be published once.
