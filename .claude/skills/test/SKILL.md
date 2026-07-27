---
name: test
description: Run ESPRotary test suites via PlatformIO + EpoxyDuino. Optionally pass a suite name (core, context, rotation). Defaults to all suites.
---

Run the ESPRotary native test suites using PlatformIO + EpoxyDuino + AUnit (no hardware required).

Parse $ARGUMENTS:
- If a specific suite is named (core, context, rotation), run `pio test -e test_<suite>`
- If "all" or nothing is given, run `pio test -e test_core -e test_context -e test_rotation`

## Reading results (important)

PlatformIO reports **"0 test cases"** for these suites because it does not parse AUnit's output — a green (exit 0) `pio test` run means the AUnit assertions passed. To see the real per-test summary (e.g. "N passed, 0 failed"), run the built binary directly:

```bash
.pio/build/test_<suite>/program
```

Show the AUnit summary line(s) and report how many tests passed vs failed per suite.

If any suite fails, run its `program` binary and quote the failing assertion lines so the failure is easy to diagnose.
