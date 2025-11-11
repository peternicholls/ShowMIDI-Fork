# Feature Specification: TDD Adoption

**Feature Branch**: `004-tdd-adoption`  
**Created**: 2025-11-11  
**Status**: Draft  
**Input**: User description: "we need to transition to test driven development. currently there are no tests in place. we need to create a strategy that works across the whole code base and for each platform we're targeting. we need to design a protocol to follow, and then set up tests for our source code as it stands now."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Team adopts a simple TDD protocol (Priority: P1)

Engineers can follow a clear, lightweight protocol to write a failing test before implementation, make it pass, and refactor safely, with guidance on naming, structure, and scope of tests.

**Why this priority**: Establishing a shared protocol unlocks consistent quality and enables all other testing improvements.

**Independent Test**: The protocol can be piloted on a small change (e.g., a single component), producing a failing test, a passing implementation, and a refactor commit that keeps tests green.

**Acceptance Scenarios**:

1. Given a new task, When an engineer starts work, Then they create a failing test that expresses the desired behavior before writing implementation.
2. Given a failing test, When the engineer implements the minimal code to pass, Then the test suite passes locally and in CI.
3. Given passing tests, When the engineer refactors the code, Then tests remain green and changes are documented in the PR.

---

### User Story 2 - Cross-platform test execution provides reliable feedback (Priority: P1)

All automated tests run consistently on supported platforms (macOS, Windows, Linux, and iOS simulator) and provide clear pass/fail feedback per platform to gate merges.

**Why this priority**: The app targets multiple platforms; results must be trustworthy everywhere to avoid regressions.

**Independent Test**: Triggering tests on a sample change runs platform jobs and reports per-platform results within target time.

**Acceptance Scenarios**:

1. Given a PR, When CI runs tests, Then results show separate statuses for macOS, Windows, Linux, and iOS.
2. Given a transient failure, When tests are retried, Then flaky detection or quarantining policies are applied and surfaced clearly.

---

### User Story 3 - Baseline tests cover critical behaviors (Priority: P2)

The current code base gains a baseline suite covering the most critical flows so that future TDD work builds on a safety net.

**Why this priority**: Establishing coverage of critical paths prevents regressions while we migrate to TDD.

**Independent Test**: Running the baseline suite locally reproduces core behaviors and prevents known regressions.

**Acceptance Scenarios**:

1. Given the existing source code, When baseline tests are executed, Then critical flows pass on all supported platforms.
2. Given a change that breaks a critical flow, When tests run, Then at least one baseline test fails with a clear message.

---

### User Story 4 - Developers can run tests quickly locally (Priority: P2)

Developers can run all or filtered tests locally with clear output, deterministic behavior, and documented fixtures.

**Why this priority**: Fast feedback accelerates adoption and improves developer experience.

**Independent Test**: A developer new to the project can run the full suite or a subset and understand failures without external help.

**Acceptance Scenarios**:

1. Given a fresh checkout, When a developer follows documented steps, Then tests run locally to completion with clear summary output.
2. Given platform-conditional logic, When running locally on any supported OS, Then platform-specific tests behave deterministically and skip or simulate hardware-dependent behavior.

---

### Edge Cases

