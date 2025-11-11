# TDD Quickstart Guide

**Feature**: 004-tdd-adoption  
**Audience**: ShowMIDI contributors (new and existing)  
**Time to First Passing Test**: ~30 minutes

## Prerequisites

Before starting, ensure you have:

- ✅ ShowMIDI repository cloned with submodules initialized
- ✅ CMake 3.15+ installed
- ✅ Platform-specific toolchain:
  - **macOS**: Xcode with Command Line Tools
  - **Windows**: Visual Studio 2022
  - **Linux**: GCC/G++, ALSA, X11, Freetype dev libraries
- ✅ Git configured for ShowMIDI repository

## Quick Start (5 Minutes)

### 1. Build Test Suite

```bash
# From repository root
cmake -B build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target ShowMIDI_Tests
```

**Expected Output**:
```
[100%] Built target ShowMIDI_Tests
```

**If build fails**: See "Troubleshooting" section below.

### 2. Run Tests

```bash
./build/ShowMIDI_Tests
```

**Expected Output** (Week 1 baseline):
```
JUCE v7.0.5
Running tests from category: All

MIDI Device Component Tests
  ✓ should display Note On message when MIDI input received
  ✓ should hide inactive channels after timeout
  
Theme Parsing Tests
  ✓ should load dark theme from SVG file
  
2/2 tests passed
```

### 3. Write Your First Test

Create `Tests/Unit/MyFirstTest.cpp`:

```cpp
#include <JuceHeader.h>

class MyFirstTest : public juce::UnitTest
{
public:
    MyFirstTest() : juce::UnitTest("My First Test", "Tutorial") {}
    
    void runTest() override
    {
        beginTest("Addition should work correctly");
        
        int result = 2 + 2;
        expectEquals(result, 4, "2 + 2 should equal 4");
    }
};

static MyFirstTest myFirstTest;
```

### 4. Register and Run

Add to `Tests/CMakeLists.txt`:
```cmake
target_sources(ShowMIDI_Tests PRIVATE
    Unit/MyFirstTest.cpp
)
```

Rebuild and run:
```bash
cmake --build build --target ShowMIDI_Tests
./build/ShowMIDI_Tests --category Tutorial
```

**Expected Output**:
```
My First Test
  ✓ Addition should work correctly

1/1 tests passed
```

**🎉 Congratulations!** You've written and run your first test.

---

## Directory Structure

```
ShowMIDI.git/
├── Source/                     # Production code
│   ├── MidiDeviceComponent.{h,cpp}
│   └── ... (~50 files)
├── Tests/                      # NEW: Test infrastructure
│   ├── Unit/                   # Fast, isolated tests
│   │   ├── MidiDeviceComponentTests.cpp
│   │   └── ThemeTests.cpp
│   ├── Integration/            # Multi-component tests
│   │   └── MidiFlowTests.cpp
│   ├── System/                 # End-to-end tests
│   │   └── StandaloneAppTests.cpp
│   ├── Fixtures/               # Shared test utilities
│   │   ├── MockMidiAdapter.{h,cpp}
│   │   └── test-theme.svg
│   ├── CMakeLists.txt          # Test build configuration
│   └── README.md               # This file
├── CMakeLists.txt              # Root CMake (add BUILD_TESTS option)
└── specs/004-tdd-adoption/     # Planning docs
    ├── spec.md
    ├── plan.md
    ├── research.md
    ├── data-model.md
    ├── contracts/
    │   ├── test-protocol.md     # TDD workflow guide
    │   ├── ci-matrix.yml        # CI configuration
    │   └── coverage-policy.md   # Coverage targets
    └── quickstart.md            # This file
```

---

## Common Workflows

### Run Specific Test Categories

```bash
./build/ShowMIDI_Tests --category MIDI       # MIDI tests only
./build/ShowMIDI_Tests --category UI         # UI tests only
./build/ShowMIDI_Tests --category Tutorial   # Your test!
```

### Run Tests with Verbose Output

