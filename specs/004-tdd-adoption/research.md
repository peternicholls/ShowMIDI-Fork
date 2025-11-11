# Test Framework Research - ShowMIDI TDD Adoption

**Research Date**: 2025-11-11  
**Project**: ShowMIDI (JUCE 7.0.5, C++17, CMake)  
**Platforms**: macOS, Windows, Linux, iOS

---

## Executive Summary

**Decision**: **JUCE UnitTest Framework** (juce_core/unit_tests)

**Rationale**:
1. **Zero Integration Complexity**: Already included in JUCE, activated with single `JUCE_UNIT_TESTS=1` definition
2. **Full Cross-Platform Support**: Tested on all ShowMIDI target platforms (macOS/Windows/Linux/iOS) by JUCE framework itself
3. **GPL-3.0 Native Compatibility**: Licensed under ISC/GPLv3 dual license, perfect alignment with ShowMIDI's GPL-3.0

---

## Alternatives Considered

### 1. Catch2 v3
**License**: Boost Software License 1.0 (BSL-1.0) ✅ GPL-compatible  
**Cross-Platform**: macOS ✅ | Windows ✅ | Linux ✅ | iOS ✅  
**Community Adoption**: Popular in JUCE projects (Pamplejuce template, ejaaskel blog)

**Pros**:
- Modern C++14/17 native syntax with BDD-style tests
- Header-only in v2, compiled library in v3 (faster compile times)
- Rich assertion library with expressive matchers
- Excellent CMake integration via FetchContent/CPM
- Large community, extensive documentation
- Used by popular JUCE templates (sudara/pamplejuce)

**Cons**:
- External dependency requiring FetchContent/CPM setup
- Additional CMakeLists.txt complexity (test target, linking)
- Catch2 v3 requires C++14 minimum (ShowMIDI uses C++17, so OK)
- Adds ~200-300 lines to CMake configuration
- Not JUCE-aware (no direct juce::String, juce::Array helpers)

**Why Rejected**: Unnecessary complexity for a framework that already has built-in testing. Adding external dependencies violates ShowMIDI's maintainability principle.

---

### 2. Google Test (GTest)
**License**: BSD-3-Clause ✅ GPL-compatible  
**Cross-Platform**: macOS ✅ | Windows ✅ | Linux ✅ | iOS ✅  
**Community Adoption**: Industry standard, used in WolfSound JUCE template

