# Tasks: Phase 11 — Testing Cadence & Governance

**Feature Branch**: `005-test-cadence`  
**Input**: Design documents from `/specs/005-test-cadence/`  
**Prerequisites**: plan.md ✅, spec.md ✅, research.md ✅, data-model.md ✅, contracts/ ✅

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

---

## Format: `- [ ] [ID] [P?] [Story?] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- All tasks include exact file paths

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and basic structure for testing governance framework

- [X] T001 Create scripts directory structure at `scripts/hooks/` and `scripts/validation/`
- [X] T002 [P] Create contracts directory at `.github/testing-governance/` for deployment
- [X] T003 [P] Add schema validation dependencies to project (yq, jq for YAML/JSON parsing)

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core testing infrastructure that MUST be complete before ANY user story implementation

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

- [X] T004 Create primary configuration file at `.github/testing-governance.yaml` with version and empty sections
- [X] T005 [P] Create flake tracking artifact schema in `scripts/validation/flake-tracking.schema.json`
- [X] T006 [P] Create test run summary artifact schema in `scripts/validation/test-run-summary.schema.json`
- [X] T007 Implement schema validation script at `scripts/validation/validate-testing-config.sh`
- [X] T008 [P] Define all 12 CheckCategory entities in `.github/testing-governance.yaml` per research.md decisions
- [X] T009 [P] Define Owner entities (core-team, platform-macos, platform-windows, platform-linux, security-team) in `.github/testing-governance.yaml`
- [X] T010 Create Git hook installation script at `scripts/install-hooks.sh`

**Checkpoint**: Foundation ready - user story implementation can now begin in parallel

---

## Phase 3: User Story 1 - Fast, Reliable PR Feedback (Priority: P1) 🎯 MVP

**Goal**: Developers receive clear, deterministic pass/fail results for required checks within 5 minutes on every PR, enabling tight TDD loop without waiting or rerunning flaky pipelines

**Independent Test**: Create a sample PR with code changes, verify required checks complete within 5 min median (10 min p95) with zero flakes

### Configuration for User Story 1

- [X] T011 [P] [US1] Define pre-commit TriggerContext in `.github/testing-governance.yaml` with formatting and linting checks
- [X] T012 [P] [US1] Define pre-push TriggerContext in `.github/testing-governance.yaml` with unit tests
- [X] T013 [P] [US1] Define pr-to-develop TriggerContext in `.github/testing-governance.yaml` with time budgets (300s median, 600s p95)
- [X] T014 [US1] Add enabled_checks list to pr-to-develop context: unit-tests, integration-tests, static-analysis, formatting, linting, build-verification

### Git Hooks for User Story 1

- [X] T015 [P] [US1] Create pre-commit hook script at `scripts/hooks/pre-commit` with clang-format auto-fix and GPL header check
- [X] T016 [P] [US1] Create pre-push hook script at `scripts/hooks/pre-push` with CTest unit test execution
- [X] T017 [US1] Update install-hooks.sh to copy pre-commit and pre-push scripts to `.git/hooks/` with execute permissions

### GitHub Actions Workflow Updates for User Story 1

- [ ] T018 [US1] Create workflow config loader job in `.github/workflows/ci.yml` to parse testing-governance.yaml
- [ ] T019 [P] [US1] Update unit-tests job in `.github/workflows/ci.yml` to read time budget from config
- [ ] T020 [P] [US1] Update integration-tests job in `.github/workflows/ci.yml` to read time budget from config
- [ ] T021 [US1] Add fail-fast strategy to PR jobs in `.github/workflows/ci.yml` per research.md Decision 5
- [ ] T022 [US1] Add parallelization matrix to PR jobs in `.github/workflows/ci.yml` (macOS primary platform)
- [ ] T023 [US1] Implement timeout enforcement in PR jobs using timeout_sec from CheckCategory configs

### Policies for User Story 1

- [X] T024 [P] [US1] Define retry-on-infrastructure-failure Policy in `.github/testing-governance.yaml` with max_retries: 2
- [X] T025 [P] [US1] Define caching-strategy Policy in `.github/testing-governance.yaml` for CMake ccache and dependencies
- [ ] T026 [US1] Implement cache configuration in `.github/workflows/ci.yml` using actions/cache for JUCE builds

**Checkpoint**: At this point, PR validation should run with deterministic checks completing within 5 min median

---

## Phase 4: User Story 2 - Cross-Platform Confidence (Priority: P1)

