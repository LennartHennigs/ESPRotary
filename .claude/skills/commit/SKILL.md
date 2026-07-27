---
name: commit
description: Stage and commit current changes for ESPRotary — checks for needed CHANGELOG/README/CLAUDE.md updates, creates a branch if on master, writes a commit message, and commits
---

Commit the current working changes. Follow these steps:

## Step 1: Understand what changed

Run `git diff --stat` and `git status` to see what files are modified or untracked. Read the diffs for any changed source files (`src/`, `test/`, `examples/`) to understand the nature of the changes.

## Step 2: Branch check

Run `git branch --show-current`. If on `master` or `main`:
- Infer a short branch name from the changes (e.g. `fix/phantom-event`, `feat/context`, `docs/readme-update`)
- Run `git checkout -b <branch-name>`
- Tell the user which branch was created

## Step 3: Check CHANGELOG

If any source files (`src/`, `test/`, `examples/`) changed, check whether the `## Unreleased` section of `CHANGELOG.md` documents those changes. If not, add the missing entries under `## Unreleased` following the established format (American English spelling):
- Bug fixes, new API, behavioral changes, and internal/test/infra changes each as a concise bullet
- Reference the relevant issue/PR with a Markdown link where applicable

Skip changelog update for whitespace-only, comment, or docs-only changes.

## Step 4: Check README

If the public API changed (new/removed/renamed methods in `src/ESPRotary.h`), check whether `README.md` reflects the change — both the prose sections and the "Class Definition" code block. If not, update them. Skip for internal or test-only changes.

## Step 5: Check CLAUDE.md

If a non-obvious pattern, gotcha, or invariant was introduced that future sessions should know about (e.g. the `last_steps` sync invariant), add a concise note to `CLAUDE.md`. Do not add obvious or one-off information.

## Step 6: Stage and commit

Stage all relevant files:
```bash
git add <changed source files> CHANGELOG.md  # plus README.md / CLAUDE.md if updated
```

Write a conventional commit message:
- `fix:` for bug fixes
- `feat:` for new features
- `test:` for test-only changes
- `docs:` for documentation only
- `chore:` / `release:` for tooling, config, infra, version bumps

Keep the subject line under 72 characters. Add a short body if the why isn't obvious from the subject.

Run `git commit -m "..."` and confirm success.
