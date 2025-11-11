# TDD Protocol: ShowMIDI Development Workflow

**Version**: 1.0.0  
**Effective**: Week 1 of TDD Adoption (004-tdd-adoption)  
**Audience**: All ShowMIDI contributors

## Overview

This document defines the test-driven development workflow for ShowMIDI. Following this protocol ensures consistent quality, facilitates code review, and builds confidence in cross-platform behavior.

## Red-Green-Refactor Cycle

### Step 1: Red (Write a Failing Test)

Before writing any implementation:

1. **Identify the behavior**: What should the system do? (derived from spec or user story)
2. **Write the test first**: Express expected behavior as assertions
3. **Run the test**: Verify it fails (confirms test is valid)

**Example** (MIDI message display):
```cpp
class MidiMessageDisplayTests : public juce::UnitTest
{
public:
    MidiMessageDisplayTests() : juce::UnitTest("MIDI Message Display", "MIDI") {}
    
    void runTest() override
    {
        beginTest("Note On message should display on correct channel");
        
        // Arrange
        MidiDeviceComponent component;
        MockMidiAdapter mockDevice;
        component.setDeviceAdapter(&mockDevice);
        
        // Act
        mockDevice.sendMessage(juce::MidiMessage::noteOn(1, 60, 0.8f));
        
        // Assert
        expect(component.isChannelActive(1), "Channel 1 should be active");
        expectEquals(component.getLastNote(1), 60, "Note 60 should be displayed");
    }
};

static MidiMessageDisplayTests midiMessageDisplayTests;
```

### Step 2: Green (Make It Pass)

Write **minimal code** to make the test pass:

1. Implement just enough logic to satisfy the assertion
2. Run the test: verify it passes
3. Run **all** tests: ensure no regressions

**Example**:
```cpp
void MidiDeviceComponent::setDeviceAdapter(IMidiDeviceAdapter* adapter)
{
    deviceAdapter = adapter;
    // Minimal implementation: just store reference
}

bool MidiDeviceComponent::isChannelActive(int channel) const
{
    return activeChannels[channel]; // Assume this is updated elsewhere
}
```

### Step 3: Refactor (Improve Without Changing Behavior)

With passing tests as a safety net:

1. **Eliminate duplication**: Extract common patterns
2. **Improve readability**: Rename variables, extract methods
3. **Optimize**: Only if profiling shows need
4. **Run tests after each change**: Ensure green stays green

**Example** (extract fixture):
```cpp
// Before: Duplication in every test
MockMidiAdapter mockDevice;
component.setDeviceAdapter(&mockDevice);

// After: Shared fixture
struct MidiTestFixture
{
    MidiDeviceComponent component;
    MockMidiAdapter mockDevice;
    
    MidiTestFixture()
    {
        component.setDeviceAdapter(&mockDevice);
    }
};
```

---

## Test Scopes

ShowMIDI uses three test scopes:

### Unit Tests

**Purpose**: Validate individual components in isolation.

**Characteristics**:
- No external dependencies (files, network, hardware)
- Fast (<100ms per test)
- Use mocks/stubs for dependencies

**Location**: `Tests/Unit/`

**Example**:
```cpp
class ThemeParsingTests : public juce::UnitTest
{
public:
    ThemeParsingTests() : juce::UnitTest("Theme Parsing", "UI") {}
    
    void runTest() override
    {
        beginTest("Dark theme should parse primary color");
        
        Theme theme;
        juce::File themeFile = juce::File::getCurrentWorkingDirectory()
            .getChildFile("Tests/Fixtures/dark-theme.svg");
        
        theme.loadFromFile(themeFile);
        
        expectEquals(theme.getPrimaryColor().toString(), "ff1a1a1a");
    }
};
```

### Integration Tests

**Purpose**: Validate interactions between components.

**Characteristics**:
- Tests 2–3 components together
- May use real JUCE objects (AudioBuffer, MidiBuffer)
- Moderate speed (<500ms per test)

**Location**: `Tests/Integration/`

**Example**:
```cpp
class MidiFlowIntegrationTests : public juce::UnitTest
{
public:
    MidiFlowIntegrationTests() : juce::UnitTest("MIDI Flow", "MIDI") {}
    
    void runTest() override
    {
        beginTest("MIDI input should update UI via device listener");
        
        // Real components, mocked hardware
        MockMidiAdapter mockDevice;
        DeviceListener listener;
        MidiDeviceComponent uiComponent;
        
        listener.addDevice(&mockDevice);
        listener.addListener(&uiComponent);
        
        // Send message
        mockDevice.sendMessage(juce::MidiMessage::noteOn(1, 60, 0.8f));
        
        // Verify flow: device → listener → UI
        expect(uiComponent.isChannelActive(1));
    }
};
```

### System Tests

**Purpose**: Validate end-to-end workflows.

**Characteristics**:
- Tests full application or plugin lifecycle
- May spawn processes (standalone app launch)
- Slow (<5s per test)

**Location**: `Tests/System/`