**Goal**: Reviewers and maintainers see that change validation covers supported platforms with clear summary of what ran and why, avoiding redundant executions

**Independent Test**: Create PR touching platform-specific code, verify platform-appropriate checks execute and summarize results clearly

### Platform Coverage Configuration for User Story 2

- [ ] T027 [P] [US2] Add platform matrix to pr-to-develop TriggerContext in `.github/testing-governance.yaml` (macos primary + conditional)
- [ ] T028 [P] [US2] Define platform-specific check conditions in `.github/testing-governance.yaml` using condition_expression
- [ ] T029 [US2] Update build-verification CheckCategory in `.github/testing-governance.yaml` to specify platform coverage requirements

### Workflow Updates for User Story 2

- [ ] T030 [US2] Implement platform detection logic in `.github/workflows/ci.yml` to analyze changed files for platform-specific code
- [ ] T031 [P] [US2] Add conditional platform-specific jobs to `.github/workflows/ci.yml` (Windows, Linux) triggered by path filters
- [ ] T032 [US2] Create aggregated status check reporter job in `.github/workflows/ci.yml` to summarize platform coverage

### Reporting for User Story 2

- [ ] T033 [P] [US2] Implement test-run-summary.json generation script at `scripts/reporting/generate-run-summary.sh`
- [ ] T034 [US2] Add artifact upload step to `.github/workflows/ci.yml` to publish test-run-summary.json per FR-009
- [ ] T035 [US2] Create PR comment bot integration in `.github/workflows/ci.yml` to post human-readable summary with platform coverage

**Checkpoint**: At this point, PRs should show clear platform coverage with appropriate checks per platform

---

## Phase 5: User Story 3 - Nightly Depth, PR Lightness (Priority: P2)

**Goal**: Team runs deeper, slower suites (system, performance, packaging, installer smoke) on scheduled runs, keeping PR pipelines lean while catching issues before releases

**Independent Test**: Verify nightly runs include extended suites (system, performance) and that regular PRs exclude them, maintaining <10 min p95

### Nightly Configuration for User Story 3

- [ ] T036 [P] [US3] Define nightly-develop TriggerContext in `.github/testing-governance.yaml` with 3600s time budget
- [ ] T037 [P] [US3] Add system-tests, performance-tests, ui-visual-regression CheckCategories to nightly enabled_checks
- [ ] T038 [US3] Enable change_detection_enabled: true for nightly-develop context per research.md Decision 2

### Nightly Workflow for User Story 3

- [ ] T039 [US3] Create new workflow file at `.github/workflows/nightly.yml` with cron schedule (0 2 * * *)
- [ ] T040 [P] [US3] Add change detection job to `.github/workflows/nightly.yml` using git diff to skip if no changes
- [ ] T041 [P] [US3] Add system-tests job to `.github/workflows/nightly.yml` with headless UI testing
- [ ] T042 [P] [US3] Add performance-tests job to `.github/workflows/nightly.yml` with latency benchmarks (<10ms validation)
- [ ] T043 [US3] Configure full platform matrix (macos, windows, linux) for `.github/workflows/nightly.yml`

### Extended Check Categories for User Story 3

- [ ] T044 [P] [US3] Define system-tests CheckCategory in `.github/testing-governance.yaml` with purpose and tools
- [ ] T045 [P] [US3] Define performance-tests CheckCategory in `.github/testing-governance.yaml` with JUCE high-resolution timer
- [ ] T046 [P] [US3] Define ui-visual-regression CheckCategory in `.github/testing-governance.yaml` (advisory, screenshot comparison)

**Checkpoint**: At this point, nightly runs should execute extended suites while PRs remain fast (<10 min)

---

## Phase 6: User Story 4 - Flake Governance and Ownership (Priority: P2)

**Goal**: Flaky checks are automatically detected, quarantined from required gates, and assigned to owners with SLAs, avoiding developer disruption while preserving signal quality

**Independent Test**: Introduce known intermittent test, verify quarantine workflow triggers, owner assignment occurs, and SLA tracking activates

### Flake Detection Configuration for User Story 4

- [ ] T047 [P] [US4] Define flake-quarantine Policy in `.github/testing-governance.yaml` with 3/10 threshold and 20% 7-day rate
- [ ] T048 [P] [US4] Add re_enable_criteria to flake-quarantine Policy (95% pass rate over 14 days, owner signoff required)
- [ ] T049 [US4] Set sla_hours: 48 for flake-quarantine Policy per research.md Decision 3

