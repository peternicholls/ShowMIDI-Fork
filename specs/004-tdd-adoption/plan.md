# Implementation Plan: TDD Adoption

**Branch**: `004-tdd-adoption` | **Date**: 2025-11-11 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/004-tdd-adoption/spec.md`

## Summary

ShowMIDI is transitioning to Test-Driven Development to establish sustainable quality practices across all supported platforms (macOS, Windows, Linux, iOS). This plan adopts **JUCE UnitTest framework** (native to JUCE, zero external dependencies) for implementing a phased rollout covering unit, integration, and system tests with cross-platform CI execution. The implementation follows an 8-week rollout from baseline coverage (90% critical flows, Week 1) to hard gate enforcement (80% line coverage for new code, Week 8), with adapter patterns for MIDI device mocking and `AudioProcessorGraph` for plugin host simulation.

## Technical Context

**Language/Version**: C++17 (JUCE framework requirement)  
**Primary Dependencies**: 
- JUCE 7.0.5 (audio plugin framework, ISC/GPLv3 dual-licensed)
- JUCE UnitTest framework (`juce_core/unit_tests`, activated via `JUCE_UNIT_TESTS=1`)
- CMake 3.15+ (build system)
- Git submodules (JUCE, VST3 SDK)

**Storage**: 
- File-based (JSON for settings via `juce::PropertiesFile`)
- SVG themes loaded from filesystem
- Test fixtures stored in `Tests/Fixtures/` (SVG, MIDI files)

**Testing**: 
- **Framework**: JUCE UnitTest (built-in, zero external dependencies)
- **Mocking**: Adapter pattern (90% of needs) + FakeIt (header-only, MIT, for complex scenarios)
- **Plugin Simulation**: `juce::AudioProcessorGraph` for offline plugin testing
- **Deterministic MIDI**: `juce::MidiBuffer` injection via adapter interfaces

**Target Platforms**: 
- **Desktop**: macOS 11.7+, Windows 10+, Linux (Ubuntu/Fedora/Arch)
- **Mobile**: iOS 12+ (AUv3 plugin only, simulator for testing)
- **Plugin Formats**: VST, VST3, AU, AUv3, LV2, CLAP, Standalone

**Project Type**: Multi-platform audio plugin (desktop + mobile)

**Performance Goals**: 
- MIDI message processing latency <10ms (preferably <5ms)
- UI updates must not block MIDI thread
- Test suite full execution <15 minutes (unit <2min, integration <3min, system <5min)
- Cross-platform CI feedback <15 minutes

**Constraints**: 
- No allocations on MIDI/audio thread (real-time safety)
- Headless CI execution (GUI-less via `MessageManager::setCurrentThreadAsMessageThread()`)
- Deterministic tests (no flaky timing, hardware dependencies mocked)
- GPL-3.0 license compliance (all test files require license headers)

**Scale/Scope**: 
- ~50 source files in `Source/` directory (baseline)
- 10 critical user flows requiring 90% coverage by Week 1
- 80% overall line coverage target by Week 8
- 4 platform test jobs (macOS, Windows, Linux, iOS simulator)

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### Multi-Platform Architecture ✅ COMPLIANT

**Requirement**: Support macOS, Windows, Linux, iOS with feature parity.

**Compliance**:
- Test matrix includes all four platforms (see `contracts/ci-matrix.yml`)
- Platform-specific test labels (`ios`, `plugin-au`, `plugin-lv2`) ensure targeted validation
- JUCE UnitTest framework verified on all platforms (macOS, Windows, Linux, iOS simulator)
- Headless execution enables CI on all platforms without GUI dependencies

**Evidence**: 
- `research.md` confirms JUCE UnitTest cross-platform support
- `ci-matrix.yml` defines platform-specific runners and test execution strategies
- `data-model.md` Platform Matrix entity documents OS-specific configurations

---

### JUCE Framework Standards ✅ COMPLIANT

**Requirement**: Adhere to JUCE idioms, use JUCE types, implement Pimpl, leak detection.

**Compliance**:
- JUCE UnitTest is native framework (no STL testing alternatives)
- Test fixtures use `juce::MidiMessage`, `juce::MidiBuffer`, `juce::String`, `juce::File`
- Adapter pattern (e.g., `IMidiDeviceAdapter`) follows JUCE interface design
- All test files include GPL-3.0 headers (same as ShowMIDI source code)
- Tests use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` for test components