```bash
./build/ShowMIDI_Tests --verbose
```

**Output** includes:
- Detailed assertion messages
- Execution time per test
- Stack traces on failures

### Run Tests via CTest

```bash
cd build
ctest --output-on-failure          # Run all
ctest -L unit                      # Unit tests only
ctest -L fast                      # Fast tests only
ctest -j4                          # Parallel (4 jobs)
```

### Generate Coverage Report

**Linux/macOS**:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build
lcov --capture --directory build --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/JUCE/*' --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_html
open coverage_html/index.html  # macOS
xdg-open coverage_html/index.html  # Linux
```

**Windows** (requires OpenCppCoverage):
```powershell
cmake -B build -DBUILD_TESTS=ON
cmake --build build --config Debug
OpenCppCoverage.exe --sources Source --excluded_sources JUCE `
  --export_type html:coverage_html -- ctest --test-dir build -C Debug
start coverage_html/index.html
```

---

## Writing Tests: Templates

### Unit Test Template

```cpp
#include <JuceHeader.h>
#include "Source/YourComponent.h"
#include "Tests/Fixtures/MockDependency.h"

class YourComponentTests : public juce::UnitTest
{
public:
    YourComponentTests() : juce::UnitTest("Your Component Tests", "YourCategory") {}
    
    void runTest() override
    {
        beginTest("Component should do X when Y happens");
        
        // Arrange: Set up test fixture
        YourComponent component;
        MockDependency mockDep;
        component.setDependency(&mockDep);
        
        // Act: Perform action
        component.doSomething();
        
        // Assert: Verify outcome
        expect(mockDep.wasCalled(), "Dependency should be called");
        expectEquals(component.getState(), ExpectedState, "State should change");
    }
};

static YourComponentTests yourComponentTests;
```

### Integration Test Template

```cpp
#include <JuceHeader.h>
#include "Source/ComponentA.h"
#include "Source/ComponentB.h"

class ComponentInteractionTests : public juce::UnitTest
{
public:
    ComponentInteractionTests() : juce::UnitTest("Component Interaction", "Integration") {}
    
    void runTest() override
    {
        beginTest("ComponentA should notify ComponentB");
        
        ComponentA compA;
        ComponentB compB;
        compA.addListener(&compB);
        
        compA.triggerEvent();
        
        expect(compB.wasNotified(), "ComponentB should receive notification");
    }
};

static ComponentInteractionTests componentInteractionTests;
```

### Test Fixture Template

```cpp
// Tests/Fixtures/MyTestFixture.h
#pragma once
#include <JuceHeader.h>

struct MyTestFixture
{
    MyComponent component;
    MockDependency mockDep;
    juce::MidiBuffer testMidiBuffer;
    
    MyTestFixture()
    {
        component.setDependency(&mockDep);
        component.initialise();
        
        // Pre-populate test data
        testMidiBuffer.addEvent(juce::MidiMessage::noteOn(1, 60, 0.8f), 0);
    }
    
    ~MyTestFixture()
    {
        component.shutdown();
    }
};

// Usage:
MyTestFixture fixture;
fixture.component.processMidi(fixture.testMidiBuffer);
expect(fixture.mockDep.receivedNoteOn());
```

---

## Debugging Failing Tests

### View Failure Details

```bash
./build/ShowMIDI_Tests --verbose
```

**Output**:
```
MIDI Device Component Tests
  ✗ should display Note On message when MIDI input received
    Expected: 60
    Actual: 0
    File: Tests/Unit/MidiDeviceComponentTests.cpp:45
```

### Run Single Test

```bash
# Run specific test by name
./build/ShowMIDI_Tests --test-name "should display Note On message"
```

### Attach Debugger

**Xcode** (macOS):
```bash
cmake -B build -G Xcode -DBUILD_TESTS=ON
open build/ShowMIDI.xcodeproj
# Set breakpoint in test file
# Run scheme "ShowMIDI_Tests"
```

**Visual Studio** (Windows):
```bash
cmake -B build -G "Visual Studio 17 2022" -DBUILD_TESTS=ON
start build/ShowMIDI.sln
# Set breakpoint in test file
# Debug → Start Debugging (F5)
```

**GDB** (Linux):
```bash
gdb ./build/ShowMIDI_Tests
(gdb) break MyFirstTest.cpp:10
(gdb) run
```

---

## Troubleshooting

### Build Errors

**Error**: `JUCE_UNIT_TESTS not defined`
```
Fix: Ensure target_compile_definitions(ShowMIDI_Tests PRIVATE JUCE_UNIT_TESTS=1)
```

**Error**: `undefined reference to juce::UnitTest::UnitTest`
```
Fix: Link juce::juce_core in target_link_libraries(ShowMIDI_Tests PRIVATE juce::juce_core)
```

**Error**: `cannot open file 'JuceHeader.h'`
```
Fix: Verify JUCE submodule initialized: git submodule update --init --recursive
```

### Runtime Errors

**Error**: `Segmentation fault` when running tests
```
Fix: Check fixture cleanup (destructors called?), verify no dangling pointers
```

**Error**: Tests pass locally, fail in CI
```
Fix: Likely non-deterministic test (timing, file paths). Use SimulatedTime, absolute paths.
```

### Platform-Specific Issues

**macOS**: `dyld: Library not loaded`
```bash
# Rebuild with correct paths
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build --target ShowMIDI_Tests --clean-first
```

**Windows**: `The application was unable to start correctly (0xc000007b)`
```
Fix: Ensure Debug/Release config matches between CMake and VS build
cmake --build build --config Debug  # Match config!
```

**Linux**: `cannot connect to X server`
```bash
# Use headless message manager
export DISPLAY=:99
Xvfb :99 -screen 0 1024x768x24 &  # Virtual framebuffer
./build/ShowMIDI_Tests
```

---

## CI Integration

Tests run automatically on:
- ✅ Pull requests to `develop` or `main`
- ✅ Pushes to `main`, `release/**`, `hotfix/**`

**View Results**: GitHub Actions → "Test Results" tab

**Local CI Simulation**:
```bash
# Run same commands as CI
cmake -B build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Next Steps

After writing your first test:

1. **Read TDD Protocol**: `specs/004-tdd-adoption/contracts/test-protocol.md`
   - Learn Red-Green-Refactor workflow
   - Understand test scopes (Unit/Integration/System)
   - Follow naming conventions

2. **Explore Existing Tests**: `Tests/Unit/MidiDeviceComponentTests.cpp`
   - See fixture usage patterns
   - Study assertion examples
   - Observe JUCE UnitTest API

3. **Write a Real Test**: Pick a component from `Source/`
   - Identify a user-facing behavior
   - Write failing test first (Red)
   - Implement minimal code (Green)
   - Refactor for clarity

4. **Check Coverage**: Generate report, identify gaps
   - Target 80% coverage for new code
   - See `specs/004-tdd-adoption/contracts/coverage-policy.md`

---

## Resources

- **JUCE UnitTest Documentation**: https://docs.juce.com/master/classUnitTest.html
- **TDD Protocol**: `specs/004-tdd-adoption/contracts/test-protocol.md`
- **CI Matrix**: `specs/004-tdd-adoption/contracts/ci-matrix.yml`
- **Coverage Policy**: `specs/004-tdd-adoption/contracts/coverage-policy.md`
- **ShowMIDI Forum**: https://forum.uwyn.com
- **Discord**: https://discord.gg/TgsCnwqWTf

---

## Questions?

**Getting Started**: This quickstart guide  
**TDD Workflow**: `contracts/test-protocol.md`  
**CI Setup**: `contracts/ci-matrix.yml`  
**Coverage Targets**: `contracts/coverage-policy.md`

**Still stuck?** Open a GitHub Discussion or ask in Discord #development channel.

---

**Document Version**: 1.0.0 (2025-11-11)
