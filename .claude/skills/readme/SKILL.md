---
name: readme
description: Audit README.md against the current ESPRotary API and recent CHANGELOG entries, then apply any needed updates
---

Audit and update `README.md` to reflect the current state of the library.

## Step 1: Gather context

- Read `src/ESPRotary.h` — extract every public method signature, enum, and constant
- Read the `## Unreleased` and most recent versioned section of `CHANGELOG.md`
- Read `README.md` in full

## Step 2: Identify gaps

Compare the public API in `ESPRotary.h` against what README documents. Flag:
- Public methods present in the header but missing or outdated in README (check both the prose sections and the "Class Definition" code block)
- Methods in README that no longer exist in the header
- Default values or bounds/timing constants in README that differ from the header
- New features or examples mentioned in CHANGELOG that have no README coverage

Do not flag internal helpers, private methods (`_`-prefixed), or test infrastructure.

## Step 3: Propose changes

List each gap with a one-line description of what needs to change. If $ARGUMENTS contains "audit", stop here and show the list without editing.

## Step 4: Apply updates

For each gap, edit `README.md` in place:
- Match the existing style, formatting, and section structure exactly
- Keep the "Class Definition" code block in sync with the header
- For new API entries, insert them in the same section and format as adjacent entries
- For removed entries, delete them cleanly without leaving stubs or comments
- Keep code examples consistent with the Arduino/ESP conventions in the file (no STL, no smart pointers)

Report what was changed when done.