**Evidence**: 
- `research.md` demonstrates JUCE-native test examples with framework types
- `test-protocol.md` mandates GPL-3.0 headers on all test files
- `quickstart.md` templates use `juce::UnitTest` base class and JUCE assertions

---

### Real-Time Performance ✅ COMPLIANT

**Requirement**: MIDI latency <10ms, no UI blocking MIDI thread, memory pre-allocation.

**Compliance**:
- Tests validate latency via `SimulatedTimeProvider` (deterministic clock)
- Mock MIDI adapters inject messages without hardware overhead
- Test suite performance targets (<15 min total) prevent slow regression tests
- No test allocations on simulated MIDI thread (adapters use pre-allocated `MidiBuffer`)

**Evidence**: 
- `research.md` Part 2 documents `SimulatedTimeProvider` for deterministic timing tests
- `coverage-policy.md` defines performance-critical path exceptions (rare, case-by-case)
- Test suite duration targets documented in `ci-matrix.yml`

---

### User-Centric Design ✅ COMPLIANT

**Requirement**: Glanceable interface, auto-discovery, visual clarity, progressive disclosure.

**Compliance**:
- TDD protocol emphasizes behavior-focused test names (e.g., "should display Note On when MIDI received")
- Quickstart guide enables new contributor to write first test in <30 minutes (SC-006)
- Clear test output with descriptive failure messages (glanceable test results)
- Test fixtures provide auto-discovery patterns (mocks auto-register)

**Evidence**: 
- `test-protocol.md` defines user-facing behavior naming convention
- `quickstart.md` 5-minute quick start section for immediate value
- `spec.md` SC-006 validates 30-minute new contributor onboarding

---

### Maintainability ✅ COMPLIANT

**Requirement**: GPL-3.0 headers, component documentation, consistent naming, build hygiene.

**Compliance**:
- All test files require GPL-3.0 license headers (enforced in `test-protocol.md`)
- Test naming convention documented (`<ComponentName>Tests`, `should X when Y`)
- CMake configuration avoids hardcoded paths (`BUILD_TESTS` option, relative paths)
- Test structure mirrors source code organization (`Tests/Unit/`, `Tests/Integration/`, `Tests/System/`)
- Comprehensive documentation (`quickstart.md`, `test-protocol.md`, `coverage-policy.md`)

**Evidence**: 
- `test-protocol.md` Section: "Naming Conventions"
- `quickstart.md` templates include GPL-3.0 headers
- `coverage-policy.md` documents coverage tracking and dashboard

---

### Development Workflow ✅ COMPLIANT

**Requirement**: GitFlow branches, Conventional Commits, CI validation, quality gates.

**Compliance**:
- Feature developed on `004-tdd-adoption` branch (GitFlow)
- CI pipeline extends existing `.github/workflows/ci.yml` (no breaking changes)
- Phased rollout aligns with PR requirements (report-only → soft gate → hard gate)
- Test results uploaded as artifacts (90-day retention)
- Conventional Commits required for TDD workflow (`test(midi): add Note On test`)

**Evidence**: 
- Branch name follows `feature/<number>-<description>` pattern
- `ci-matrix.yml` defines CI trigger strategy (PRs to develop/main)
- `coverage-policy.md` enforcement timeline aligns with PR gating strategy

---

### Summary: NO VIOLATIONS

All constitution principles are satisfied. TDD adoption enhances maintainability and quality without compromising platform support, JUCE standards, performance, or user experience.

## Project Structure

### Documentation (this feature)

