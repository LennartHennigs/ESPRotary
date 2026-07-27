---
name: compile
description: Compile ESPRotary examples with arduino-cli to verify they build. Pass an example name, or leave blank to compile all examples for the ESP8266 D1.
---

Compile ESPRotary examples to verify they build cleanly.

The library targets Arduino, ESP8266, and ESP32. The primary verification board is the ESP8266 D1 (`esp8266:esp8266:d1`).

Parse $ARGUMENTS:
- If a specific example name is given, find the matching `.ino` under `examples/` and compile just that one.
- If nothing (or "all") is given, compile every example under `examples/`.

Compile each with:

```bash
arduino-cli compile --fqbn esp8266:esp8266:d1 examples/<Name>
```

If a default-FQBN compile fails on build-option grounds, use the fuller FQBN:
`esp8266:esp8266:d1:xtal=80,vt=flash,exception=disabled,ssl=all,eesz=4M2M,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=921600`

## Notes

- `examples/SimpleCounterWithButton` also requires the **Button2** library to be installed; if it's missing, note that rather than reporting a real failure.
- The `ESP32Interrupt` example targets ESP32 — compile it with `--fqbn esp32:esp32:esp32` if an ESP32 core is installed; otherwise skip and say so.

Report any compiler errors clearly, quoting the relevant error lines. If compilation is clean, confirm which examples passed.
