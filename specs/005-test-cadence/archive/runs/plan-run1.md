# Implementation Plan: Phase 11 — Testing Cadence & Governance (Run 1 of 3)

**Branch**: `005-test-cadence` | **Date**: 2025-11-12 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/005-test-cadence/spec.md`

**Note**: This is Run 1 of 3 consecutive planning iterations. Final plan will cherry-pick from all runs.

## Summary

Establish comprehensive testing cadence and governance framework aligned with TDD principles, defining when and how to run unit, integration, system, performance, UI/visual regression, static analysis, formatting, linting, security (SCA/SAST), license compliance, build verification, packaging, and installer smoke tests across local development, pre-commit, pre-push, PR, CI (macOS/Windows/Linux), nightly, release, and hotfix workflows. Optimize for fast feedback (≤5 min PR median), determinism, parallelization, and cost-consciousness while maintaining comprehensive coverage. Deliverable: Constitution amendment recommendations codifying test taxonomy, triggers, time budgets, flake policies, platform coverage, and ownership.

## Technical Context

**Language/Version**: Markdown, YAML (GitHub Actions), Shell (Bash/Zsh for local hooks)
**Primary Dependencies**: 
- GitHub Actions (CI/CD platform)
- JUCE UnitTest framework (existing from 004-tdd-adoption)
- CTest (CMake test runner)
- clang-format, clang-tidy (static analysis/formatting)
- Git hooks (pre-commit, pre-push automation)

**Storage**: N/A (policy/governance documentation; test results in CI artifacts)
**Testing**: Meta-level (this feature defines testing strategy itself); validation via policy compliance checks
**Target Platform**: Cross-platform CI/CD (GitHub Actions runners: ubuntu-latest, macos-latest, windows-latest)
**Project Type**: Governance/process framework (produces documentation artifacts and CI/CD workflow updates)
**Performance Goals**: 
- PR check median ≤ 5 minutes
- PR check p95 ≤ 10 minutes
- Nightly full suite ≤ 60 minutes (change detection optimized)

**Constraints**: 
- Public repository: unlimited GitHub Actions minutes (no cost ceiling)
- Must integrate with existing TDD adoption (004-tdd-adoption baseline)
- Determinism: ≤2% re-run rate across required checks
- Flake quarantine: ≥95% sustained pass rate for re-enablement

**Scale/Scope**: 
- 12 check categories (unit, integration, system, performance, UI/visual, static analysis, format, lint, security, license, build, packaging/installer)
- 8 trigger contexts (local on-demand, pre-commit, pre-push, PR, on-merge, nightly, release, hotfix)
- 4 platforms (macOS, Windows, Linux, iOS simulator)
- Progressive gates: feature → develop → release → main

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

| Principle | Applies | Assessment | Violation? |
|-----------|---------|------------|------------|
| **Multi-Platform Architecture** | ✅ Yes | Platform coverage policy (FR-007) ensures parity across macOS/Windows/Linux with nightly comprehensive validation and release full validation. PR checks run on macOS primary + platform-specific when needed. | ✅ Pass |
| **JUCE Framework Standards** | ⚠️ Partial | Existing test infrastructure (004-tdd-adoption) uses JUCE UnitTest; this phase defines when/how tests run, not implementation. No direct JUCE code in governance docs. | ✅ Pass (N/A for process) |
| **Real-Time Performance** | ✅ Yes | Performance tests included in taxonomy (FR-001); run nightly to avoid blocking PR velocity. Time budgets protect TDD loop (≤5 min median). Aligns with constitution's <10ms latency requirement by ensuring tests catch regressions. | ✅ Pass |
| **User-Centric Design** | ✅ Yes | Fast feedback (SC-001) and deterministic checks (SC-002) protect developer experience, which directly supports user-centric development velocity. Flake governance (FR-005) prevents noise. | ✅ Pass |
| **Maintainability** | ✅ Yes | Ownership mapping (FR-008), skip/bypass governance (FR-010), aggregated reporting (FR-009), and Constitution amendment deliverable (FR-014) all enhance long-term maintainability of testing practices. | ✅ Pass |
| **Development Workflow (GitFlow)** | ✅ Yes | Branch-specific gates (FR-006) align perfectly with GitFlow: feature PRs → develop merges → release branches → main. Progressive depth matches workflow stages. | ✅ Pass |
| **CI/CD Integration** | ✅ Yes | Extends existing `.github/workflows/ci.yml` with structured triggers (FR-002) and platform coverage (FR-007). Nightly, release, hotfix cadences map to existing pipeline triggers. | ✅ Pass |
| **Quality Gates** | ✅ Yes | FR-006 defines blocking vs advisory classification; staging gate requirement ensures no regression reaches develop/main. Exceeds existing quality gates by adding flake detection and progressive depth. | ✅ Pass |

**Overall**: ✅ **PASS** — No constitution violations. This phase strengthens constitutional compliance by codifying testing practices that protect multi-platform reliability, real-time performance, and maintainability.

## Project Structure

### Documentation (this feature — Phase 11)

```text
specs/005-test-cadence/
├── plan.md                                  # This file (Run 1 of 3)
├── research.md                              # Phase 0: Technology/pattern decisions
├── data-model.md                            # Phase 1: Entities, relationships, validation
├── quickstart.md                            # Phase 1: Developer onboarding guide
├── contracts/
│   └── testing-governance-schema.md         # YAML/JSON schemas for config/tracking
└── checklists/
    └── requirements.md                      # Spec quality validation (pre-planning)
```

### Configuration & CI/CD (target deployment locations)

```text
.github/
├── testing-governance.yaml                  # Primary config (check categories, triggers, policies, owners)
└── workflows/
    ├── ci.yml                               # Enhanced with structured triggers and gates
    ├── nightly.yml                          # Full platform coverage (new)
    ├── release.yml                          # Release-specific gates (new)
    └── flake-detector.yml                   # Automated flake quarantine workflow (new)

.git/hooks/
├── pre-commit                               # Local: format, lint, GPL headers
└── pre-push                                 # Local: unit tests

scripts/
├── install-hooks.sh                         # One-time setup for developers
├── validate-testing-config.sh               # Schema validation for testing-governance.yaml
└── hooks/
    ├── pre-commit                           # Source for .git/hooks/pre-commit
    └── pre-push                             # Source for .git/hooks/pre-push
```

### Test Organization (existing from 004-tdd-adoption)

```text
Tests/
├── Unit/                                    # Fast isolated tests (TriggerContext: local, pre-push, PR, nightly)
├── Integration/                             # Core flow validation (TriggerContext: PR, nightly)
├── System/                                  # End-to-end scenarios (TriggerContext: nightly, release)
└── Fixtures/                                # Shared test doubles

Source/                                      # Production code (unchanged structure)
```

### Runtime Artifacts (CI outputs, not git-versioned)

```text
GitHub Actions Artifacts:
├── flake-tracking.json                      # Per-check pass/fail history, quarantined checks
├── test-run-summary.json                    # Per-run results with metrics
└── test-reports/                            # Platform-specific CTest XML outputs
    ├── macos-test-report.xml
    ├── windows-test-report.xml
    └── linux-test-report.xml
```

**Structure Decision**: This is a **governance/process feature** producing configuration artifacts and documentation. No new production code directories. Enhances existing test infrastructure (Tests/) and CI/CD workflows (.github/workflows/). Git hooks (scripts/hooks/) and configuration (testing-governance.yaml) are the primary deliverables.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| [e.g., 4th project] | [current need] | [why 3 projects insufficient] |
| [e.g., Repository pattern] | [specific problem] | [why direct DB access insufficient] |