```text
specs/004-tdd-adoption/
├── spec.md              # Feature specification (user stories, requirements)
├── plan.md              # This file (implementation plan)
├── research.md          # Phase 0 output (test framework research, mocking strategies)
├── data-model.md        # Phase 1 output (test entities: TestSuite, TestCase, Fixture, etc.)
├── quickstart.md        # Phase 1 output (30-minute new contributor guide)
├── contracts/           # Phase 1 output (protocols and policies)
│   ├── test-protocol.md      # TDD workflow (Red-Green-Refactor, naming, review)
│   ├── ci-matrix.yml         # Cross-platform test execution strategy
│   └── coverage-policy.md    # Coverage targets, enforcement timeline, exceptions
└── checklists/          # Phase 2 output (task breakdown - NOT yet created)
    └── implementation-tasks.md
```

### Source Code (repository root)

```text
ShowMIDI.git/
├── Source/                      # Production code (~50 files)
│   ├── MidiDeviceComponent.{h,cpp}
│   ├── Theme.{h,cpp}
│   ├── SettingsManager.{h,cpp}
│   └── ... (~47 other files)
│
├── Tests/                       # NEW: Test infrastructure (to be created)
│   ├── Unit/                    # Fast, isolated component tests
│   │   ├── MidiDeviceComponentTests.cpp
│   │   ├── ThemeTests.cpp
│   │   ├── SettingsManagerTests.cpp
│   │   └── ... (one test file per Source/ component)
│   │
│   ├── Integration/             # Multi-component interaction tests
│   │   ├── MidiFlowTests.cpp          # MIDI input → listener → UI
│   │   ├── ThemeApplicationTests.cpp  # Theme load → UI update
│   │   └── DeviceDiscoveryTests.cpp   # Device appears → sidebar update
│   │
│   ├── System/                  # End-to-end, full lifecycle tests
│   │   ├── StandaloneAppTests.cpp     # App launch, quit, settings
│   │   ├── PluginVST3Tests.cpp        # VST3 host load, process, unload
│   │   ├── PluginAUTests.cpp          # Audio Unit (macOS only)
│   │   └── PluginLV2Tests.cpp         # LV2 (Linux only)
│   │
│   ├── Fixtures/                # Shared test utilities and test assets
│   │   ├── MockMidiAdapter.{h,cpp}    # IMidiDeviceAdapter test double
│   │   ├── SimulatedTimeProvider.{h,cpp}  # Deterministic clock
│   │   ├── TestThemeFile.h            # Helper to generate test SVG themes
│   │   ├── dark-theme-test.svg        # Binary test asset
│   │   └── test-midi-sequence.mid     # Binary MIDI file for playback tests
│   │
│   ├── CMakeLists.txt           # Test build configuration
│   └── README.md                # Test documentation (links to quickstart.md)
│
├── CMakeLists.txt               # Root CMake (add BUILD_TESTS option)
├── .github/
│   └── workflows/
│       └── ci.yml               # Existing CI (extend with test jobs)
│
└── JuceLibraryCode/             # JUCE-generated code (excluded from coverage)
```

**Structure Decision**: 

ShowMIDI follows a **multi-platform audio plugin architecture** with clear separation between production code (`Source/`) and test code (`Tests/`). Tests are organized by scope (Unit/Integration/System) to enable selective execution and parallel CI runs. Test fixtures provide reusable mocks and test assets, adhering to the adapter pattern for external dependencies (MIDI devices, file dialogs, time providers). This structure supports:

1. **Incremental Coverage**: Unit tests added per-component without cross-dependencies
2. **Platform-Specific Tests**: System tests conditionally compiled per platform (AU/macOS, LV2/Linux)
3. **Fast Feedback**: Unit tests run first (fast), system tests last (slow) via CTest cost-based scheduling
4. **CMake Integration**: Single `BUILD_TESTS=ON` flag enables all test infrastructure

## Complexity Tracking

> **No Constitution Violations Identified**

This feature does not introduce architectural complexity that violates ShowMIDI's constitution principles. The TDD adoption aligns with existing architectural standards:

- **Single Test Framework**: JUCE UnitTest (built-in, not a 4th external project)
- **No Design Pattern Overhead**: Adapter pattern is lightweight wrapper, not Repository/Factory/Strategy over-engineering
- **Platform Parity Maintained**: All tests run on macOS/Windows/Linux/iOS without platform-specific workarounds
- **JUCE-Idiomatic**: Test code uses same JUCE types and patterns as production code

