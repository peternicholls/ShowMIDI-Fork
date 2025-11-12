# ShowMIDI Test Suite

**Framework**: JUCE UnitTest (built into JUCE core)  
**Feature**: 004-tdd-adoption  
**Documentation**: See [specs/004-tdd-adoption/quickstart.md](../specs/004-tdd-adoption/quickstart.md)

## Quick Start

### Build Tests

```bash
# From repository root
cmake -B build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target ShowMIDI_Tests
```

### Run Tests

```bash
# Run all tests
./build/ShowMIDI_Tests

# Run specific category
./build/ShowMIDI_Tests --category MIDI
./build/ShowMIDI_Tests --category UI

# Run with verbose output
./build/ShowMIDI_Tests --verbose
```

### Run via CTest

```bash
cd build
ctest --output-on-failure       # All tests
ctest -L unit                   # Unit tests only
ctest -L integration            # Integration tests only
ctest -L system                 # System tests only
ctest -j4                       # Parallel execution
```

## Directory Structure

```
Tests/
├── Unit/                   # Fast, isolated component tests
│   ├── ThemeTests.cpp
│   ├── SettingsManagerTests.cpp
│   └── MidiMessageParserTests.cpp
├── Integration/            # Multi-component interaction tests
│   ├── MidiFlowTests.cpp
│   ├── DeviceDiscoveryTests.cpp
│   └── ThemeApplicationTests.cpp
├── System/                 # End-to-end, full lifecycle tests
│   ├── StandaloneAppTests.cpp
│   ├── PluginVST3Tests.cpp
│   └── PluginAUTests.cpp
├── Fixtures/               # Shared test utilities and test assets
│   ├── MockMidiAdapter.{h,cpp}
│   ├── SimulatedTimeProvider.{h,cpp}
│   ├── dark-theme-test.svg
│   └── test-midi-sequence.mid
├── CMakeLists.txt          # Test build configuration
└── README.md               # This file
```

## Test Scopes

| Scope | Purpose | Speed | Location | Examples |
|-------|---------|-------|----------|----------|
| **Unit** | Validate individual components | <100ms | `Unit/` | Theme parsing, MIDI message validation |
| **Integration** | Test component interactions | <500ms | `Integration/` | MIDI flow, device discovery |
| **System** | End-to-end workflows | <5s | `System/` | App launch, plugin load/unload |

## Documentation

For comprehensive testing guidance, see:

- **Quickstart Guide**: [specs/004-tdd-adoption/quickstart.md](../specs/004-tdd-adoption/quickstart.md)
- **TDD Protocol**: [specs/004-tdd-adoption/contracts/test-protocol.md](../specs/004-tdd-adoption/contracts/test-protocol.md)
- **CI Matrix**: [specs/004-tdd-adoption/contracts/ci-matrix.yml](../specs/004-tdd-adoption/contracts/ci-matrix.yml)
- **Coverage Policy**: [specs/004-tdd-adoption/contracts/coverage-policy.md](../specs/004-tdd-adoption/contracts/coverage-policy.md)

## Writing Your First Test

See the [quickstart guide](../specs/004-tdd-adoption/quickstart.md#write-your-first-test) for a step-by-step tutorial.

## Questions?

- **Forum**: https://forum.uwyn.com
- **Discord**: https://discord.gg/TgsCnwqWTf
- **GitHub Discussions**: ShowMIDI repository