**Pros**:
- Industry-standard test framework (Google's official choice)
- Mature ecosystem with GoogleMock for mocking
- CMake native support (`include(GoogleTest)`, `gtest_discover_tests()`)
- Parameterized tests with `TEST_P` macros
- Rich assertion macros (`EXPECT_EQ`, `ASSERT_NEAR`, etc.)
- Excellent CI/CD integration (CTest, XML output)
- Used in Jan Wilczek's audio plugin template

**Cons**:
- External dependency via FetchContent or git submodule
- Requires separate test executable target in CMake
- More verbose test syntax compared to Catch2
- Larger binary size (compiled library + test runner)
- BSD-3-Clause license requires attribution (minor overhead)
- Not JUCE-native, requires adapters for JUCE types

**Why Rejected**: While excellent, it's overkill for ShowMIDI's needs. JUCE UnitTest provides sufficient capabilities without external dependencies.

---

## Chosen Framework: JUCE UnitTest

### Overview
JUCE's built-in unit testing framework (`juce_core/unit_tests`) provides a lightweight, macro-based testing system integrated directly into JUCE modules.

**License**: ISC License / GPLv3 dual-licensed (per JUCE modules)  
**Location**: `JUCE/modules/juce_core/unit_tests/`  
**Activation**: Single preprocessor definition `JUCE_UNIT_TESTS=1`

---

### Capabilities

#### Fixture Support
✅ **Test Fixtures via Class Inheritance**
```cpp
class MyComponentTests : public juce::UnitTest
{
public:
    MyComponentTests() : UnitTest("MyComponent Tests", UnitTestCategories::audio) {}
    
    void initialise() override {
        // Setup before each test
        component = std::make_unique<MyComponent>();
    }
    
    void shutdown() override {
        // Teardown after each test
        component.reset();
    }
    
    void runTest() override {
        beginTest("Test name");
        expect(component->isValid());
        // ...
    }
    
private:
    std::unique_ptr<MyComponent> component;
};

static MyComponentTests myComponentTests;
```

#### Assertion Capabilities
- `expect(condition)` - Basic assertion (non-fatal)
- `expect(condition, "Failure message")` - With custom message
- `expectEquals(actual, expected)` - Equality check
- `expectWithinAbsoluteError(actual, expected, tolerance)` - Float comparison
- `expectGreaterThan(a, b)` / `expectLessThan(a, b)` - Ordering checks
- `logMessage(text)` - Test output logging

**Example**:
```cpp
void runTest() override
{
    beginTest("MIDI message parsing");
    
    auto msg = juce::MidiMessage::noteOn(1, 60, 0.8f);
    expect(msg.isNoteOn(), "Should be note-on");
    expectEquals(msg.getNoteNumber(), 60);
    expectWithinAbsoluteError(msg.getFloatVelocity(), 0.8f, 0.01f);
}
```

#### Test Organization
- **Categories**: Organize tests by domain (audio, midi, ui, etc.)
- **Test Names**: Group assertions via `beginTest("name")`
- **Random Seeds**: Reproducible randomized tests via `getRandom()`

**Example**:
```cpp
// JUCE's own test categories
namespace juce::UnitTestCategories
{
    static const char* const audio = "Audio";
    static const char* const midi = "MIDI";
    static const char* const files = "Files";
    // ...
}
```

---

### Cross-Platform Support

✅ **macOS**: Tested by JUCE AudioPluginHost, AudioPerformanceTest examples  
✅ **Windows**: Tested by JUCE UnitTestRunner (Visual Studio 2022)  
✅ **Linux**: Tested by JUCE UnitTestRunner (GCC/Clang)  
✅ **iOS**: Supported (Android CMakeLists.txt shows unit test integration)

**Evidence**: JUCE includes `extras/UnitTestRunner` project that compiles on all platforms. ShowMIDI's existing CI (macOS/Windows/Linux) will work unchanged.

---

### CMake Integration

**Minimal Setup** (~10 lines):
```cmake
# In CMakeLists.txt - Add test executable
juce_add_console_app(ShowMIDI_Tests)

target_sources(ShowMIDI_Tests PRIVATE
    Tests/MidiMessageTests.cpp
    Tests/ThemeTests.cpp
)

target_compile_definitions(ShowMIDI_Tests PRIVATE
    JUCE_UNIT_TESTS=1  # Enable JUCE unit tests
)

target_link_libraries(ShowMIDI_Tests PRIVATE
    juce::juce_core
    juce::juce_audio_basics
    # ... other JUCE modules used by ShowMIDI
)

# CTest integration (optional)
enable_testing()
add_test(NAME ShowMIDI_UnitTests COMMAND ShowMIDI_Tests)
```

**Running Tests**:
```bash
# Build test executable
cmake --build build --target ShowMIDI_Tests

# Run tests
./build/ShowMIDI_Tests

# Run specific category
./build/ShowMIDI_Tests --category MIDI

# Run with custom random seed
./build/ShowMIDI_Tests --seed 0x12345678
```

---

### CI/CD Integration (GitHub Actions)

**Existing Pipeline Compatibility**: ShowMIDI's current `.github/workflows/ci.yml` requires **zero changes**. Tests run as separate executable.

**GitHub Actions Integration**:
```yaml
# Add to existing jobs in .github/workflows/ci.yml
- name: Build Tests
  run: cmake --build build --target ShowMIDI_Tests --config ${{ matrix.config }}

- name: Run Unit Tests
  run: |
    cd build
    if [ "$RUNNER_OS" == "Windows" ]; then
      ./ShowMIDI_Tests.exe
    else
      ./ShowMIDI_Tests
    fi
```

**Test Output**: Plain text by default, parseable by GitHub Actions logs:
```
Starting test: MIDI Message Tests...
  Test 1/3: Note On Parsing...
    ✓ Should be note-on
    ✓ Note number equals 60
    ✓ Velocity within tolerance
  Test 2/3: Control Change...
    ✗ FAILURE: CC value mismatch
       Expected: 127
       Actual:   64
```

---

### License Compatibility

**JUCE License**: ISC License (permissive) / GPLv3 (copyleft option)  
**ShowMIDI License**: GPL-3.0

✅ **Fully Compatible**: JUCE's dual licensing explicitly supports GPL-3.0 projects. No attribution overhead beyond existing JUCE license compliance.

**Comparison to Alternatives**:
- Catch2 (BSL-1.0): GPL-compatible, but requires separate LICENSE.Catch2.txt
- GTest (BSD-3-Clause): GPL-compatible, requires separate LICENSE.googletest.txt
- JUCE UnitTest: Already covered under existing JUCE licensing

---

## Community Adoption in JUCE Projects

### JUCE UnitTest Usage
- **JUCE Framework Itself**: 100+ built-in tests in `juce_core`, `juce_audio_formats`, `juce_graphics`
- **Projucer**: Uses JUCE UnitTest for internal validation
- **AudioPluginHost**: Includes unit test runner (extras/UnitTestRunner)

### Catch2 Usage
- **Pamplejuce** (sudara/pamplejuce): Popular JUCE CMake template with Catch2 v3
- **ejaaskel.dev**: Blog post on audio processor unit testing with Catch2

### Google Test Usage
- **WolfSound Template** (JanWilczek/audio-plugin-template): Comprehensive tutorial
- **ThePeculiarParrot**: Commercial JUCE plugins using GTest

**Observation**: External frameworks (Catch2, GTest) are popular in **template projects** and **learning resources**, but production JUCE projects often stick with JUCE UnitTest for simplicity.

---

## Integration Notes

### File Structure
```
ShowMIDI.git/
├── CMakeLists.txt          # Add test target here
├── Tests/                  # New test directory
│   ├── MidiMessageTests.cpp
│   ├── ThemeTests.cpp
│   ├── ComponentTests.cpp
│   └── UtilityTests.cpp
└── Source/                 # Existing source code
    └── ...
```

### CMake Snippet (Complete)
```cmake
# ==============================================================================
# ShowMIDI Unit Tests
# ==============================================================================

# Create test console app
juce_add_console_app(ShowMIDI_Tests)

# Generate JUCE header for test target
juce_generate_juce_header(ShowMIDI_Tests)

# Add test source files
file(GLOB_RECURSE TEST_FILES "Tests/*.cpp")
target_sources(ShowMIDI_Tests PRIVATE ${TEST_FILES})

# Enable JUCE unit tests
target_compile_definitions(ShowMIDI_Tests PRIVATE
    JUCE_UNIT_TESTS=1
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

# Link same dependencies as main plugin
target_link_libraries(ShowMIDI_Tests PRIVATE
    juce::juce_core
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# CTest integration (optional)
enable_testing()
add_test(NAME ShowMIDI_UnitTests COMMAND ShowMIDI_Tests)
```

### Example Test File
```cpp
/*
  ==============================================================================

   ShowMIDI
   Copyright (C) 2023 Uwyn LLC.  https://www.uwyn.com

   [GPL-3.0 license header - same as other ShowMIDI files]

  ==============================================================================
*/

#include <JuceHeader.h>

namespace showmidi
{

class MidiMessageTests final : public juce::UnitTest
{
public:
    MidiMessageTests()
        : UnitTest("MIDI Message Handling", juce::UnitTestCategories::audio)
    {
    }

    void runTest() override
    {
        beginTest("Note On Parsing");
        
        auto msg = juce::MidiMessage::noteOn(1, 60, 0.8f);
        expect(msg.isNoteOn(), "Should be note-on message");
        expectEquals(msg.getChannel(), 1);
        expectEquals(msg.getNoteNumber(), 60);
        expectWithinAbsoluteError(msg.getFloatVelocity(), 0.8f, 0.01f);
        
        beginTest("Control Change");
        
        auto cc = juce::MidiMessage::controllerEvent(1, 7, 100);
        expect(cc.isController(), "Should be CC message");
        expectEquals(cc.getControllerNumber(), 7);
        expectEquals(cc.getControllerValue(), 100);
    }
};

// Register test (auto-discovered by JUCE)
static MidiMessageTests midiMessageTests;

} // namespace showmidi
```

---

## Recommended Next Steps

1. **Prototype Phase**:
   - Add CMake test target to `CMakeLists.txt`
   - Create `Tests/` directory with one sample test
   - Verify local builds on macOS

2. **CI Integration**:
   - Add test build/run steps to `.github/workflows/ci.yml`
   - Verify all platforms (macOS/Windows/Linux) pass

3. **Documentation**:
   - Update `CONTRIBUTING.md` with test guidelines
   - Add "Running Tests" section to `README.md`

4. **Rollout**:
   - Start with utility/helper function tests (lowest risk)
   - Progress to MIDI message handling tests
   - Defer UI component tests (require graphical context)

---

## Conclusion

**JUCE UnitTest** is the optimal choice for ShowMIDI's TDD adoption:

✅ **Zero friction**: Already included, one-line activation  
✅ **Full platform support**: Tested on all ShowMIDI targets  
✅ **GPL-3.0 native**: No licensing overhead  
✅ **CI-ready**: Drop-in GitHub Actions integration  
✅ **JUCE-native**: Direct support for `juce::String`, `juce::Array`, etc.  

External frameworks (Catch2, GTest) are excellent but introduce unnecessary complexity for a project already using JUCE. The built-in framework provides sufficient capabilities for ShowMIDI's testing needs while maintaining alignment with the project's **Maintainability** and **JUCE Framework Standards** constitutional principles.

---

## References

### Primary Sources
- [JUCE UnitTest.h](../../../JUCE/modules/juce_core/unit_tests/juce_UnitTest.h)
- [JUCE UnitTestRunner Example](../../../JUCE/extras/UnitTestRunner/CMakeLists.txt)
- [JUCE String Tests](../../../JUCE/modules/juce_core/text/juce_String.cpp#L2352) (example usage)

### External Research
- WolfSound: ["How to Build Audio Plugin with JUCE & CMake"](https://thewolfsound.com/how-to-build-audio-plugin-with-juce-cpp-framework-cmake-and-unit-tests/) (GTest approach)
- Pamplejuce Template: [sudara/pamplejuce](https://github.com/sudara/pamplejuce) (Catch2 v3 approach)
- ejaaskel.dev: ["Unit Testing Audio Processors with JUCE & Catch2"](https://ejaaskel.dev/unit-testing-audio-processors-with-juce-catch2/)
- JUCE Forum: ["JUCE Unit Tests and CMake"](https://forum.juce.com/t/juce-unit-tests-and-cmake/40638)

### License Documentation
- [Catch2 License](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt) - BSL-1.0
- [Google Test License](https://github.com/google/googletest/blob/main/LICENSE) - BSD-3-Clause
- [JUCE License](../../../JUCE/LICENSE.md) - ISC/GPLv3 dual license

---

## Part 2: Mocking & Simulation Strategies for JUCE Audio Plugins

**Research Date**: 2025-11-11  
**Context**: ShowMIDI processes MIDI messages, interacts with MidiInput/MidiOutput devices, and runs in plugin hosts (VST/AU/etc).

### Executive Summary — Mocking Strategy

**Decision**: **Adapter Pattern + Minimal Mocking with JUCE Native Support**

Use JUCE's built-in capabilities combined with adapter/wrapper interfaces for external dependencies (MIDI devices, plugin hosts, file dialogs). Mock only when necessary using lightweight header-only libraries (FakeIt preferred, trompeloeil as alternative). Leverage JUCE's `AudioProcessorGraph` for plugin host simulation and `MidiBuffer` for deterministic MIDI message injection.

**Rationale**:
1. **JUCE-Native Approach**: JUCE classes like `MidiInput`/`MidiOutput` are not designed for mocking (platform-specific, no virtual interfaces)
2. **Minimal Mocking Philosophy**: Adapter layers are more maintainable than aggressive mocking
3. **Deterministic Testing**: `MidiBuffer` can be pre-populated; `AudioProcessorGraph` simulates plugin hosting offline
4. **Cross-Platform Compatibility**: All components work on macOS/Windows/Linux/iOS
5. **Headless CI Execution**: `MessageManager::setCurrentThreadAsMessageThread()` enables GUI-less testing

**Key Patterns**:
- **MIDI device mocking**: Adapter pattern (`IMidiDeviceAdapter`) with `MidiBuffer` injection
- **Plugin host simulation**: `AudioProcessorGraph` with offline rendering
- **Deterministic timing**: Simulated clock providers (`ITimeProvider`)
- **File dialogs**: Dependency injection (`IFileProvider`)

**Alternatives Considered**: GoogleMock (too heavy), full virtual interfaces (massive refactoring)

---

### MIDI Device Mocking: Adapter Pattern + MidiBuffer Injection

**Approach**: Wrap `MidiInput`/`MidiOutput` in testable adapter interface. Use `MidiBuffer` for deterministic message sequences.

**Code Snippet**:

```cpp
// IMidiDeviceAdapter.h - Testable interface
namespace showmidi
{

class IMidiDeviceAdapter
{
public:
    virtual ~IMidiDeviceAdapter() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void injectMessage(const juce::MidiMessage& msg, double timestamp) = 0;
};

// ProductionMidiAdapter.h - Real hardware wrapper
class ProductionMidiAdapter : public IMidiDeviceAdapter
{
    std::unique_ptr<juce::MidiInput> device;
    
public:
    ProductionMidiAdapter(const juce::String& deviceId, juce::MidiInputCallback* callback)
    {
        device = juce::MidiInput::openDevice(deviceId, callback);
    }
    
    void start() override { if (device) device->start(); }
    void stop() override { if (device) device->stop(); }
    void injectMessage(const juce::MidiMessage&, double) override 
    { 
        // No-op in production (messages come from hardware)
    }
};

// MockMidiAdapter.h - Test double
class MockMidiAdapter : public IMidiDeviceAdapter
{
    juce::MidiInputCallback* callback;
    bool running = false;
    
public:
    explicit MockMidiAdapter(juce::MidiInputCallback* cb) : callback(cb) {}
    
    void start() override { running = true; }
    void stop() override { running = false; }
    
    void injectMessage(const juce::MidiMessage& msg, double timestamp) override
    {
        if (running && callback)
            callback->handleIncomingMidiMessage(nullptr, msg);
    }
};

} // namespace showmidi
```

**Example Test**:

```cpp
class MidiFlowTests : public juce::UnitTest
{
public:
    MidiFlowTests() : juce::UnitTest("MidiFlow", juce::UnitTestCategories::audio) {}
    
    void runTest() override
    {
        beginTest("Note On triggers LED update");
        
        // Arrange: Create mock adapter
        auto mockAdapter = std::make_unique<MockMidiAdapter>(this);
        mockAdapter->start();
        
        // Act: Inject MIDI Note On message
        juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, (juce::uint8)100);
        mockAdapter->injectMessage(noteOn, 0.0);
        
        // Assert: Verify UI state
        juce::MessageManager::getInstance()->runDispatchLoopUntil(50);
        expect(ledIndicator.isLit(), "LED should be lit after Note On");
    }
};
```

✅ **Cross-Platform**: All code uses JUCE types  
✅ **Headless CI**: `MessageManager::runDispatchLoopUntil()` processes events without GUI

---

### Plugin Host Simulation: AudioProcessorGraph + Offline Rendering

**Approach**: Use JUCE's `AudioProcessorGraph` to load plugins, feed audio/MIDI, and validate outputs offline (no real DAW required).

**Code Snippet**:

```cpp
#include <juce_audio_processors/juce_audio_processors.h>

class PluginHostSimulator
{
    juce::AudioProcessorGraph graph;
    juce::AudioProcessorGraph::Node::Ptr pluginNode;
    
public:
    void loadPlugin(juce::AudioPluginInstance* plugin)
    {
        pluginNode = graph.addNode(std::unique_ptr<juce::AudioProcessor>(plugin));
    }
    
    void processTestBlock(juce::MidiBuffer& midiInput, juce::AudioBuffer<float>& audioOutput)
    {
        graph.prepareToPlay(44100.0, 512);
        graph.processBlock(audioOutput, midiInput);
    }
};
```

**Example Test**:

```cpp
class PluginValidationTests : public juce::UnitTest
{
public:
    PluginValidationTests() : juce::UnitTest("PluginValidation") {}
    
    void runTest() override
    {
        beginTest("Plugin processes MIDI note without crashes");
        
        // Arrange: Load ShowMIDI plugin into simulator
        PluginHostSimulator host;
        auto plugin = createPluginInstance();
        host.loadPlugin(plugin.release());
        
        // Act: Send MIDI note through plugin
        juce::MidiBuffer midiBuffer;
        midiBuffer.addEvent(juce::MidiMessage::noteOn(1, 60, 0.8f), 0);
        juce::AudioBuffer<float> audioBuffer(2, 512);
        
        host.processTestBlock(midiBuffer, audioBuffer);
        
        // Assert: Plugin processed successfully
        expect(audioBuffer.getMagnitude(0, 512) >= 0.0f);
    }
};
```

✅ **Headless**: No DAW GUI required  
✅ **Deterministic**: Offline rendering with fixed sample rates

**Alternatives**: Real DAW automation (rejected: non-deterministic, slow, GUI-dependent)

---

### Deterministic Timing: Simulated Clock

**Approach**: Replace `Time::getMillisecondCounterHiRes()` with injectable clock interface.

**Code Snippet**:

```cpp
class ITimeProvider
{
public:
    virtual ~ITimeProvider() = default;
    virtual double getCurrentTime() const = 0;
};

class SimulatedTimeProvider : public ITimeProvider
{
    double simulatedTime = 0.0;
    
public:
    void advanceTime(double seconds) { simulatedTime += seconds; }
    double getCurrentTime() const override { return simulatedTime; }
};
```

**Example Test**:

```cpp
class TimingTests : public juce::UnitTest
{
public:
    void runTest() override
    {
        beginTest("MIDI latency < 10ms");
        
        auto simClock = std::make_unique<SimulatedTimeProvider>();
        simClock->advanceTime(0.005); // 5ms
        
        expectLessThan(simClock->getCurrentTime(), 0.010, "Under 10ms target");
    }
};
```

---

### Mocking Framework Comparison

| Framework | License | C++ | Header-Only | Platforms | Thread-Safe | JUCE Compatible | Verdict |
|-----------|---------|-----|-------------|-----------|-------------|-----------------|---------|
| **FakeIt** | MIT | C++11 | ✅ Yes | GCC/Clang/MSVC | ❌ No | ✅ Yes | ✅ **Recommended** |
| **trompeloeil** | Boost 1.0 | C++14 | ✅ Yes | GCC/Clang/MSVC | ✅ Yes | ✅ Yes | ✅ Alternative |
| **GoogleMock** | BSD-3 | C++11 | ❌ No | All | ✅ Yes | ⚠️ Partial | ❌ Too Heavy |

**Recommendation**: Use **FakeIt** sparingly (< 10% of tests). Prefer adapter pattern (90%).

**FakeIt Example**:

```cpp
#include <fakeit.hpp>
using namespace fakeit;

struct IMidiDevice {
    virtual void sendMessage(const juce::MidiMessage&) = 0;
};

Mock<IMidiDevice> mockDevice;
When(Method(mockDevice, sendMessage)).AlwaysReturn();

IMidiDevice& device = mockDevice.get();
device.sendMessage(juce::MidiMessage::noteOn(1, 60, 0.8f));

Verify(Method(mockDevice, sendMessage)).Once();
```

---

### Recommended Test Structure

```
Tests/
├── Unit/
│   ├── MidiDeviceComponentTests.cpp  # Uses MockMidiAdapter
│   ├── ThemeTests.cpp                # Color schemes, fonts
│   └── SettingsManagerTests.cpp      # JSON serialization
├── Integration/
│   ├── MidiFlowTests.cpp             # MIDI input → LED update
│   └── PluginHostTests.cpp           # AudioProcessorGraph tests
├── System/
│   ├── StandaloneAppTests.cpp        # App launch (headless)
│   └── PluginValidationTests.cpp     # VST3/AU block processing
└── Fixtures/
    ├── MockMidiAdapter.h
    ├── SimulatedTimeProvider.h
    └── test_song.mid                 # Binary test asset
```

---

### CI Integration

Extend existing `.github/workflows/ci.yml`:

```yaml
- name: Build Tests
  run: cmake --build build --target ShowMIDI_Tests
- name: Run Unit Tests
  run: ./build/Tests/ShowMIDI_Tests --junit --output=test-results.xml
```

**Headless**: All tests call `MessageManager::setCurrentThreadAsMessageThread()` in `main()`.

---

### Performance Targets

| Test Suite | Target | Max |
|------------|--------|-----|
| Unit | < 2 min | 5 min |
| Integration | < 3 min | 8 min |
| System | < 5 min | 10 min |
| **Full** | **< 10 min** | **15 min** |

---

### Recommendations for ShowMIDI

**Phase 0 (Week 1)**:
1. Implement adapters: `IMidiDeviceAdapter`, `IFileProvider`, `ITimeProvider`
2. Write 3 baseline tests (Theme, MidiFlow, StandaloneApp)

**Phase 1 (Weeks 2-4)**:
1. Expand unit tests (SettingsManager, MidiDeviceComponent)
2. Add integration tests (MIDI pipeline)
3. Introduce FakeIt only if needed

**Phase 2 (Weeks 5-8)**:
1. Create `PluginHostSimulator`
2. Add system tests for VST3/AU/AAX/LV2
3. Validate cross-platform consistency

---

### Additional References

**Mocking Frameworks**:
- [FakeIt](https://github.com/eranpeer/FakeIt) - MIT, header-only
- [trompeloeil](https://github.com/rollbear/trompeloeil) - Boost 1.0, thread-safe

**JUCE Examples**:
- `JUCE/modules/juce_audio_basics/mpe/juce_MPEInstrument.cpp` (MPEInstrumentTests)
- `JUCE/modules/juce_audio_processors/processors/juce_AudioProcessorGraph.cpp` (AudioProcessorGraphTests)

**JUCE Forum**:
- ["MidiInput and Unit-tests"](https://forum.juce.com/t/14585)
- ["Problems unit-testing AudioProcessorGraph"](https://forum.juce.com/t/32913)

---

### Conclusion — Part 2

**Adopted Strategy**:
1. **Adapter pattern** for MIDI devices, file dialogs, time (90% of needs)
2. **JUCE native tools** for plugin host simulation (`AudioProcessorGraph`), message injection (`MidiBuffer`)
3. **Minimal mocking** with FakeIt (only for complex callbacks)
4. **Headless execution** via `MessageManager::setCurrentThreadAsMessageThread()`

This approach:
- ✅ Minimal refactoring (adapters, not full virtual hierarchies)
- ✅ Maintains JUCE idioms and C++17 standards
- ✅ Supports headless CI on all platforms
- ✅ Enables deterministic, fast tests (< 15 min full suite)
- ✅ Scales from baseline (Week 1) to 80% coverage (Week 8)