**Justification Summary**: N/A (no violations to justify)

---

## Implementation Phases

### Phase 0: Seeding (Week 1)

**Objective**: Establish TDD protocol, seed baseline tests for 10 critical flows, activate CI test jobs.

**Deliverables**:
1. ✅ **TDD Protocol Published** (`contracts/test-protocol.md`)
   - Red-Green-Refactor workflow documented
   - Test scopes (Unit/Integration/System) defined
   - Naming conventions established
   - Review expectations clarified

2. ✅ **Research Complete** (`research.md`)
   - Test framework decision (JUCE UnitTest chosen)
   - Mocking strategy (adapter pattern + FakeIt)
   - Cross-platform validation approach

3. ✅ **Data Model Defined** (`data-model.md`)
   - Test entities (TestSuite, TestCase, Fixture, TestResult, TestReport)
   - State transitions documented
   - Validation rules established

4. ✅ **CI Matrix Configured** (`contracts/ci-matrix.yml`)
   - Platform runners defined (macOS, Windows, Linux, iOS)
   - Test label strategy documented
   - Parallel execution configured

5. ✅ **Coverage Policy** (`contracts/coverage-policy.md`)
   - 90% critical flow coverage (qualitative)
   - Enforcement timeline (report-only for Week 1)
   - Exception process defined

6. **CMake Test Integration** (to be implemented)
   - Add `BUILD_TESTS=ON` option to root `CMakeLists.txt`
   - Create `Tests/CMakeLists.txt` with `juce_add_console_app(ShowMIDI_Tests)`
   - Link JUCE modules (core, audio_basics, audio_processors, gui_basics, etc.)
   - Enable `JUCE_UNIT_TESTS=1` preprocessor definition

7. **Baseline Test Implementation** (to be implemented)
   - **Critical Flow 1**: MIDI message reception (Unit + Integration)
   - **Critical Flow 2**: Channel auto-discovery (Integration)
   - **Critical Flow 3**: Channel auto-hide (Integration)
   - **Critical Flow 4**: Theme loading (Unit)
   - **Critical Flow 5**: Keyboard shortcuts (Integration)
   - **Critical Flow 6**: Plugin initialization (System)
   - **Critical Flow 7**: Standalone device selection (System)
   - **Critical Flow 8**: Settings persistence (Unit + Integration)
   - **Critical Flow 9**: Multi-channel display (Integration)
   - **Critical Flow 10**: High-throughput MIDI (Integration + Performance)

8. **Adapter Interfaces** (to be implemented)
   - `IMidiDeviceAdapter` (`Tests/Fixtures/IMidiDeviceAdapter.h`)
   - `MockMidiAdapter` (`Tests/Fixtures/MockMidiAdapter.{h,cpp}`)
   - `ITimeProvider` (`Tests/Fixtures/ITimeProvider.h`)
   - `SimulatedTimeProvider` (`Tests/Fixtures/SimulatedTimeProvider.{h,cpp}`)

9. **CI Pipeline Extension** (to be implemented)
   - Update `.github/workflows/ci.yml` with test build/run steps
   - Add test result artifact upload (JUnit XML)
   - Configure branch protection (report-only, no blocking)

**Platforms**: macOS, Windows, Linux (iOS in Phase 1)

**Coverage Goal**: 90% of 10 critical flows (qualitative assessment)

**Blocking Policy**: Report-only (no merge blocking)

**Success Criteria**:
- ✅ All documentation complete (protocol, research, data model, contracts)
- [ ] CI jobs execute on all 3 desktop platforms
- [ ] Baseline tests pass locally and in CI
- [ ] New contributors can run tests via quickstart guide

---

### Phase 1: Stabilize (Weeks 2–3)

**Objective**: Expand baseline coverage, introduce iOS simulator tests, reduce test flakiness, reach 50% line coverage.

**Deliverables**:
1. **Expand Unit Tests** (Week 2)
   - `SettingsManagerTests.cpp` (JSON persistence, defaults, validation)
   - `UtilityTests.cpp` (`sm::scaled()` DPI utility, string helpers)
   - `MidiMessageParserTests.cpp` (Note On/Off, CC, Sysex parsing)