**Example**:
```cpp
class StandaloneAppLaunchTests : public juce::UnitTest
{
public:
    StandaloneAppLaunchTests() : juce::UnitTest("App Launch", "System") {}
    
    void runTest() override
    {
        beginTest("Standalone app should launch without crashes");
        
        // Simulate app initialization
        ShowMidiApplication app;
        app.initialise("--headless");
        
        expect(app.isInitialised());
        
        app.shutdown();
    }
};
```

---

## Naming Conventions

### Test Class Names

Pattern: `<ComponentName>Tests` (plural)

Examples:
- `MidiDeviceComponentTests`
- `ThemeParsingTests`
- `SettingsManagerTests`

### Test Method/Case Names

Pattern: `should <expected behavior> when <condition>`

Examples:
- "should display Note On message when MIDI input received"
- "should hide inactive channels after timeout"
- "should load custom theme when file dropped"

**Bad Examples** (avoid):
- ❌ "test1", "testMidi" (not descriptive)
- ❌ "checkIfNoteDisplays" (not behavior-focused)

### JUCE UnitTest Categories

Use domain-aligned categories for filtering:

- `"MIDI"`: MIDI message handling, device management
- `"UI"`: User interface components, themes, layout
- `"Settings"`: Preferences, persistence, configuration
- `"System"`: End-to-end, cross-component workflows
- `"Performance"`: Latency, throughput, memory benchmarks

**Usage**:
```cpp
MidiDeviceComponentTests() : juce::UnitTest("MIDI Device Component", "MIDI") {}
```

**Run specific category**:
```bash
./build/ShowMIDI_Tests --category MIDI
```

---

## Review Expectations

### Pull Request Requirements

Every PR containing code changes MUST include:

1. **At least one test** (unit, integration, or system)
   - **Exception**: Documentation-only changes
2. **All tests passing** on local machine before pushing
3. **CI passing** on all platforms (macOS, Windows, Linux)
4. **Test descriptions** in PR body explaining what's tested

**Template**:
```markdown
## Tests Added

- **Unit**: `MidiChannelStateTests::shouldResetChannelWhenClearPressed`
- **Integration**: `MidiFlowTests::shouldUpdateUIOnNoteOff`

## Test Coverage

- Baseline coverage: 45% → 52% (+7%)
- New code coverage: 85%
```

### Code Review Checklist

Reviewers should verify:

- ✅ Tests follow Red-Green-Refactor (check commit history if possible)
- ✅ Test names describe behavior, not implementation
- ✅ Assertions use `expect()` or `expectEquals()` (not raw if statements)
- ✅ Fixtures are reusable and deterministic
- ✅ No hardcoded paths (use `juce::File::getCurrentWorkingDirectory()`)
- ✅ Platform-conditional code has platform-conditional tests

---

## Definition of Done

A task is **done** when:

1. ✅ All acceptance criteria from spec/user story satisfied
2. ✅ Tests written **before** implementation (Red-Green-Refactor)
3. ✅ All new code covered by tests (aim for 80%+ coverage)
4. ✅ All tests passing locally
5. ✅ CI passing on all platforms
6. ✅ Code reviewed and approved by at least one maintainer
7. ✅ GPL-3.0 headers present on new files
8. ✅ Conventional Commit message (e.g., `feat(midi): add channel auto-hide`)

### Allowances for Legacy Code

During the adoption phase (Weeks 1–8):

- **Touching legacy code**: If you modify existing untested code, add tests for **your changes only** (not required to retrofit entire file)
- **Refactoring legacy code**: Add baseline tests **before** refactoring to prevent regressions
- **Hard gate timeline**: From Week 3, new test failures block merges; from Week 8, all new code requires 80% coverage

---

## Quick Reference

### Run Tests Locally

```bash
# Build tests
cmake -B build -DBUILD_TESTS=ON
cmake --build build --target ShowMIDI_Tests

# Run all tests
./build/ShowMIDI_Tests

# Run specific category
./build/ShowMIDI_Tests --category MIDI

# Run with verbose output
./build/ShowMIDI_Tests --verbose
```

### Common Assertions (JUCE UnitTest)

```cpp
expect(condition, "optional failure message");
expectEquals(actual, expected, "optional message");
expectGreaterThan(value, threshold);
expectLessThan(value, threshold);
expectWithinAbsoluteError(value, expected, tolerance);
```

### Test Fixture Template

```cpp
struct MyComponentFixture
{
    MyComponent component;
    MockDependency mockDep;
    
    MyComponentFixture()
    {
        component.setDependency(&mockDep);
        component.initialise();
    }
    
    ~MyComponentFixture()
    {
        component.shutdown();
    }
};

// Usage in test:
MyComponentFixture fixture;
fixture.component.doSomething();
expect(fixture.mockDep.wasCalled());
```

---

## Questions?

- **Forum**: https://forum.uwyn.com
- **Discord**: https://discord.gg/TgsCnwqWTf
- **GitHub Discussions**: ShowMIDI repository

---

**Version History**:
- 1.0.0 (2025-11-11): Initial protocol for TDD adoption
