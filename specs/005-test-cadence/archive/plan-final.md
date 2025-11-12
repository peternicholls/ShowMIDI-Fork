# Implementation Plan: Phase 11 — Testing Cadence & Governance (Final)

**Branch**: `005-test-cadence` | **Date**: 2025-11-12 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/005-test-cadence/spec.md`
**Consolidated From**: Run 1 (base) + Run 2 (clarification tracking) + Run 3 (conciseness)

## Summary

Establish comprehensive testing cadence and governance framework aligned with TDD principles, defining when and how to run unit, integration, system, performance, UI/visual regression, static analysis, formatting, linting, security (SCA/SAST), license compliance, build verification, packaging, and installer smoke tests across local development, pre-commit, pre-push, PR, CI (macOS/Windows/Linux), nightly, release, and hotfix workflows. Optimize for fast feedback (≤5 min PR median), determinism, parallelization, and cost-consciousness while maintaining comprehensive coverage. Deliverable: Constitution amendment recommendations codifying test taxonomy, triggers, time budgets, flake policies, platform coverage, and ownership.

## Technical Context

**Language/Version**: Markdown, YAML (GitHub Actions), Shell (Bash/Zsh for local hooks)

**Primary Dependencies**: 
- GitHub Actions (CI/CD platform - ubuntu-latest, macos-latest, windows-latest runners)
- JUCE UnitTest framework (existing from 004-tdd-adoption)
- CTest (CMake test runner)
- clang-format, clang-tidy (static analysis/formatting)
- Git hooks (pre-commit, pre-push automation)
- GitHub CodeQL, Dependabot (security scanning - free for public repos)
- FOSSA or license-checker scripts (license compliance)

**Storage**: N/A (policy/governance documentation; test results in CI artifacts)

**Testing**: Meta-level (this feature defines testing strategy itself); validation via policy compliance checks

**Target Platform**: Cross-platform CI/CD (GitHub Actions runners: ubuntu-latest, macos-latest, windows-latest)

**Project Type**: Governance/process framework (produces documentation artifacts and CI/CD workflow updates)

**Performance Goals**: 
- PR check median ≤ 5 minutes
- PR check p95 ≤ 10 minutes
- Nightly full suite ≤ 60 minutes (change detection optimized)
- Re-run rate due to non-determinism ≤ 2%

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
├── plan-final.md                            # This file (consolidated from runs 1-3)
├── research-final.md                        # Phase 0: Technology/pattern decisions (consolidated)
├── data-model-final.md                      # Phase 1: Entities, relationships, validation (consolidated)
├── quickstart-final.md                      # Phase 1: Developer onboarding guide (consolidated)
├── contracts/
│   ├── testing-governance-schema.md         # YAML/JSON schemas for config/tracking
│   ├── checks.schema.json                   # Check category validation schema
│   └── policy.schema.json                   # Policy validation schema
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
| N/A | No constitutional violations | N/A |

---

## Phase 0: Outline & Research

**Objective**: Document all technology and pattern decisions with rationale and alternatives.

**Status**: ✅ **COMPLETE** — All decisions documented in `research-final.md`

### Key Decisions (Summary)

1. **Check Taxonomy & Tooling**: 12 categories with specific tools per category
2. **Trigger Strategy**: Local hooks + GitHub Actions with tiered coverage
3. **Flake Detection**: Automated 3/10 threshold with quarantine workflow
4. **Platform Coverage**: Tiered (PR macOS primary, nightly all platforms, release full)
5. **Time Budget Enforcement**: Fail-fast + parallel execution + caching
6. **Skip/Bypass Governance**: Documented exception process with audit trail
7. **Constitution Amendment**: New Section VI in constitution.md

**Deliverable**: ✅ `research-final.md` (consolidated from runs 1-3)

---

## Phase 1: Design & Contracts

**Objective**: Define data model, contracts, and developer quickstart guide.

**Status**: ✅ **COMPLETE** — All artifacts generated

### 1. Data Model

**Deliverable**: ✅ `data-model-final.md`

**Entities Defined**:
- `CheckCategory`: Test/check types with ownership, duration, blocking policy
- `TriggerContext`: Events/environments where checks execute
- `Policy`: Governance rules (flake, skip, retry, caching)
- `Owner`: Team/area accountability with SLAs
- `TestRun`: Single execution record with results
- `QuarantineRecord`: Flaky check tracking and remediation

**Relationships**: Documented with validation rules and state transitions

### 2. API Contracts

**Deliverable**: ✅ `contracts/` directory

**Files**:
- `testing-governance-schema.md`: Human-readable schema documentation
- `checks.schema.json`: JSON schema for check category validation
- `policy.schema.json`: JSON schema for policy validation

### 3. Quickstart Guide

**Deliverable**: ✅ `quickstart-final.md`

**Contents**:
- Local developer workflow (setup, TDD loop, what runs when)
- PR validation expectations for reviewers
- Release/hotfix workflow for release managers
- Flake management for maintainers
- Configuration file editing
- Common scenarios and troubleshooting

### 4. Agent Context Update

**Action Required**: Run agent context update script

```bash
cd /Users/peternicholls/Dev/ShowMIDI.git
.specify/scripts/bash/update-agent-context.sh copilot
```

**Technologies to Add**:
- GitHub Actions (workflow definitions)
- YAML (configuration)
- Shell scripting (Bash/Zsh hooks)
- clang-tidy, clang-format (existing but ensure documented)
- CodeQL, Dependabot (security scanning)
- FOSSA (license compliance)

---

## Constitution Check (Post-Design)

*Re-evaluation after Phase 1 artifacts generated*

| Principle | Post-Design Assessment | Status |
|-----------|------------------------|--------|
| **Multi-Platform Architecture** | Data model includes platform coverage policies; quickstart documents nightly/release full platform validation | ✅ Pass |
| **JUCE Framework Standards** | No changes to JUCE usage patterns; testing infrastructure already JUCE-compliant | ✅ Pass |
| **Real-Time Performance** | Performance tests categorized separately; run in nightly to avoid PR blocking; preserves TDD velocity | ✅ Pass |
| **User-Centric Design** | Fast feedback (5 min median) and flake governance protect developer UX; quickstart enhances onboarding | ✅ Pass |
| **Maintainability** | Ownership, SLAs, schemas, and quickstart all improve long-term maintainability; constitution amendment codifies practices | ✅ Pass |
| **Development Workflow (GitFlow)** | Trigger contexts and gates align with GitFlow branches; progressive depth matches risk profile | ✅ Pass |
| **CI/CD Integration** | Workflow updates extend existing ci.yml; new nightly/release workflows follow established patterns | ✅ Pass |
| **Quality Gates** | Blocking/advisory classification balances risk and velocity; staging gates prevent regressions | ✅ Pass |

**Overall**: ✅ **PASS** — Post-design evaluation confirms no constitutional violations. All artifacts strengthen constitutional compliance.

---

## Phase 2: Task Breakdown

**Status**: ⏸️ **PENDING** — Use `/speckit.tasks` command to generate actionable implementation tasks

**Scope**:
- Git hook scripts (pre-commit, pre-push)
- GitHub Actions workflow updates (ci.yml, nightly.yml, release.yml, flake-detector.yml)
- Configuration file (`testing-governance.yaml`)
- Validation scripts (`validate-testing-config.sh`, `install-hooks.sh`)
- Constitution amendment (Section VI)
- Documentation updates (CONTRIBUTING.md, README.md)

**Command**: `@workspace /speckit.tasks` (to be run after plan approval)

---

## Deliverables Summary

### Phase 0 (Research)
✅ `research-final.md` — Technology decisions, rationale, alternatives

### Phase 1 (Design)
✅ `data-model-final.md` — Entities, relationships, validation rules
✅ `contracts/testing-governance-schema.md` — Schema documentation
✅ `contracts/checks.schema.json` — Check category JSON schema
✅ `contracts/policy.schema.json` — Policy JSON schema
✅ `quickstart-final.md` — Developer/maintainer guide

### Phase 2 (Implementation Tasks)
⏸️ `tasks.md` — Generated via `/speckit.tasks` command

### Constitution Amendment
⏸️ `constitution.md` Section VI — To be added after implementation

---

## Next Steps

1. **Review Consolidated Plan**: Approve final plan before proceeding to task breakdown
2. **Generate Tasks**: Run `/speckit.tasks` to create implementation task list
3. **Pilot Phase**: Implement one check category (unit tests) to validate workflow
4. **Incremental Rollout**: Add categories progressively (nightly → PR)
5. **Constitution Ratification**: Amend constitution.md after successful pilot

---

## Notes

- **Run 1 Contributions**: Comprehensive research, detailed data model, thorough quickstart, flake workflow
- **Run 2 Contributions**: Explicit clarification tracking, phased prerequisites, schema validation emphasis
- **Run 3 Contributions**: Concise summaries, clean data model structure, focused quickstart

**Consolidation Approach**: Run 1 provided the most complete foundation. Run 2's clarification tracking was integrated into Technical Context. Run 3's conciseness informed summary and decision documentation.