### Flake Detection Workflow for User Story 4

- [ ] T050 [US4] Create flake detector workflow at `.github/workflows/flake-detector.yml` triggered on workflow_run completion
- [ ] T051 [P] [US4] Implement flake analysis script at `scripts/validation/analyze-flakes.sh` to parse flake-tracking.json history
- [ ] T052 [P] [US4] Implement quarantine script at `scripts/validation/quarantine-check.sh` to remove check from required gates
- [ ] T053 [US4] Add flake-tracking.json download and update logic to `.github/workflows/flake-detector.yml`
- [ ] T054 [US4] Implement owner assignment logic in `scripts/validation/quarantine-check.sh` using CODEOWNERS or Owner mapping

### Flake Management Automation for User Story 4

- [ ] T055 [P] [US4] Create GitHub issue template at `.github/ISSUE_TEMPLATE/flaky-test.md` for automated flake reports
- [ ] T056 [US4] Add issue creation step to `.github/workflows/flake-detector.yml` with owner assignment and SLA deadline
- [ ] T057 [P] [US4] Implement re-enablement validation script at `scripts/validation/validate-re-enablement.sh` checking 14-day pass rate
- [ ] T058 [US4] Add artifact upload for updated flake-tracking.json to `.github/workflows/flake-detector.yml`

### Documentation for User Story 4

- [ ] T059 [P] [US4] Update CONTRIBUTING.md with flake governance section referencing quickstart.md
- [ ] T060 [US4] Add flake management workflow diagram to `specs/005-test-cadence/docs/flake-workflow.md`

**Checkpoint**: At this point, flaky tests should auto-quarantine with owner assignment and SLA tracking

---

## Phase 7: User Story 5 - Release and Hotfix Assurance (Priority: P3)

**Goal**: Release/hotfix flows apply expanded validation and packaging checks so artifacts are production-ready with minimal post-release surprises

**Independent Test**: Simulate release branch creation, verify all required pre-release gates, packaging checks, and smoke validations pass

### Release Configuration for User Story 5

- [ ] T061 [P] [US5] Define release-gate TriggerContext in `.github/testing-governance.yaml` with 5400s time budget (90 min)
- [ ] T062 [P] [US5] Define hotfix-gate TriggerContext in `.github/testing-governance.yaml` with 1200s time budget (20 min)
- [ ] T063 [US5] Add packaging-validation and installer-smoke CheckCategories to release-gate enabled_checks
- [ ] T064 [US5] Define safety-critical subset for hotfix-gate enabled_checks (unit, integration, build, High+ security)

### Release Workflow for User Story 5

