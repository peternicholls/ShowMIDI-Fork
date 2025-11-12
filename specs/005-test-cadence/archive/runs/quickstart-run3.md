# Quickstart — Testing Cadence (Run 3)

This guide explains how to use the testing cadence locally and what to expect in CI.

## Local (Developer)

- Pre-commit: Run formatting and linting with auto-fix.
- Pre-push: Run unit tests.

Suggested commands:

macOS build:

```zsh
cd Builds/MacOSX
xcodebuild -project ShowMIDI.xcodeproj -scheme ShowMIDI clean build
```

Linux build:

```zsh
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Tests (if enabled):

```zsh
cd build
ctest --output-on-failure
```

## Pull Requests (PR)

- Required checks: unit, integration (core), static analysis, formatting/linting, build verification.
- Time budgets: median ≤ 5m, p95 ≤ 10m.
- Platforms: macOS by default; platform-conditional checks run when relevant files change.

## Nightly (Scheduled)

- Runs extended suites: system, performance, UI/visual regression on macOS, Windows, Linux.
- Change-detection skip: Nightly may skip if no code changes since last success.

## Release / Hotfix

- Release: full platform validation + packaging + installer smoke (blocking).
- Hotfix: safety-critical subset for speed.

## Flake Governance

- Auto-detect flakiness (3/10 failures or ≥20%/7 days), quarantine from required gates, assign owner, re-enable at ≥95%/14 days with sign-off.

## Skip/Bypass

- Requires documented justification and approval; all events are audited and reported.
