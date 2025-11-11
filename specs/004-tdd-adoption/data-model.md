# Data Model: TDD Adoption

**Feature**: 004-tdd-adoption  
**Date**: 2025-11-11

## Overview

This document defines the key entities, relationships, and state transitions for ShowMIDI's test infrastructure. Unlike business domain models, this focuses on test artifacts, configuration, and execution metadata.

## Entities

### Test Suite

A collection of test cases organized by scope and platform.

**Attributes**:
- `name` (String): Suite identifier (e.g., "ShowMIDI_UnitTests", "ShowMIDI_IntegrationTests")
- `scope` (Enum): Unit | Integration | System
- `platform` (Enum): macOS | Windows | Linux | iOS | All
- `testCases` (Array<TestCase>): Contained test cases
- `tags` (Array<String>): Labels for filtering (e.g., "midi", "ui", "fast", "slow")
- `estimatedDuration` (Double): Expected runtime in seconds

**Validation Rules**:
- `name` must be unique within project
- `scope` determines directory placement (Unit/Integration/System)
- Platform-specific suites only execute on matching CI runners

**Relationships**:
- Contains many `TestCase`
- Executed by `TestRunner`
- Produces `TestReport`

---

### Test Case

A deterministic specification of expected behavior.

**Attributes**:
- `category` (String): JUCE UnitTest category (e.g., "MIDI", "UI", "Theme")
- `name` (String): Descriptive test name
- `setup` (Function): Fixture initialization
- `action` (Function): System under test invocation
- `assertions` (Array<Assertion>): Expected outcomes
- `teardown` (Function): Cleanup
- `tags` (Array<String>): Filter labels (inherited from suite + test-specific)

**Validation Rules**:
- Test names within a category must be unique
- Setup/teardown must not throw exceptions
- Assertions must use `expect()` or `expectEquals()` (JUCE UnitTest API)

**Relationships**:
- Belongs to one `TestSuite`
- Uses zero or more `Fixture`
- Produces `TestResult`

**State Transitions**:
```
Pending → Running → [Passed | Failed | Skipped]
```

---

### Fixture

Reusable setup data or environment configuration.

**Attributes**:
- `name` (String): Fixture identifier (e.g., "MockMidiAdapter", "TestThemeFile")
- `type` (Enum): Mock | Stub | Fake | TestAsset
- `initializationData` (Variant): Configuration (e.g., file path, MIDI message sequence)
- `cleanup` (Function): Resource deallocation

**Validation Rules**:
- Fixtures must be deterministic (no random data, no network I/O)
- Test assets must reside in `Tests/Fixtures/` directory
- Mocks/stubs implement adapter interfaces (e.g., `IMidiDeviceAdapter`)

**Relationships**:
- Used by many `TestCase`
- References `TestAsset` (if type == TestAsset)

**Examples**:
- `MockMidiAdapter`: Injects MIDI messages without hardware
- `TestThemeFile`: Predefined SVG theme (`Tests/Fixtures/dark-theme.svg`)
- `SimulatedTimeProvider`: Deterministic clock for timing tests

---

### Test Result

Outcome of a single test case execution.

**Attributes**:
- `testCaseName` (String): Reference to executed test
- `status` (Enum): Passed | Failed | Skipped
- `duration` (Double): Execution time in milliseconds
- `failureMessage` (String | Null): Assertion failure details (if status == Failed)
- `platform` (String): Execution platform (macOS 14.2, Ubuntu 22.04, etc.)
- `timestamp` (DateTime): Execution start time

**Validation Rules**:
- `failureMessage` required if status == Failed
- `duration` must be >= 0

**Relationships**:
- Produced by one `TestCase`
- Aggregated into `TestReport`

---

### Test Report

Aggregated results for a full test suite execution.