- [ ] T065 [US5] Create release workflow at `.github/workflows/release.yml` triggered on push to release/* branches
- [ ] T066 [P] [US5] Add packaging-validation job to `.github/workflows/release.yml` for DMG, EXE, RPM/DEB artifacts
- [ ] T067 [P] [US5] Add installer-smoke job to `.github/workflows/release.yml` (install → launch → uninstall tests)
- [ ] T068 [US5] Configure full platform validation (no change detection) for `.github/workflows/release.yml`
- [ ] T069 [US5] Add all plugin format validation (VST/VST3/AU/LV2/CLAP) to `.github/workflows/release.yml`

### Hotfix Workflow for User Story 5

- [ ] T070 [US5] Create hotfix workflow at `.github/workflows/hotfix.yml` triggered on push to hotfix/* branches
- [ ] T071 [P] [US5] Add fast-track unit and integration jobs to `.github/workflows/hotfix.yml` with reduced scope
- [ ] T072 [US5] Configure security scan job (High/Critical severity only) in `.github/workflows/hotfix.yml`
- [ ] T073 [US5] Implement fast turnaround optimizations (aggressive caching, parallel execution) in `.github/workflows/hotfix.yml`

### Check Categories for User Story 5

- [ ] T074 [P] [US5] Define packaging-validation CheckCategory in `.github/testing-governance.yaml` with platform-specific artifact tools
- [ ] T075 [P] [US5] Define installer-smoke CheckCategory in `.github/testing-governance.yaml` with install/uninstall test scripts
- [ ] T076 [US5] Add release-manager owner to `.github/testing-governance.yaml` with ownership of packaging and installer checks

**Checkpoint**: At this point, release and hotfix branches should have appropriate expanded/fast-track validation

---

## Phase 8: Security, License, and Static Analysis (Cross-Cutting)

**Purpose**: Implement security scanning, license compliance, and static analysis checks across all trigger contexts

### Security Check Categories

- [ ] T077 [P] Define security-sast CheckCategory in `.github/testing-governance.yaml` using GitHub CodeQL
- [ ] T078 [P] Define security-sca CheckCategory in `.github/testing-governance.yaml` using Dependabot/OWASP Dependency-Check
- [ ] T079 [P] Define license-compliance CheckCategory in `.github/testing-governance.yaml` using FOSSA or license-checker
- [ ] T080 Define security-team Owner in `.github/testing-governance.yaml` with ownership of security and license checks

### Static Analysis Configuration

- [ ] T081 [P] Define static-analysis CheckCategory in `.github/testing-governance.yaml` using clang-tidy and cppcheck
- [ ] T082 [P] Define formatting CheckCategory in `.github/testing-governance.yaml` using existing clang-format config
- [ ] T083 [P] Define linting CheckCategory in `.github/testing-governance.yaml` with GPL header validation

### Workflow Integration

- [ ] T084 Create CodeQL workflow at `.github/workflows/codeql.yml` for SAST scanning
- [ ] T085 [P] Add CodeQL job to pr-to-develop trigger context in `.github/workflows/ci.yml` (High/Critical blocking)
- [ ] T086 [P] Configure Dependabot in `.github/dependabot.yml` for SCA scanning
- [ ] T087 Add license compliance check script at `scripts/validation/check-licenses.sh` using FOSSA API or local scan
- [ ] T088 [P] Add static analysis job to `.github/workflows/ci.yml` with clang-tidy and cppcheck
- [ ] T089 Integrate formatting and linting checks into pre-commit hook (already in T015)

**Checkpoint**: At this point, security, license, and static analysis checks should run in appropriate contexts

---

## Phase 9: Skip/Bypass Governance and Audit Trail (Cross-Cutting)

**Purpose**: Implement documented exception process with approval and audit trail per FR-010

### Skip Policy Configuration

- [ ] T090 [P] Define skip-approval-required Policy in `.github/testing-governance.yaml` with approver list
- [ ] T091 [P] Add justification_template to skip-approval-required Policy per research.md Decision 6
- [ ] T092 Define audit_log_retention_days: 365 in skip-approval-required Policy

### Approval Workflow

- [ ] T093 Create skip request issue template at `.github/ISSUE_TEMPLATE/skip-check-request.md` with required fields
- [ ] T094 [P] Implement skip approval validation script at `scripts/validation/validate-skip-approval.sh`
- [ ] T095 Add skip approval check to PR validation in `.github/workflows/ci.yml` (scan for skip comment, verify approver)

### Audit Trail

- [ ] T096 [P] Create audit log schema at `scripts/validation/skip-audit.schema.json`
- [ ] T097 Implement audit logging in `scripts/validation/validate-skip-approval.sh` to append to skip-audit.json artifact
- [ ] T098 [P] Create monthly audit report script at `scripts/reporting/generate-skip-audit-report.sh`
- [ ] T099 Add scheduled workflow at `.github/workflows/monthly-audit.yml` to generate and publish skip audit reports

**Checkpoint**: At this point, skip/bypass requests should require documented approval with full audit trail

---

## Phase 10: Constitution Amendment and Documentation (Cross-Cutting)

**Purpose**: Produce constitutional amendment and comprehensive documentation per FR-013 and FR-014

### Constitution Amendment

- [ ] T100 Create draft amendment document at `specs/005-test-cadence/constitution-amendment-draft.md` with Section VI structure
- [ ] T101 [P] Document test taxonomy in Section VI per research.md Decision 1 (12 categories with purposes)
- [ ] T102 [P] Document trigger cadence in Section VI per research.md Decision 2 (local, PR, nightly, release, hotfix)
- [ ] T103 [P] Document governance principles in Section VI (flake detection, platform coverage, progressive gates)
- [ ] T104 Review amendment with maintainers and incorporate feedback into draft
- [ ] T105 Merge approved amendment into `.specify/memory/constitution.md` as new Section VI

### Documentation Updates

- [ ] T106 [P] Update README.md with testing overview section linking to quickstart.md
- [ ] T107 [P] Update CONTRIBUTING.md with complete testing guidelines (local setup, PR expectations, flake management)
- [ ] T108 [P] Create testing governance README at `.github/testing-governance/README.md` documenting configuration structure
- [ ] T109 [P] Document all CheckCategories with examples in `.github/testing-governance/CHECK_CATEGORIES.md`
- [ ] T110 [P] Document all Policies with enforcement details in `.github/testing-governance/POLICIES.md`
- [ ] T111 Create workflow architecture diagram at `specs/005-test-cadence/docs/workflow-architecture.md`

### Developer Onboarding

- [ ] T112 [P] Copy quickstart.md from specs to `docs/testing/` as canonical developer guide
- [ ] T113 Add testing setup checklist to CONTRIBUTING.md referencing hooks installation and config validation
- [ ] T114 Create troubleshooting guide at `docs/testing/TROUBLESHOOTING.md` with common scenarios from quickstart.md

**Checkpoint**: At this point, all documentation and constitutional amendments should be complete

---

## Phase 11: Polish & Validation (Final)

**Purpose**: Final integration testing, validation, and cleanup

### Integration Testing

- [ ] T115 Run full integration test: create sample PR, verify all US1 and US2 checks execute correctly
- [ ] T116 [P] Simulate flaky test scenario, verify US4 quarantine workflow triggers and owner assignment works
- [ ] T117 [P] Trigger nightly workflow manually, verify US3 extended suites run with change detection
- [ ] T118 Create test release branch, verify US5 packaging and installer checks execute
- [ ] T119 Test hotfix workflow with fast-track validation subset

### Configuration Validation

- [ ] T120 [P] Run schema validation on `.github/testing-governance.yaml` using `scripts/validation/validate-testing-config.sh`
- [ ] T121 [P] Validate all CheckCategory references are used in at least one TriggerContext
- [ ] T122 [P] Validate all Owner entities have at least one owned_check_category
- [ ] T123 Verify all time budgets meet constitutional requirements (PR p95 ≤ 10 min)

### Performance Validation

- [ ] T124 Measure PR check median and p95 duration over 10 sample PRs, verify ≤ 5 min / ≤ 10 min per SC-001
- [ ] T125 [P] Analyze nightly run duration with change detection, verify ≤ 60 min per SC-001
- [ ] T126 [P] Calculate re-run rate due to non-determinism over 30 days, verify ≤ 2% per SC-002

### Cleanup and Finalization

- [ ] T127 Remove sample/placeholder content from configuration files
- [ ] T128 [P] Add file header comments to all scripts with GPL-3.0 license per constitution
- [ ] T129 [P] Run clang-format on any C++ test fixtures or helpers added during implementation
- [ ] T130 Create pull request for 005-test-cadence feature with comprehensive description
- [ ] T131 Run quickstart.md validation: follow developer workflow end-to-end as documented
- [ ] T132 Update agent context with completed Phase 11 using `.specify/scripts/bash/update-agent-context.sh`

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Phase 1 completion - BLOCKS all user stories
- **User Stories (Phases 3-7)**: All depend on Foundational (Phase 2) completion
  - US1 and US2 can proceed in parallel (different trigger contexts)
  - US3 depends on US1 completion (extends PR infrastructure)
  - US4 can proceed in parallel with US1-US3 (separate workflow)
  - US5 can proceed in parallel with US1-US4 (separate workflows)
- **Cross-Cutting (Phases 8-9)**: Can proceed in parallel with user stories
- **Constitution (Phase 10)**: Depends on all user story phases being substantially complete
- **Polish (Phase 11)**: Depends on all previous phases completion

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundational - No dependencies on other stories
- **User Story 2 (P1)**: Can start after Foundational - Integrates with US1 but independently testable
- **User Story 3 (P2)**: Builds on US1 PR infrastructure but creates separate nightly workflow
- **User Story 4 (P2)**: Independent flake detection workflow, can run parallel to US1-US3
- **User Story 5 (P3)**: Independent release/hotfix workflows, can run parallel to US1-US4

### Within Each User Story

- Configuration tasks before workflow tasks
- Schema/policy definitions before implementation scripts
- Workflow updates before testing/validation
- Documentation after implementation

### Parallel Opportunities

- **Phase 1**: All 3 setup tasks can run in parallel
- **Phase 2**: T005-T006, T008-T009 can run in parallel after T004 completes
- **Phase 3 (US1)**: T011-T014 config tasks, T015-T016 hook scripts, T019-T020 workflow updates, T024-T025 policies
- **Phase 4 (US2)**: T027-T029 config tasks, T031 platform jobs, T033 reporting script
- **Phase 5 (US3)**: T036-T038 config tasks, T041-T042 test jobs, T044-T046 check definitions
- **Phase 6 (US4)**: T047-T049 policies, T051-T052 scripts, T055 template, T057 validation, T059 docs
- **Phase 7 (US5)**: T061-T064 config tasks, T066-T067 release jobs, T071-T072 hotfix jobs, T074-T075 checks
- **Phase 8**: All check category definitions (T077-T083) can run in parallel
- **Phase 9**: T090-T092 policies, T096-T098 audit infrastructure
- **Phase 10**: All documentation tasks (T101-T103, T106-T114) can run in parallel after T100
- **Phase 11**: Validation tasks T116-T118, T120-T122, T125-T126, T128-T129 can run in parallel

---

## Parallel Execution Example: User Story 1

```bash
# Configuration tasks (parallel):
Task: "Define pre-commit TriggerContext in .github/testing-governance.yaml"
Task: "Define pre-push TriggerContext in .github/testing-governance.yaml"
Task: "Define pr-to-develop TriggerContext in .github/testing-governance.yaml"

# Hook scripts (parallel):
Task: "Create pre-commit hook script at scripts/hooks/pre-commit"
Task: "Create pre-push hook script at scripts/hooks/pre-push"

# Workflow updates (parallel after config):
Task: "Update unit-tests job in .github/workflows/ci.yml"
Task: "Update integration-tests job in .github/workflows/ci.yml"

# Policies (parallel):
Task: "Define retry-on-infrastructure-failure Policy"
Task: "Define caching-strategy Policy"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup (T001-T003)
2. Complete Phase 2: Foundational (T004-T010) - CRITICAL blocker
3. Complete Phase 3: User Story 1 (T011-T026)
4. **STOP and VALIDATE**: Create sample PR, verify checks run in <5 min median
5. Demo to team, gather feedback before proceeding

### Incremental Delivery

1. **Foundation** (Phases 1-2) → Git hooks + config infrastructure ready
2. **MVP** (Phase 3: US1) → PR validation with fast feedback → Deploy/Demo
3. **Platform Coverage** (Phase 4: US2) → Multi-platform validation → Deploy/Demo
4. **Extended Suites** (Phase 5: US3) → Nightly comprehensive checks → Deploy/Demo
5. **Flake Management** (Phase 6: US4) → Auto-quarantine with SLAs → Deploy/Demo
6. **Release Assurance** (Phase 7: US5) → Production-ready validation → Deploy/Demo
7. **Security & Compliance** (Phase 8) → SAST/SCA/license checks → Deploy
8. **Governance** (Phase 9) → Skip approval and audit trail → Deploy
9. **Documentation** (Phase 10) → Constitutional amendment → Ratify
10. **Polish** (Phase 11) → Final validation and cleanup → Feature complete

### Parallel Team Strategy

With multiple developers after Foundational phase completes:

- **Developer A**: User Story 1 (PR feedback) - highest priority
- **Developer B**: User Story 2 (platform coverage) - can run parallel to US1
- **Developer C**: User Story 4 (flake governance) - independent workflow
- **Developer D**: Phase 8 (security/static analysis) - cross-cutting concerns

Stories integrate independently without blocking each other.

---

## Success Metrics

Track these metrics during implementation to validate success criteria:

- **SC-001 (PR Velocity)**: Median ≤ 5 min, p95 ≤ 10 min (measure after US1 complete)
- **SC-002 (Determinism)**: Re-run rate ≤ 2% (measure over 2 weeks after US1 complete)
- **SC-003 (Flake Governance)**: Quarantine SLA adherence (measure after US4 complete)
- **SC-004 (Coverage Balance)**: Nightly coverage vs PR coverage (measure after US3 complete)
- **SC-005 (Platform Parity)**: Pass rate variance across platforms (measure after US2 complete)
- **SC-006 (Security Response)**: Detection and remediation time (measure after Phase 8 complete)

---

## Notes

- All tasks follow strict checklist format: `- [ ] [TaskID] [P?] [Story?] Description with file path`
- [P] marker indicates parallelizable tasks (different files, no blocking dependencies)
- [Story] label (US1, US2, etc.) maps to user stories from spec.md
- Each user story is independently implementable and testable
- MVP scope is User Story 1 only (fast PR feedback)
- Progressive delivery: foundation → US1 → US2 → US3 → US4 → US5 → polish
- Total: 132 tasks across 11 phases
- Estimated effort: 3-4 weeks with 2-3 developers working in parallel
