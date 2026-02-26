# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESPRotary is an Arduino/ESP library (v2.1.1) for reading rotary encoder values via callback functions. It targets Arduino, ESP8266, and ESP32 devices and is distributed through the Arduino IDE Library Manager and PlatformIO.

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

## Development

This is a header-only-style Arduino library — there is no build system or test runner. Development is done in the Arduino IDE or VS Code with the Arduino extension.

The `.vscode/arduino.json` is configured for an ESP8266 D1 Mini on `/dev/tty.usbserial-1410`. To verify/upload a sketch, open one of the examples and use the Arduino IDE or VS Code Arduino extension commands.

**To test changes**, upload an example sketch to hardware:
- `examples/SimpleCounter` — basic rotation and direction callbacks
- `examples/RangedCounter` — bounds and overflow callbacks
- `examples/Speedup` — speedup mode
- `examples/ESP32Interrupt` / `examples/ESP8266Interrupt` — timer interrupt usage instead of `loop()`

## Release Process

Update `library.properties` and `library.json` with the new version number, then add an entry to `CHANGELOG.md`.
