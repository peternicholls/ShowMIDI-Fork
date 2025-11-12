# Feature Specification: Phase 11 — Testing Cadence & Governance

**Feature Branch**: `005-test-cadence`  
**Created**: 2025-11-12  
**Status**: Draft  
**Input**: User description: "Extend specification with Phase 11: testing cadence and governance. Define when and how to run unit, integration, system, performance, UI/visual regression, static analysis, formatting, linting, security (SCA/SAST), license checks, build verification, packaging, and installer smoke tests. Align with TDD: local dev write-then-run. Define triggers and environments: pre-commit, pre-push, PR, CI (macOS, Windows, Linux), nightly/cron, release, hotfix. Optimize for fast feedback (under 5 min locally/PR), deterministic, parallelizable, and cost-conscious. Provide constitutional rationale for testing and propose explicit amendments to Constitution regarding testing, checks, and builds as a result. Be comprehensive but not excessive; define thresholds, time budgets, flake policies, quarantine workflow, and ownership. Output recommendations to constitution as a deliverable from Phase 11."

## User Scenarios & Testing *(mandatory)*

<!--
  IMPORTANT: User stories should be PRIORITIZED as user journeys ordered by importance.
  Each user story/journey must be INDEPENDENTLY TESTABLE - meaning if you implement just ONE of them,
  you should still have a viable MVP (Minimum Viable Product) that delivers value.
  
  Assign priorities (P1, P2, P3, etc.) to each story, where P1 is the most critical.
  Think of each story as a standalone slice of functionality that can be:
  - Developed independently
  - Tested independently
  - Deployed independently
  - Demonstrated to users independently
-->

### User Story 1 - Fast, Reliable PR Feedback (Priority: P1)

Developers receive clear, deterministic pass/fail results for required checks within minutes on every change, enabling a tight TDD loop without waiting long or rerunning flaky pipelines.

**Why this priority**: Rapid feedback protects productivity and quality; slow or noisy signals cause rework and delays.

**Independent Test**: Time a representative pull request and verify required checks complete within defined budgets with zero flakes.

**Acceptance Scenarios**:

1. **Given** a pull request with code changes, **When** required checks run automatically, **Then** results are available within the stated time budget and are deterministic (no re-runs needed).
2. **Given** a failing check, **When** the developer pushes a fix, **Then** the updated checks reflect only the latest commit and complete within the time budget.

---

### User Story 2 - Cross-Platform Confidence (Priority: P1)

Reviewers and maintainers see that change validation covers supported platforms (desktop OS targets) with a clear summary of what ran and why, avoiding redundant or excessive executions.

**Why this priority**: The product serves multiple platforms; parity is essential to prevent platform-specific regressions.

**Independent Test**: Verify checks include platform coverage appropriate for PRs and extended coverage in nightly and release runs.

**Acceptance Scenarios**:

1. **Given** a PR touching core logic, **When** validation runs, **Then** platform-appropriate checks are executed and summarized in a single, understandable report.
2. **Given** a change impacting platform-specific code, **When** validation runs, **Then** platform-specific checks are included without slowing unrelated PRs.

---

### User Story 3 - Nightly Depth, PR Lightness (Priority: P2)

The team runs deeper, slower suites (system, performance, packaging, installer smoke) on scheduled runs, keeping PR pipelines lean while still catching issues before releases.

**Why this priority**: Balances comprehensive coverage with developer velocity; prevents slow checks from blocking everyday work.

**Independent Test**: Confirm nightly runs include extended suites and that PR runs exclude them except when explicitly required.

**Acceptance Scenarios**:

1. **Given** it is a scheduled nightly run, **When** validation executes, **Then** extended suites run and publish comprehensive reports.
2. **Given** a regular PR, **When** validation executes, **Then** only fast suites run by default within the time budget.

---

### User Story 4 - Flake Governance and Ownership (Priority: P2)

Flaky checks are automatically detected, quarantined from required gates, and assigned to owners with SLAs, avoiding developer disruption while preserving signal quality.

**Why this priority**: Flakes degrade trust and waste time; governance restores clarity.

**Independent Test**: Introduce a known intermittent test and verify quarantine workflow, owner assignment, and SLA tracking.

**Acceptance Scenarios**:

1. **Given** a check is detected as flaky, **When** it crosses flake thresholds, **Then** it is removed from required gates, flagged, and assigned to an owner with due date.
2. **Given** a quarantined check, **When** it is fixed and stabilized, **Then** it returns to required gates after sustained pass rate.