2. **Add Integration Tests** (Week 2)
   - `DeviceListenerTests.cpp` (device discovery, removal, state changes)
   - `ThemeApplicationTests.cpp` (SVG parse → color extraction → UI apply)

3. **iOS Simulator Integration** (Week 3)
   - Configure iOS SDK in CMake (`CMAKE_SYSTEM_NAME=iOS`, `CMAKE_OSX_SYSROOT=iphonesimulator`)
   - Add `ShowMIDI_AUv3_Tests` target (smoke tests only)
   - Update CI to run iOS tests on macOS runner

4. **Flakiness Reduction** (Week 3)
   - Identify non-deterministic tests (timing, file paths, randomness)
   - Replace `Time::getMillisecondCounterHiRes()` with `SimulatedTimeProvider`
   - Use absolute paths or `juce::File::getSpecialLocation()` for test assets

5. **Coverage Measurement Setup** (Week 3)
   - Configure Linux job with `CMAKE_BUILD_TYPE=Coverage`
   - Generate lcov report, filter JUCE/system includes
   - Upload to Codecov (badge in README.md)

**Platforms**: macOS, Windows, Linux, iOS (simulator)

**Coverage Goal**: 50% overall line coverage (informational)

**Blocking Policy**: 
- Week 2: Report-only
- Week 3: New test failures block merges, legacy failures allowed with tracking ticket

**Success Criteria**:
- [ ] 50% line coverage measured and reported
- [ ] iOS simulator tests pass on macOS runner
- [ ] Flaky test rate <10% of total tests
- [ ] Integration tests cover top 5 user flows

---

### Phase 2: Expansion (Weeks 4–6)

**Objective**: Introduce plugin format tests (VST3 first), refine mocking, reach 70% line coverage, enable soft gate.

**Deliverables**:
1. **Plugin Host Simulator** (Week 4)
   - Implement `PluginHostSimulator` using `juce::AudioProcessorGraph`
   - Create `PluginVST3Tests.cpp` (load, process block, unload)
   - Validate VST3 on macOS, Windows, Linux

2. **Expand Mocking Infrastructure** (Week 4–5)
   - Introduce FakeIt for complex callback mocking (sparingly)
   - Create `IFileProvider` adapter for file dialogs
   - Add `TestAssetHelper` for generating SVG/MIDI test files programmatically

3. **Coverage Expansion** (Weeks 4–6)
   - Target uncovered components (UI components, visualization logic)
   - Add negative tests (error handling, invalid input)
   - Add boundary tests (16 channels, max velocity, empty buffers)

4. **Coverage Soft Gate Activation** (Week 4)
   - Update CI to warn on PRs with <70% coverage
   - PR comment bot displays coverage diff
   - Maintainer approval required for <70% PRs

5. **Performance Benchmarks** (Week 6)
   - Add `Performance/` test suite (labeled `slow`)
   - Validate MIDI latency <10ms (using `SimulatedTimeProvider`)
   - Measure throughput for dense CC streams

**Platforms**: macOS, Windows, Linux, iOS + VST3 plugin format

**Coverage Goal**: 70% overall line coverage (soft gate)

**Blocking Policy**: Soft gate (warn on <70%, can merge with approval)

**Success Criteria**:
- [ ] 70% line coverage achieved
- [ ] VST3 plugin tests pass on all desktop platforms
- [ ] Performance benchmarks validate <10ms latency
- [ ] PR bot displays coverage diff

---

### Phase 3: Enforcement (Weeks 7–8)

**Objective**: Add AU/AAX/LV2 plugin tests, tighten flaky test management, enforce 80% hard gate for new code.

**Deliverables**:
1. **Plugin Format Tests** (Week 7)
   - `PluginAUTests.cpp` (macOS Audio Unit)
   - `PluginLV2Tests.cpp` (Linux LV2)
   - Defer AAX to Phase 4 (requires Avid SDK licensing)

