# Tasks: TDD Adoption

**Input**: Design documents from `/specs/004-tdd-adoption/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/, quickstart.md

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story. Since this is TDD adoption, test infrastructure IS the implementation.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3, US4)
- Include exact file paths in descriptions

## Path Conventions

- **Test code**: `Tests/` at repository root
- **Test infrastructure**: `Tests/Fixtures/`, `Tests/Unit/`, `Tests/Integration/`, `Tests/System/`
- **CI configuration**: `.github/workflows/`
- **Root configuration**: `CMakeLists.txt`

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and test framework setup

- [X] T001 Add BUILD_TESTS option to CMakeLists.txt (set to OFF by default)
- [X] T002 Create Tests/ directory structure (Unit/, Integration/, System/, Fixtures/)
- [X] T003 Create Tests/CMakeLists.txt with juce_add_console_app(ShowMIDI_Tests)
- [X] T004 Link JUCE modules in Tests/CMakeLists.txt (juce_core, juce_audio_basics, juce_audio_processors, juce_audio_utils, juce_graphics, juce_gui_basics, juce_gui_extra)
- [X] T005 Add JUCE_UNIT_TESTS=1 compile definition to ShowMIDI_Tests target
- [X] T006 Create Tests/README.md linking to quickstart.md
- [X] T007 Verify JUCE submodule initialized (git submodule update --init --recursive)

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core test infrastructure that MUST be complete before ANY user story can be fully implemented

**⚠️ CRITICAL**: No user story work can begin until adapter interfaces and CI pipeline are ready

- [X] T008 [P] Create Tests/Fixtures/IMidiDeviceAdapter.h interface
- [X] T009 [P] Create Tests/Fixtures/MockMidiAdapter.h header
- [X] T010 [P] Create Tests/Fixtures/MockMidiAdapter.cpp implementation
- [X] T011 [P] Create Tests/Fixtures/ITimeProvider.h interface
- [X] T012 [P] Create Tests/Fixtures/SimulatedTimeProvider.h header
- [X] T013 [P] Create Tests/Fixtures/SimulatedTimeProvider.cpp implementation
- [X] T014 [P] Create Tests/Fixtures/IFileProvider.h interface (for file dialog mocking)
- [X] T015 Build ShowMIDI_Tests target locally on macOS to verify CMake integration (success: binary builds without errors)
- [X] T016 Run empty test suite to verify JUCE UnitTest framework activation (expected output: "0 tests run, 0 failures" or JUCE test runner initialization message)

**Checkpoint**: Foundation ready - test implementation can now begin in parallel

---

## Phase 3: User Story 1 - Team adopts a simple TDD protocol (Priority: P1) 🎯 MVP

**Goal**: Engineers can follow Red-Green-Refactor workflow with clear naming, structure, and scope guidance

**Independent Test**: Protocol can be piloted on a single component, producing failing test → passing implementation → refactor commit

### Implementation for User Story 1

- [X] T017 [US1] Verify contracts/test-protocol.md exists and documents Red-Green-Refactor workflow
- [X] T018 [US1] Verify contracts/test-protocol.md defines test scopes (Unit/Integration/System)
- [X] T019 [US1] Verify contracts/test-protocol.md includes naming conventions (ComponentNameTests, "should X when Y")
- [X] T020 [US1] Verify contracts/test-protocol.md includes review expectations and Definition of Done (DoD MUST require tests for all new code and refactors)
- [X] T020a [US1] Verify contracts/test-protocol.md or quickstart.md documents platform-conditional compilation patterns (#if JUCE_MAC, test labels: ios, plugin-au, plugin-lv2)
- [X] T021 [US1] Create example test in Tests/Unit/ExampleTest.cpp demonstrating protocol
- [X] T022 [US1] Update CONTRIBUTING.md with link to test-protocol.md
- [X] T023 [US1] Validate quickstart.md enables new contributor to write first test in <30 minutes

**Checkpoint**: TDD protocol is documented and validated via example test

---

## Phase 4: User Story 2 - Cross-platform test execution (Priority: P1)

**Goal**: Tests run consistently on macOS, Windows, Linux, iOS with clear per-platform feedback

**Independent Test**: Triggering tests on a sample change runs platform jobs and reports per-platform results within target time (<15 min)

### Implementation for User Story 2

- [X] T024 [P] [US2] Update .github/workflows/ci.yml to add test-macos job with CMake build and ctest execution
- [X] T025 [P] [US2] Update .github/workflows/ci.yml to add test-windows job with CMake build and ctest execution
- [X] T026 [P] [US2] Update .github/workflows/ci.yml to add test-linux job with CMake build, dependencies install, and ctest execution
- [X] T027 [US2] Configure test result artifact upload (JUnit XML format) in all platform jobs
- [X] T028 [US2] Add test job triggers for PRs to develop and main branches
- [X] T029 [US2] Set artifact retention to 90 days in workflow configuration
- [X] T030 [US2] Verify contracts/ci-matrix.yml documents platform-specific test labels (unit, integration, system, midi, ui, ios, plugin-vst3, plugin-au, plugin-lv2)
- [X] T031 [US2] Update CMakeLists.txt to register CTest with enable_testing()
- [X] T032 [US2] Add CTest test labels configuration in Tests/CMakeLists.txt
- [X] T033 [US2] Trigger test run on CI for all three desktop platforms (macOS, Windows, Linux) - PR #24 created
- [ ] T034 [US2] Validate median CI feedback time is <15 minutes (awaiting first CI run completion)

**Checkpoint**: CI runs tests on all desktop platforms with clear per-platform status

---

## Phase 5: User Story 3 - Baseline tests cover critical behaviors (Priority: P2)

**Goal**: Baseline suite covers 10 critical flows (90% qualitative coverage) to prevent regressions

**Independent Test**: Running baseline suite locally reproduces core behaviors and prevents known regressions

### Critical Flow Tests for User Story 3

**Note**: These are the actual test implementations for critical flows

- [ ] T035 [P] [US3] Create Tests/Unit/ThemeTests.cpp with test for loading dark theme from SVG file
- [ ] T036 [P] [US3] Create Tests/Unit/SettingsManagerTests.cpp with tests for JSON persistence and defaults
- [ ] T037 [P] [US3] Create Tests/Unit/MidiMessageParserTests.cpp with tests for Note On/Off and CC parsing
- [ ] T038 [P] [US3] Create Tests/Integration/MidiFlowTests.cpp with test for MIDI input → listener → UI display
- [ ] T039 [P] [US3] Create Tests/Integration/DeviceDiscoveryTests.cpp with test for new MIDI device appearing in sidebar
- [ ] T040 [P] [US3] Create Tests/Integration/ChannelAutoHideTests.cpp with test for inactive channel timeout using SimulatedTimeProvider
- [ ] T041 [P] [US3] Create Tests/Integration/ThemeApplicationTests.cpp with test for SVG drop → theme applies to UI
- [ ] T042 [P] [US3] Create Tests/Integration/KeyboardShortcutsTests.cpp with tests for spacebar (pause), delete (reset), V (toggle viz)
- [ ] T043 [US3] Create Tests/System/StandaloneAppTests.cpp with test for app launch without crash (headless mode)
- [ ] T044 [US3] Create Tests/System/PluginInitializationTests.cpp with test for VST3 plugin load in AudioProcessorGraph
- [ ] T045 [US3] Create Tests/System/DeviceSelectionTests.cpp with test for MIDI input selection in standalone app
- [ ] T046 [US3] Create Tests/Integration/SettingsPersistenceTests.cpp with test for save on quit, restore on launch
- [ ] T047 [US3] Create Tests/Integration/MultiChannelDisplayTests.cpp with test for 16 simultaneous channels
- [ ] T048 [US3] Create Tests/Integration/HighThroughputMidiTests.cpp with test for dense CC automation without drops

### Test Fixtures for User Story 3

- [ ] T049 [P] [US3] Create Tests/Fixtures/dark-theme-test.svg (minimal SVG for theme tests)
- [ ] T050 [P] [US3] Create Tests/Fixtures/TestThemeHelper.h with utility to generate test SVG themes programmatically
- [ ] T051 [P] [US3] Create Tests/Fixtures/test-midi-sequence.mid (binary MIDI file for playback tests)

### Validation for User Story 3

- [ ] T052 [US3] Run baseline test suite locally and verify all tests pass
- [ ] T053 [US3] Run baseline test suite on CI and verify all platforms pass
- [ ] T054 [US3] Verify 90% of 10 critical flows have at least one passing test

**Checkpoint**: Baseline suite covers critical flows and passes on all platforms

---

## Phase 6: User Story 4 - Developers can run tests quickly locally (Priority: P2)

**Goal**: Developers run all or filtered tests locally with clear output, deterministic behavior, documented fixtures

**Independent Test**: New developer runs full suite or subset and understands failures without external help

### Implementation for User Story 4

- [ ] T055 [US4] Verify quickstart.md includes 5-minute quick start section with build and run commands
- [ ] T056 [US4] Verify quickstart.md includes filtering examples (--category MIDI, ctest -L unit)
- [ ] T057 [US4] Verify quickstart.md includes troubleshooting section for common build/runtime errors
- [ ] T058 [US4] Verify quickstart.md includes debugging section (Xcode, Visual Studio, GDB)
- [ ] T059 [US4] Add cost-based scheduling to CTest in Tests/CMakeLists.txt (unit=5-10, integration=20-40, system=50-100)
- [ ] T060 [US4] Configure parallel test execution with ctest -j4 documentation in quickstart.md
- [ ] T061 [US4] Ensure all tests use SimulatedTimeProvider instead of Time::getMillisecondCounterHiRes() for determinism
- [ ] T062 [US4] Ensure all tests use absolute paths or juce::File::getSpecialLocation() for test assets
- [ ] T063 [US4] Ensure all tests call MessageManager::setCurrentThreadAsMessageThread() for headless execution
- [ ] T064 [US4] Add clear assertion failure messages to all expect() calls in tests
- [ ] T065 [US4] Recruit new contributor and time their first test write-and-run (target: <30 minutes)
- [ ] T066 [US4] Measure full test suite execution time locally (target: <15 minutes)

**Checkpoint**: New developers can run tests locally with clear feedback in <30 minutes

---

## Phase 7: Stabilize & Expand Coverage (Weeks 2-3)

**Purpose**: Expand unit tests, add iOS simulator support, reduce flakiness, reach 50% line coverage

- [ ] T067 [P] Add unit tests for UtilityTests.cpp (sm::scaled() DPI utility, string helpers)
- [ ] T068 [P] Expand SettingsManagerTests.cpp with validation tests
- [ ] T069 [P] Expand MidiMessageParserTests.cpp with Sysex parsing tests
- [ ] T070 [P] Expand DeviceDiscoveryTests.cpp with device removal and state change tests
- [ ] T071 Configure iOS SDK in CMake (CMAKE_SYSTEM_NAME=iOS, CMAKE_OSX_SYSROOT=iphonesimulator)
- [ ] T072 Add ShowMIDI_AUv3_Tests target in Tests/CMakeLists.txt for iOS
- [ ] T073 Create Tests/System/IOSSimulatorTests.cpp with AUv3 smoke tests
- [ ] T074 Update .github/workflows/ci.yml test-macos job to include iOS simulator tests
- [ ] T075 Identify non-deterministic tests via CI retry failures
- [ ] T076 Replace all remaining Time::getMillisecondCounterHiRes() calls with SimulatedTimeProvider
- [ ] T077 Configure Linux CI job with CMAKE_BUILD_TYPE=Coverage
- [ ] T078 Add lcov report generation and Codecov upload to Linux CI job
- [ ] T079 Add coverage badge to README.md
- [ ] T080 Measure and report 50% line coverage target
- [ ] T081 Update branch protection to block new test failures from Week 3 onward

**Checkpoint**: 50% line coverage, iOS tests pass, flaky test rate <10%

---

## Phase 8: Expansion - Plugin Tests & Soft Gate (Weeks 4-6)

**Purpose**: Add VST3 plugin tests, refine mocking, reach 70% coverage, enable soft gate

- [ ] T082 [P] Create Tests/Fixtures/PluginHostSimulator.h using juce::AudioProcessorGraph
- [ ] T083 [P] Create Tests/Fixtures/PluginHostSimulator.cpp implementation
- [ ] T084 Create Tests/System/PluginVST3Tests.cpp with load, process block, unload tests
- [ ] T085 Validate VST3 tests pass on macOS, Windows, Linux CI jobs
- [ ] T086 [P] Introduce FakeIt header-only library for complex callback mocking (add to Tests/Fixtures/)
- [ ] T087 [P] Create Tests/Fixtures/MockFileProvider.h implementation of IFileProvider
- [ ] T088 [P] Create Tests/Fixtures/TestAssetHelper.h for generating SVG/MIDI files programmatically
- [ ] T089 [P] Add negative tests to existing test suites (error handling, invalid input)
- [ ] T090 [P] Add boundary tests (16 channels, max velocity, empty buffers)
- [ ] T091 Update .github/workflows/ci.yml to warn on PRs with <70% coverage (soft gate)
- [ ] T092 Add PR comment bot configuration for coverage diff display
- [ ] T093 Create Tests/Performance/ directory for performance benchmark tests
- [ ] T094 Create Tests/Performance/MidiLatencyTests.cpp validating <10ms latency
- [ ] T095 Create Tests/Performance/ThroughputTests.cpp measuring dense CC stream handling
- [ ] T096 Measure and report 70% line coverage target

**Checkpoint**: 70% coverage, VST3 tests pass, soft gate active

---

## Phase 9: Enforcement - Hard Gate & Plugin Formats (Weeks 7-8)

**Purpose**: Add AU/LV2 tests, enforce 80% hard gate, remediate legacy code

- [ ] T097 [P] Create Tests/System/PluginAUTests.cpp for macOS Audio Unit tests
- [ ] T098 [P] Create Tests/System/PluginLV2Tests.cpp for Linux LV2 tests
- [ ] T099 Add CTest WILL_FAIL property configuration for quarantined flaky tests
- [ ] T100 Create GitHub Issue template for flaky test tracking
- [ ] T101 Update .github/workflows/ci.yml to fail PRs with <80% coverage on new/modified files (hard gate)
- [ ] T102 Add incremental coverage check script (compares only changed files)
- [ ] T103 Create GitHub Issue template for coverage exception requests
- [ ] T104 Identify all components with <50% coverage
- [ ] T105 Add baseline tests to bring all components to >=60% coverage
- [ ] T106 Clear or ticket all failing tests
- [ ] T107 Update CONTRIBUTING.md with TDD section linking to test-protocol.md
- [ ] T108 Add "Running Tests" section to README.md
- [ ] T109 Measure and report 80% line coverage for new code
- [ ] T110 Validate flaky test rate <5%

**Checkpoint**: 80% coverage hard gate active, AU/LV2 tests pass, flaky rate <5%

---

## Phase 10: Polish & Cross-Cutting Concerns

**Purpose**: Finalize documentation, optimize performance, sustain coverage

- [ ] T111 [P] Extract common test fixtures to reduce duplication across test files
- [ ] T112 [P] Optimize slow tests (>500ms) via parallelization or improved mocking
- [ ] T113 [P] Add system tests for complex workflows (MIDI routing, multi-device scenarios)
- [ ] T114 [P] Increase branch coverage measurement (target 75%+)
- [ ] T115 [P] Add stress tests (1000+ MIDI messages/sec, 16 channels active)
- [ ] T116 Reduce full suite execution time from 15 min to <10 min target
- [ ] T117 Enable CTest parallel execution with -j4 or higher in CI
- [ ] T118 Add CMake build caching to CI runners
- [ ] T119 Setup weekly coverage reports to Discord webhook
- [ ] T120 Create quarterly test health review process documentation
- [ ] T121 Record optional quickstart screencast for new contributors
- [ ] T122 Validate sustained 80%+ coverage over 4-week period
- [ ] T123 Validate zero flaky tests in quarantine
- [ ] T124 Conduct team retrospective on TDD adoption process

**Checkpoint**: Full test suite <10 min, 80%+ coverage sustained, zero flaky tests

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion - BLOCKS all user stories
- **User Stories (Phase 3-6)**: All depend on Foundational phase completion
  - US1 (Protocol), US2 (CI), US3 (Baseline), US4 (Local dev) can proceed in parallel
  - US3 (Baseline tests) requires US1 (Protocol) and US2 (CI) for validation
- **Stabilize (Phase 7)**: Depends on US3 baseline tests being complete
- **Expansion (Phase 8)**: Depends on Phase 7 (50% coverage baseline)
- **Enforcement (Phase 9)**: Depends on Phase 8 (70% coverage)
- **Polish (Phase 10)**: Depends on Phase 9 (80% coverage hard gate)

### User Story Dependencies

- **User Story 1 (Protocol - P1)**: Can start after Foundational (Phase 2) - Validates documentation exists
- **User Story 2 (CI - P1)**: Can start after Foundational (Phase 2) - Enables platform testing
- **User Story 3 (Baseline - P2)**: Depends on US1 (protocol to follow) and US2 (CI to validate)
- **User Story 4 (Local Dev - P2)**: Can start after Foundational (Phase 2) - Improves developer experience

### Within Each User Story

- **US1**: Documentation validation tasks can run in parallel
- **US2**: CI job additions can run in parallel (T024-T026)
- **US3**: Test file creation can run in parallel (T035-T048), fixtures in parallel (T049-T051)
- **US4**: Documentation validation and optimization tasks can run in parallel

### Parallel Opportunities

- All Setup tasks (T001-T007) can run in parallel
- All Foundational adapter interfaces (T008-T014) can run in parallel
- US1, US2, US4 documentation tasks can run in parallel
- US3 test file creation (T035-T051) can run in parallel - 17 test files
- Phase 7 unit test expansion (T067-T070) can run in parallel
- Phase 8 plugin simulator and mocking (T082-T088) can run in parallel
- Phase 9 plugin format tests (T097-T098) can run in parallel
- Phase 10 polish tasks (T111-T115) can run in parallel

---

## Parallel Example: User Story 3 (Baseline Tests)

```bash
# Launch all test file creation tasks together:
Task: "Create Tests/Unit/ThemeTests.cpp"
Task: "Create Tests/Unit/SettingsManagerTests.cpp"
Task: "Create Tests/Unit/MidiMessageParserTests.cpp"
Task: "Create Tests/Integration/MidiFlowTests.cpp"
Task: "Create Tests/Integration/DeviceDiscoveryTests.cpp"
Task: "Create Tests/Integration/ChannelAutoHideTests.cpp"
Task: "Create Tests/Integration/ThemeApplicationTests.cpp"
Task: "Create Tests/Integration/KeyboardShortcutsTests.cpp"
Task: "Create Tests/System/StandaloneAppTests.cpp"
Task: "Create Tests/System/PluginInitializationTests.cpp"
Task: "Create Tests/System/DeviceSelectionTests.cpp"
Task: "Create Tests/Integration/SettingsPersistenceTests.cpp"
Task: "Create Tests/Integration/MultiChannelDisplayTests.cpp"
Task: "Create Tests/Integration/HighThroughputMidiTests.cpp"