---

### User Story 5 - Release and Hotfix Assurance (Priority: P3)

Release/hotfix flows apply expanded validation and packaging checks so that artifacts are production-ready with minimal post-release surprises.

**Why this priority**: Production stability relies on predictable, verifiable outputs.

**Independent Test**: Simulate a release cut and verify all required pre-release gates, packaging checks, and smoke validations pass.

**Acceptance Scenarios**:

1. **Given** a release branch, **When** release validation is triggered, **Then** required extended checks and packaging validations complete successfully before publishing artifacts.
2. **Given** a hotfix branch, **When** validation runs, **Then** safety-critical suites execute while maintaining fast turnaround.

---

[Add more user stories as needed, each with an assigned priority]

### Edge Cases

- A check exceeds its time budget but eventually passes (policy on fail-fast vs allow-complete)
- A test is flaky on one platform but stable on others (platform-specific quarantine)
- External dependency intermittently fails (retry policy vs marking as non-deterministic)
- PR from a fork with limited permissions (handling for required checks and status reporting)
- Long-running performance test impacts CI capacity (move to scheduled window)
- Skip directives are abused (justification and approval policy)
- Security/license finding appears near release (escalation and blocking rules)
- Multi-feature integration requires coordinated merge (staging workflow and regression validation)
- Feature breaks existing tests during multi-feature integration (temporary exemption policy and tracking)

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001 (Taxonomy)**: Define a clear taxonomy and purpose for checks: unit, integration, system, performance, UI/visual-regression, static analysis, formatting, linting, security scans, license compliance, build verification, packaging, and installer smoke.
- **FR-002 (Triggers & Cadence)**: Define when each category runs: local on-demand (developer-initiated full suite or filtered), pre-commit (formatting and linting with auto-fix where possible), pre-push (unit tests to catch logic errors before remote push), pull request (unit, integration, static analysis, build verification, code quality per FR-006), on-merge to develop (full required suite re-validation), scheduled nightly (extended suites including system, performance, UI/visual regression), release branches (add packaging and installer smoke), and hotfix (safety-critical subset for fast turnaround).
- **FR-003 (Time Budgets)**: Establish time budgets: required PR checks median ≤ 5 minutes, 95th percentile ≤ 10 minutes; deeper suites run outside PR unless explicitly required.
- **FR-004 (Signal Quality)**: Require checks to be deterministic; specify policies for environment isolation and stable inputs (e.g., seeded randomness) to avoid nondeterminism.
- **FR-005 (Flake Policy)**: Define flake detection thresholds, quarantine workflow, owner assignment, and SLAs for resolution and re-enablement. A check is flagged as flaky if it exhibits 3 failures in 10 runs OR 20% failure rate over a 7-day rolling window. Upon detection, the check is automatically quarantined (removed from required gates), flagged in reporting, and assigned to a designated owner with remediation SLA. Re-enablement requires sustained pass rate (≥95% over 14 days) and owner sign-off.
- **FR-006 (Blocking vs Advisory)**: Classify checks as blocking or advisory by risk and branch type. All merges/PRs MUST pass through a release staging gate before acceptance. Default policy for feature PRs: block on unit, integration (core flows), static analysis, formatting/linting, and build verification; advisory for UI/visual regression and SAST/SCA/license except High/Critical severity findings which block; performance/packaging/installer excluded from feature PR. Features MUST NOT break existing code (regression gate). Exception: documented multi-feature integration scenarios requiring coordinated merges. Progressive depth: develop merges require full unit+integration pass; release staging adds system tests and performance baselines; release branches add packaging and installer smoke as blocking; main branch requires all gates green including final smoke tests.
- **FR-007 (Platform Coverage)**: Specify platform coverage expectations for PR vs nightly/release to balance velocity and confidence. PR checks run on primary platform (macOS) plus platform-specific validation when changes touch platform-conditional code. Nightly scheduled runs execute comprehensive suites on all supported platforms (macOS, Windows, Linux) but skip execution if no code changes detected since last successful run. Release branches require full platform validation regardless of change detection.
- **FR-008 (Ownership)**: Map checks to accountable owners/areas with contact and escalation paths.
- **FR-009 (Reporting & Artifacts)**: Provide an aggregated, human-readable summary per run, with links to detailed logs and artifacts where applicable.
- **FR-010 (Skip/Bypass Governance)**: Require documented justification and approval for bypassing or skipping checks; record and report such events.
- **FR-011 (Capacity & Cost)**: Define policies to keep the pipeline efficient and cost-conscious (parallelize where sensible; schedule heavy suites appropriately). Leverage change detection: skip nightly runs if no code changes since last successful execution. Public repository benefit: utilize unlimited GitHub Actions minutes for comprehensive coverage. Optimize for determinism and parallelization over runtime constraints where possible.
- **FR-012 (Release/Hotfix Gates)**: Define expanded gates for release/hotfix including packaging and installer smoke validations before distribution.
- **FR-013 (Constitutional Rationale)**: State the constitutional rationale for testing: protect user experience, multi-platform reliability, real-time performance, and maintainability; document how the cadence supports these goals.
- **FR-014 (Constitution Recommendations Output)**: Produce explicit recommendations to update the Constitution regarding testing, checks, and builds as a deliverable of this phase.
- **FR-015 (Non-Excess Criterion)**: Codify guardrails to remain comprehensive but not excessive (e.g., limit required PR suites to essential fast checks; move slow suites to scheduled runs).
- **FR-016 (Failure Handling)**: Define consistent policies for reruns, retries, and failure triage to minimize noise and rework.