2. **Flaky Test Quarantine** (Week 7)
   - Implement CTest `WILL_FAIL` property for known flaky tests
   - Track quarantined tests in GitHub Issues
   - Set target: <5% quarantine rate

3. **Coverage Hard Gate Activation** (Week 8)
   - Update CI to fail PRs with <80% coverage on new/modified files
   - Legacy code <80% allowed if unchanged
   - Exception requests via GitHub Issue template

4. **Legacy Test Remediation** (Week 8)
   - Identify all components <50% coverage
   - Add baseline tests to reach 60% minimum
   - Clear or ticket all failing tests

5. **Documentation Finalization** (Week 8)
   - Update CONTRIBUTING.md with TDD section
   - Add "Running Tests" to README.md
   - Record quickstart screencast (optional)

**Platforms**: macOS, Windows, Linux, iOS + VST3, AU, LV2

**Coverage Goal**: 80% overall line coverage (hard gate for new code)

**Blocking Policy**: Hard gate (new code <80% blocks merge, legacy allowed)

**Success Criteria**:
- [ ] 80% line coverage for new code enforced
- [ ] AU and LV2 plugin tests pass
- [ ] Flaky test rate <5%
- [ ] All legacy failures cleared or ticketed

---

### Phase 4: Optimization (Week 9+)

**Objective**: Sustain 80%+ coverage, improve test depth, optimize execution time, add AAX if feasible.

**Deliverables**:
1. **Test Refactoring** (Ongoing)
   - Extract common fixtures to reduce duplication
   - Optimize slow tests (>500ms) via parallelization or mocking
   - Balance unit vs integration tests (prefer unit for speed)

2. **Depth Improvements** (Ongoing)
   - Add system tests for complex workflows (MIDI routing, multi-device scenarios)
   - Increase branch coverage (target 75%+, line coverage insufficient alone)
   - Add stress tests (1000+ MIDI messages/sec, 16 channels active)

3. **CI Performance** (Ongoing)
   - Reduce full suite execution from 15 min → 10 min target
   - Enable CTest parallel execution (`-j4` or higher)
   - Cache CMake builds on CI runners

4. **AAX Plugin Tests** (If Avid SDK acquired)
   - Requires Avid AAX SDK (not open-source)
   - Defer until licensing resolved

5. **Maintenance** (Ongoing)
   - Weekly coverage reports to Discord
   - Quarterly test health review (flakiness, coverage trends)
   - Continuous protocol refinement based on team feedback

**Platforms**: All (macOS, Windows, Linux, iOS, VST3, AU, LV2, AAX if available)

**Coverage Goal**: Sustain 80%+, push towards 85% where practical

**Blocking Policy**: Hard gate maintained (80% for all code)

**Success Criteria**:
- [ ] 80%+ coverage sustained over 4 weeks
- [ ] Full suite execution <10 minutes
- [ ] Zero flaky tests
- [ ] Team reports high confidence in test suite

---

## Phase Summary

| Phase | Weeks | Coverage | Platforms | Formats | Blocking | Key Milestone |
|-------|-------|----------|-----------|---------|----------|---------------|
| **0 Seeding** | 1 | 90% critical flows | macOS, Win, Linux | Standalone | Report-only | Baseline tests + protocol |
| **1 Stabilize** | 2–3 | 50% lines | + iOS sim | + AUv3 | Week 3: New failures block | iOS support + coverage tracking |
| **2 Expansion** | 4–6 | 70% lines | (same) | + VST3 | Soft gate (warn <70%) | Plugin host simulator |
| **3 Enforcement** | 7–8 | 80% lines (new code) | (same) | + AU, LV2 | Hard gate (new code <80%) | Full enforcement |
| **4 Optimization** | 9+ | 80%+ sustained | (same) | + AAX (optional) | Hard gate maintained | Depth + performance |

---

## Risk Assessment

### Technical Risks