**Attributes**:
- `suiteName` (String): Test suite identifier
- `platform` (String): Execution platform
- `totalTests` (Int): Total test cases
- `passedTests` (Int): Passed count
- `failedTests` (Int): Failed count
- `skippedTests` (Int): Skipped count
- `totalDuration` (Double): Total runtime in seconds
- `coveragePercentage` (Double | Null): Line coverage (if available)
- `timestamp` (DateTime): Report generation time
- `results` (Array<TestResult>): Individual test results

**Validation Rules**:
- `totalTests` == `passedTests` + `failedTests` + `skippedTests`
- `coveragePercentage` must be 0.0–100.0 (if not null)

**Relationships**:
- Aggregates many `TestResult`
- Stored as `CIArtifact` (JUnit XML, HTML)

**Output Formats**:
- JUnit XML (for GitHub Actions UI)
- HTML (human-readable, uploaded as artifact)
- JSON (for badge generation, trend analysis)

---

### Platform Matrix

Configuration defining CI test execution environments.

**Attributes**:
- `platforms` (Array<PlatformConfig>): Supported OS configurations
- `testLabels` (Map<String, Array<String>>): Platform-specific test filters
- `parallelJobs` (Int): Concurrent CI runners

**PlatformConfig**:
- `os` (String): macOS-latest, ubuntu-latest, windows-latest
- `runner` (String): GitHub Actions runner label
- `enabledFormats` (Array<String>): Plugin formats to test (Standalone, VST3, AU, etc.)
- `additionalDeps` (Array<String>): Platform-specific dependencies (ALSA, X11, etc.)

**Validation Rules**:
- iOS tests only registered when `os` == macOS
- ALSA/X11 dependencies required when `os` == ubuntu

**Relationships**:
- Defines execution for `TestSuite`
- Referenced by CI workflow configuration

---

### Component Under Test (CUT)

Any module or feature with defined inputs/outputs and observable behavior.

**Attributes**:
- `componentName` (String): Source file name (e.g., "MidiDeviceComponent", "Theme")
- `responsibilities` (Array<String>): What the component does
- `dependencies` (Array<String>): Other components it relies on
- `testCoverage` (Double): Current line coverage percentage

**Validation Rules**:
- `testCoverage` updated after each coverage run
- Components with <50% coverage flagged for baseline expansion

**Relationships**:
- Tested by one or more `TestCase`
- May depend on other `Component Under Test`

**Examples**:
- `MidiDeviceComponent`: Displays MIDI messages, manages channel state
- `SettingsManager`: Loads/saves user preferences
- `Theme`: Parses SVG themes, applies colors

---

## State Transitions

### Test Execution Lifecycle

```
Test Suite: Queued → Running → Completed → [Reported | Archived]
Test Case:  Pending → Running → [Passed | Failed | Skipped]
Fixture:    Uninitialized → Initialized → Cleanup → Destroyed
```

### Coverage Tracking

```
Component: Uncovered (0%) → Baseline Coverage (50%) → Target Coverage (70%) → Full Coverage (80%+)
```

**Triggers**:
- Baseline phase (Week 1): Identify critical flows, achieve 90% qualitative coverage
- Stabilization (Weeks 2-3): Reach 50% line coverage across components
- Expansion (Weeks 4-6): Reach 70% line coverage (soft gate)
- Enforcement (Weeks 7-8): Reach 80% line coverage (hard gate for new code)

---

## Validation Summary

| Entity | Key Constraints |
|--------|----------------|
| Test Suite | Unique name, platform-specific registration |
| Test Case | Category+name unique, assertions required |
| Fixture | Deterministic, adapter pattern for mocks |
| Test Result | Status determines required fields |
| Test Report | Sum invariant (total == passed + failed + skipped) |
| Platform Matrix | iOS tests only on macOS |
| Component Under Test | Coverage percentage 0.0–100.0 |

---

## Integration with Constitution

- **Multi-Platform**: Platform Matrix entity ensures macOS/Windows/Linux/iOS parity
- **JUCE Standards**: Fixtures use JUCE types (MidiMessage, String, etc.)
- **Real-Time Performance**: Deterministic fixtures prevent timing issues
- **Maintainability**: Clear entity relationships document test architecture