### Key Entities *(include if feature involves data)*

- **Check Category**: Name, purpose, blocking/advisory, expected duration, owner
- **Trigger**: Event (pre-commit, PR, nightly, release), scope, time budget
- **Policy**: Rules for flake handling, skip governance, escalation, reporting
- **Owner**: Area/team, contact, SLA, responsibilities

## Success Criteria *(mandatory)*

<!--
  ACTION REQUIRED: Define measurable success criteria.
  These must be technology-agnostic and measurable.
-->

### Measurable Outcomes

- **SC-001 (PR Velocity)**: Required PR checks complete with median ≤ 5 minutes and 95th percentile ≤ 10 minutes for typical changes.
- **SC-002 (Determinism)**: Re-run rate due to non-determinism ≤ 2% across all required checks.
- **SC-003 (Flake Governance)**: 0 quarantined checks older than 7 days without an assigned owner and remediation plan; median time-to-fix flaky checks ≤ 2 business days.
- **SC-004 (Coverage Balance)**: Nightly runs include extended suites, achieving ≥ 90% coverage of risk areas not covered in PR runs; release gates report 100% completion before artifact distribution.
- **SC-005 (Cross-Platform Parity)**: Required checks have equivalent pass rates across supported platforms, with no platform more than 3 percentage points worse than the best.
- **SC-006 (Security/License Responsiveness)**: High-severity compliance issues detected within 24 hours and median remediation in ≤ 5 business days.

---

## Clarifications

### Session 2025-11-12

- Q: What is the default PR blocking policy (strict/balanced/minimal) and are there branch-specific overrides? → A: Balanced, but branch specific
- Q: Must all merges/PRs pass through a staging gate before acceptance? What testing depth is required as code approaches release? → A: All merges/PRs go via release staging before acceptance. Features must pass unit, integration, and code quality (cannot break existing code). Exception: multi-feature integration requiring further work. Testing depth increases closer to release.
- Q: What threshold triggers flake detection and quarantine? → A: Option B - 3 failures in 10 runs or 20% failure rate over 7 days
- Q: What checks run pre-commit vs pre-push locally? → A: Option B - Pre-commit: format+lint (auto-fix); pre-push: unit tests
- Q: What is the nightly performance test platform scope? → A: Mix of comprehensive and balanced - run on all platforms (macOS, Windows, Linux) nightly; skip if no code changes since last run (change detection optimization)

### Constitutional Rationale (Context)

Testing exists to uphold the product's core principles: multi-platform reliability, real-time performance, user-centric quality, and maintainability. This cadence prioritizes fast, trustworthy signals for everyday work, deep assurance on a schedule, and strict gates when risk is highest (release), ensuring we are comprehensive without becoming excessive.

### Deliverable: Constitution Recommendations (Outcome of Phase 11)

This phase will produce concrete recommendations to amend the Constitution to codify:
- Test taxonomy and their purposes
- Trigger/cadence expectations and time budgets
- Flake governance and skip/bypass policies
- Platform coverage standards for PR vs scheduled vs release
- Roles and ownership expectations for checks and gates