| Risk | Impact | Probability | Mitigation Strategy | Owner |
|------|--------|-------------|---------------------|-------|
| **JUCE UnitTest limitations** | High | Low | FakeIt available as backup for complex mocking; adapter pattern handles 90% of needs | Engineering |
| **Headless CI GUI tests** | Medium | Medium | Use `MessageManager::setCurrentThreadAsMessageThread()` for GUI-less execution; Xvfb on Linux | DevOps |
| **iOS simulator flakiness** | Medium | Medium | Limit iOS tests to smoke tests only; prioritize desktop platforms | QA |
| **Coverage tool variance** | Low | High | Use lcov (Linux) as canonical source; macOS/Windows for validation only | QA |
| **Platform-specific test failures** | High | Low | Platform-conditional compilation (`#if JUCE_MAC`, etc.); platform-specific test labels | Engineering |
| **Test execution time >15 min** | Medium | Medium | Parallel execution (`ctest -j4`), cost-based scheduling, prefer unit over system tests | Engineering |
| **Flaky timing tests** | Medium | High | Replace all `Time::getMillisecondCounterHiRes()` with `SimulatedTimeProvider`; no sleeps | Engineering |
| **Legacy code coverage gaps** | Low | High | Phase 3 soft gate allows <80% for legacy; remediate incrementally in Phase 4 | Product |

### Process Risks

| Risk | Impact | Probability | Mitigation Strategy | Owner |
|------|--------|-------------|---------------------|-------|
| **Team TDD adoption resistance** | High | Medium | Quickstart guide <30 min onboarding; pair programming sessions; celebrate early wins | Product |
| **Coverage gaming (empty tests)** | Medium | Medium | Code review focus on meaningful assertions; coverage anti-patterns documented in `coverage-policy.md` | Engineering Lead |
| **Slow PR feedback loop** | Medium | Low | Optimize CI to <15 min; enable local test runs with clear documentation | DevOps |
| **Exception request abuse** | Low | Medium | Strict approval process (GitHub Issue template, maintainer review); public tracking | Product |
| **Week 8 hard gate premature** | High | Low | Soft gate at Week 4 provides 4-week buffer; can extend timeline if <70% by Week 6 | Product |

### Dependency Risks

| Dependency | Risk Description | Mitigation |
|------------|------------------|------------|
| **JUCE 7.0.5** | Breaking changes in future JUCE updates | Pin JUCE version via git submodule; test against JUCE develop branch monthly |
| **CMake 3.15+** | CI runners may use older CMake | Explicitly install CMake 3.15+ in CI setup step; document minimum version |
| **Codecov service** | Free tier limitations or service outage | lcov reports uploaded as artifacts (90-day retention); local coverage generation |
| **GitHub Actions runners** | macOS/Windows runner availability | Retry policy (2 attempts); fallback to self-hosted runners if persistent |
| **FakeIt (if used)** | Unmaintained library (last update 2020) | Adapter pattern is primary strategy; FakeIt is <10% of tests, easily replaced |

---

## Success Metrics

*(Derived from spec.md Success Criteria)*

| Metric | Target | Measurement Method | Reporting Frequency |
|--------|--------|-------------------|---------------------|
| **PR Test Inclusion Rate** | ≥80% by Week 4, ≥95% by Week 8 | Count PRs with test file changes / total PRs | Weekly (automated) |
| **CI Feedback Time** | ≤15 minutes (median) | GitHub Actions workflow duration | Weekly (automated) |
| **Critical Flow Coverage** | ≥90% by Week 1 | Manual checklist of 10 flows | Week 1 milestone |
| **Line Coverage** | 50% Week 3, 70% Week 6, 80% Week 8 | Codecov dashboard | Weekly (automated) |
| **Cross-Platform Pass Rate** | ≥95% rolling 30-day | GitHub Actions success rate | Monthly report |
| **Flaky Test Rate** | ≤5% by Week 8 | Quarantined tests / total tests | Weekly (manual) |
| **New Contributor Onboarding** | ≤30 minutes to first passing test | Timed user study (n=3) | Week 2 validation |

**Tracking Dashboard**: 
- GitHub Project board (weekly updates)
- Weekly coverage report posted to Discord
- Monthly retrospective in team meeting

---

## Next Steps

### Immediate Actions (Week 1)