- Platform-specific behavior that differs subtly between macOS, Windows, and Linux (e.g., timing, file paths, UI metrics).
- Non-deterministic tests due to concurrency, timing, or audio/MIDI hardware availability.
- Tests requiring GUI rendering or plugin hosts that are not available in headless environments.
- Large binary test assets leading to slow or flaky runs.
- Time zone, locale, and DPI scaling differences affecting expectations.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The organization MUST publish a concise TDD protocol describing test-first workflow (red-green-refactor), test scopes (unit/integration/system), naming conventions, and review expectations.
- **FR-002**: The system MUST provide a way to execute the full test suite on each supported platform and surface per-platform pass/fail in PRs.
- **FR-003**: The system MUST enable developers to run tests locally with a single documented command and to filter tests by component or tag.
- **FR-004**: The project MUST include baseline tests covering critical user-visible flows and safeguards around known regressions within the initial adoption phase.
- **FR-005**: The test approach MUST include strategies to simulate or mock external dependencies (e.g., hardware MIDI devices, plugin hosts, file dialogs) to ensure deterministic tests. Implementation MUST use adapter pattern (e.g., `IMidiDeviceAdapter`, `IFileProvider`) and `juce::AudioProcessorGraph` for plugin host simulation as documented in plan.md.
- **FR-006**: The suite MUST include guidance for handling platform-conditional paths (e.g., `#if JUCE_MAC`, platform-specific test labels) to avoid false failures and ensure parity across OSes. Guidance MUST be documented in test-protocol.md or quickstart.md.
- **FR-007**: The process MUST establish a definition-of-done that includes tests for new code and refactors (with allowances for legacy code touched incidentally).
- **FR-008**: The system MUST detect and manage flaky tests (e.g., quarantine, retry policy, reporting) to maintain trust in results. A test is considered "flaky" if it fails >5% of CI runs over a 7-day period with no code changes, or if it exhibits non-deterministic behavior (timing-dependent, hardware-dependent, or randomized).
- **FR-009**: The project MUST achieve 90% coverage of critical flows within the first adoption week (non-critical code excluded), then expand to 70% line coverage overall by week 4, and 80% by week 8; enforcement shifts from reporting-only (weeks 1-3) to soft gate (week 4) and hard gate (week 8) for new code.
- **FR-010**: Automated system tests MUST initially cover the standalone desktop application (macOS, Windows, Linux) and add iOS simulator smoke tests in phase 2 (week 3) followed by plugin format smoke tests (VST3 first, AU/AAX/LV2 phased by week 8).
- **FR-011**: During baseline creation (weeks 1-2) test failures DO NOT hard-block merges; from week 3 onward new test failures block merges while legacy failing tests require tracking tickets and may merge with annotation until resolved by week 6.
- **FR-012**: Test robustness MUST be demonstrated via scenario coverage (happy paths, failure paths, boundary conditions, and concurrency/time-sensitive cases), including at least one negative test per critical flow.
 
### Phased Rollout Plan

| Phase | Weeks | Focus | Platforms Included | Blocking Policy | Coverage Goal |
|-------|-------|-------|--------------------|-----------------|---------------|
| 0 Seeding | 1 | Establish protocol, seed baseline critical tests | macOS, Windows, Linux | No hard block; report only | 90% of critical flows (qualitative) |
| 1 Stabilize | 2-3 | Expand baseline, reduce flakiness | macOS, Windows, Linux + iOS simulator (smoke) | Week 2 report; Week 3 new failures block | 50% overall lines (informational) |
| 2 Expansion | 4-6 | Introduce plugin smoke tests (VST3 first), refine mocks | macOS, Windows, Linux, iOS + VST3 | Soft gate at week 4; legacy failures ticketed | 70% overall lines (soft gate) |
| 3 Enforcement | 7-8 | Add AU/AAX/LV2 smoke tests, tighten flaky management | All platforms/formats | Hard gate for new code; legacy failures cleared | 80% overall lines (hard gate new code) |
| 4 Optimization | 9+ | Improve depth (integration/system), performance, test refactors | All | Hard gate maintained | >80% sustained, push towards 85% where practical |

### Key Entities *(include if feature involves data)*

- **Test Suite**: A collection of test cases organized by scope (unit, integration, system) and tags; includes platform-conditional annotations.
- **Test Case**: A deterministic specification of expected behavior; includes setup, action, and assertions.
- **Fixture**: Reusable setup data or environment configuration to isolate tests from external dependencies.
- **Platform Matrix**: The set of supported environments (OS versions, architectures) against which tests are run.
- **Component Under Test (CUT)**: Any module or feature with defined inputs/outputs and observable behavior.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Within 4 weeks, ≥80% of new PRs include at least one test; within 8 weeks, ≥95%.
- **SC-002**: Median time from PR open to full cross-platform test feedback ≤ 15 minutes.
- **SC-003**: Baseline suite covers ≥ 90% of identified critical flows within 1 week.
- **SC-004**: Cross-platform pass rate ≥ 95% over a rolling 30-day window.
- **SC-005**: Flaky test quarantine rate ≤ 5% of total tests after 8 weeks.
- **SC-006**: New contributor can write and run their first passing test in ≤ 30 minutes following documentation.

## Assumptions

- **Timeline Baseline**: "Week 1" begins on the date this feature branch is merged to `develop`. All subsequent week references (Week 2-8) are calendar weeks from that merge date.
- Supported platforms are macOS, Windows, Linux, and iOS (simulator) for application deliverables; plugin formats (VST3, AU, AAX, LV2) introduced progressively.
- Some behaviors (e.g., GUI rendering, device I/O) require simulation/mocking for deterministic tests.
- Legacy code without tests will be incrementally covered; not all code attains coverage targets immediately.
- **Critical Flow Definition**: A flow is "critical" if it affects MIDI message processing, device discovery, or standalone app launch (constitution's Real-Time Performance and User-Centric Design principles).
