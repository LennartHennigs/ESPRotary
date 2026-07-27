---
name: release
description: Cut a new ESPRotary release — suggests version bump based on change type, updates version files and CHANGELOG, commits, tags, pushes, creates a GitHub release, and publishes to PlatformIO
---

Cut a new ESPRotary release. Follow these steps exactly:

## Step 1: Determine the version

Read `library.json` to get the current version.

Read the `## Unreleased` section of `CHANGELOG.md` to understand what changed.

If $ARGUMENTS is provided, use that as the new version. Otherwise:
- Classify the unreleased changes:
  - **Major** (x.0.0): breaking API changes, removed functions, changed signatures
  - **Minor** (x.y.0): new public functions, new features, new examples
  - **Patch** (x.y.z): bug fixes, internal changes, test additions, docs
- Suggest the new version with a one-line explanation of your reasoning
- Use AskUserQuestion to ask the user to confirm the suggested version or provide a different one

## Step 2: Update version files

Update the version number in **both** (they must match):
- `library.json` — the `"version"` field
- `library.properties` — the `version=` line

## Step 3: Finalize CHANGELOG

In `CHANGELOG.md`:
- Rename `## Unreleased` to `## [x.y.z] - YYYY-MM-DD` (today's date, new version)
- Add a fresh empty `## Unreleased` heading above it
- Use American English spelling

## Step 4: Commit, tag, push

ESPRotary tags use the **bare version number — no `v` prefix** (e.g. `2.2.1`).

```bash
git add library.json library.properties CHANGELOG.md
git commit -m "release: bump version to X.Y.Z"
git tag X.Y.Z
git push && git push --tags
```

## Step 5: Create GitHub release

Extract the release notes for this version from the CHANGELOG (the bullets under `## [x.y.z]`), then run:

```bash
gh release create X.Y.Z --target master --title "X.Y.Z" --notes "<changelog bullets>" --latest
```

The **Arduino Library Manager** picks up the new tag automatically from GitHub — no separate step needed there.

## Step 6: Publish to PlatformIO registry

Requires `pio account login` first (interactive — the maintainer must do this; verify with `pio account show`).

```bash
pio pkg publish --no-interactive
```

`pio pkg publish` packages the current working directory (the `/.pio/` build dir and `.git` are excluded automatically) and uses the version from `library.json`. A published version is **immutable** — it can only be published once, so make sure the release is correct first.

Confirm each step succeeded and report which registries were updated.