1. **CMake Integration** (Est: 2 hours)
   - [ ] Add `BUILD_TESTS` option to root `CMakeLists.txt`
   - [ ] Create `Tests/CMakeLists.txt` with `ShowMIDI_Tests` target
   - [ ] Link JUCE modules, enable `JUCE_UNIT_TESTS=1`

2. **First Test Implementation** (Est: 4 hours)
   - [ ] Create `Tests/Unit/ThemeTests.cpp` (simplest, no mocking)
   - [ ] Build and run locally on macOS
   - [ ] Verify test output format

3. **Adapter Interface Design** (Est: 3 hours)
   - [ ] Define `IMidiDeviceAdapter.h` interface
   - [ ] Implement `MockMidiAdapter.cpp` test double
   - [ ] Create example usage in `Tests/Integration/MidiFlowTests.cpp`

4. **CI Pipeline Extension** (Est: 3 hours)
   - [ ] Update `.github/workflows/ci.yml` with test build step
   - [ ] Add test execution for macOS job
   - [ ] Upload test results as artifact (JUnit XML)

5. **Quickstart Validation** (Est: 1 hour)
   - [ ] Recruit new contributor (not familiar with codebase)
   - [ ] Time their first test write-and-run
   - [ ] Iterate on `quickstart.md` based on feedback

**Week 1 Deliverable**: 3 baseline tests passing on macOS CI

### Follow-Up Actions (Weeks 2–3)

1. **Expand to Windows/Linux** (Est: 4 hours)
   - [ ] Verify tests pass on Windows CI runner
   - [ ] Verify tests pass on Linux CI runner
   - [ ] Fix any platform-specific compilation issues

2. **Coverage Setup** (Est: 2 hours)
   - [ ] Configure Linux job with `CMAKE_BUILD_TYPE=Coverage`
   - [ ] Generate and upload lcov report to Codecov
   - [ ] Add coverage badge to README.md

3. **iOS Integration** (Est: 3 hours)
   - [ ] Configure iOS simulator build in CMake
   - [ ] Add smoke test for AUv3 plugin
   - [ ] Update CI to run iOS tests on macOS runner

4. **Baseline Completion** (Est: 12 hours)
   - [ ] Implement remaining 7 critical flow tests (10 total)
   - [ ] Achieve 90% qualitative coverage of critical paths
   - [ ] Document any gaps or deferred tests

**Week 3 Deliverable**: 50% line coverage, iOS tests passing, new failure blocking enabled

---

## Open Questions

*(To be resolved before Phase 2)*

1. **AAX Plugin Testing**: Do we pursue Avid AAX SDK license for Phase 3, or defer AAX tests to Phase 4?
   - **Decision needed by**: Week 5
   - **Impact**: Phase 3 scope and timeline

2. **Self-Hosted Runners**: Should we set up self-hosted runners for faster macOS builds, or rely on GitHub-hosted?
   - **Decision needed by**: Week 4 (if CI times exceed 15 min)
   - **Impact**: CI performance and cost

3. **Test Data Repository**: Large binary test assets (MIDI files, SVG themes) — store in Git LFS or inline?
   - **Decision needed by**: Week 2
   - **Impact**: Repository size and CI clone speed

4. **Coverage Tooling**: Should we add branch coverage measurement (not just line coverage)?
   - **Decision needed by**: Week 6 (before hard gate)
   - **Impact**: Coverage accuracy and enforcement strictness

---

## References

- **Feature Spec**: [spec.md](spec.md)
- **Research**: [research.md](research.md)
- **Data Model**: [data-model.md](data-model.md)
- **Quickstart Guide**: [quickstart.md](quickstart.md)
- **Test Protocol**: [contracts/test-protocol.md](contracts/test-protocol.md)
- **CI Matrix**: [contracts/ci-matrix.yml](contracts/ci-matrix.yml)
- **Coverage Policy**: [contracts/coverage-policy.md](contracts/coverage-policy.md)
- **Constitution**: [.specify/memory/constitution.md](../../.specify/memory/constitution.md)

---

**Document Version**: 1.0.0 (2025-11-11)  
**Next Review**: Week 3 milestone (coverage validation)