# Launch all fixture creation tasks together:
Task: "Create Tests/Fixtures/dark-theme-test.svg"
Task: "Create Tests/Fixtures/TestThemeHelper.h"
Task: "Create Tests/Fixtures/test-midi-sequence.mid"
```

---

## Implementation Strategy

### MVP First (User Stories 1 + 2 Only)

1. Complete Phase 1: Setup (T001-T007)
2. Complete Phase 2: Foundational (T008-T016) - CRITICAL blocker
3. Complete Phase 3: User Story 1 - Protocol (T017-T023)
4. Complete Phase 4: User Story 2 - CI (T024-T034)
5. **STOP and VALIDATE**: Verify protocol is documented and CI runs on all platforms
6. Demo to team for feedback

### Incremental Delivery

1. Complete Setup + Foundational → Foundation ready
2. Add US1 (Protocol) + US2 (CI) → Test infrastructure and CI working (MVP!)
3. Add US3 (Baseline tests) → 90% critical flow coverage, safety net in place
4. Add US4 (Local dev) → Developer experience optimized
5. Phase 7 (Stabilize) → 50% coverage, iOS support
6. Phase 8 (Expansion) → 70% coverage, VST3 tests
7. Phase 9 (Enforcement) → 80% hard gate, AU/LV2 tests
8. Phase 10 (Polish) → Sustained quality, <10 min suite

### Parallel Team Strategy

With multiple developers:

1. Team completes Setup + Foundational together (T001-T016)
2. Once Foundational is done:
   - Developer A: User Story 1 (Protocol validation)
   - Developer B: User Story 2 (CI pipeline)
   - Developer C: User Story 4 (Local dev docs)
3. After US1+US2 complete:
   - All developers: User Story 3 (Baseline tests) - parallelize test file creation
4. Continue with Phase 7-10 sequentially (coverage-gated)

---

## Notes

- [P] tasks = different files, no dependencies
- [Story] label maps task to specific user story for traceability
- Each user story should be independently completable and testable
- Tests ARE the implementation for this TDD adoption feature
- All test files must include GPL-3.0 license headers
- Commit after each task or logical group
- Stop at any checkpoint to validate progress
- Total tasks: 124 (excludes documentation verification tasks already complete)
- Estimated parallel tasks: 40+ tasks can run simultaneously in US3 alone
