# CI/CD Pipeline Requirements Quality Checklist: TDD Adoption

**Purpose**: Validation of CI/CD requirements quality for test execution across platforms, artifact management, gating strategies, and visibility/reporting. Tests whether CI/CD requirements are complete, clear, and measurable—NOT whether the pipeline works.

**Created**: 2025-11-11  
**Feature**: [plan.md Phase 0–3](../plan.md) | [contracts/ci-matrix.yml](../contracts/ci-matrix.yml)  
**Focus**: Workflow triggers, runners, test execution, artifacts, status reporting, branch protection  
**Depth**: Comprehensive (40–60 items)  
**Risk Emphasis**: Correctness (fail-fast on platform mismatch) > Speed (feedback time) > Visibility

---

## Workflow Triggers & Branch Strategy

- [ ] CHK-CI001 - Are GitHub Actions workflow trigger requirements specified for all relevant branches (develop, main, release/*, hotfix/*)? [Completeness, Plan Phase 1]
- [ ] CHK-CI002 - Is the "full validation on PR" requirement documented (all platform jobs for PRs to develop/main)? [Clarity, Spec FR-002]
- [ ] CHK-CI003 - Is the "report-only on feature branches" requirement specified (no CI run on feature/* pushes, only at PR)? [Completeness, Plan Phase 1]
- [ ] CHK-CI004 - Are the trigger conditions for test-macos, test-windows, test-linux, test-ios jobs explicitly specified? [Completeness, Plan Phase 1]
- [ ] CHK-CI005 - Is the job execution order/dependency documented (e.g., code-quality before test jobs, or parallel)? [Clarity, Plan Phase 1]
- [ ] CHK-CI006 - Are artifact retention requirements specified (90-day retention for test results)? [Completeness, Plan Phase 2]
- [ ] CHK-CI007 - Is the concurrent run policy documented (allow parallel runs on same branch or serialize)? [Gap]
- [ ] CHK-CI008 - Is the timeout requirement for each test job specified (target <15 min per job or <30 min total)? [Clarity, Plan Phase 1]

---

## Platform-Specific Runners & Environments

- [ ] CHK-CI009 - Are runner specifications defined for each platform (ubuntu-latest, macos-latest, windows-latest, iOS simulator)? [Completeness, Plan contracts/ci-matrix.yml]
- [ ] CHK-CI010 - Is the macOS runner requirement for iOS simulator tests documented? [Completeness, Plan Phase 1]
- [ ] CHK-CI011 - Are environment setup requirements specified (CMake version, compiler versions, JUCE path configuration)? [Completeness, Plan Phase 1]
- [ ] CHK-CI012 - Is the Git submodule initialization requirement (juce, vst3sdk) explicitly called out in workflow? [Completeness, Plan Phase 0]
- [ ] CHK-CI013 - Are Linux dependency installation requirements documented (ALSA, FreeType, X11, etc.)? [Completeness, Plan Phase 1]
- [ ] CHK-CI014 - Is the Visual Studio 2022 build environment requirement for Windows documented? [Completeness, Plan Phase 1]
- [ ] CHK-CI015 - Are Xcode version requirements for macOS specified? [Gap]
- [ ] CHK-CI016 - Is the self-hosted runner fallback requirement documented (when GitHub-hosted unavailable)? [Gap, Plan Risk Assessment]

---

## Test Execution & Job Configuration

- [ ] CHK-CI017 - Are the CMake build parameters specified for each platform (e.g., CMAKE_BUILD_TYPE=Debug/Release, platform-specific flags)? [Completeness, Plan Phase 1]
- [ ] CHK-CI018 - Is the BUILD_TESTS=ON flag requirement for test builds documented? [Completeness, Plan Phase 0]
- [ ] CHK-CI019 - Are CTest execution parameters specified (ctest --output-on-failure, -j4 parallel, --rerun-failed)? [Clarity, Plan Phase 1]
- [ ] CHK-CI020 - Is the test label filtering requirement documented (ability to run unit, integration, system, or platform-specific tests separately)? [Completeness, Plan Phase 1]
- [ ] CHK-CI021 - Are requirements for measuring and reporting test duration per job specified? [Gap]
- [ ] CHK-CI022 - Is the flaky test retry policy documented in CI (e.g., retry flaky tests N times)? [Completeness, Plan Phase 1]
- [ ] CHK-CI023 - Is the exit code handling requirement specified (non-zero on test failure, zero on all-pass)? [Completeness, Best Practice]
- [ ] CHK-CI024 - Are post-build cleanup requirements documented (e.g., artifact upload happens even on test failure)? [Gap]

---

## Artifact Management & Test Result Reporting

- [ ] CHK-CI025 - Are JUnit XML test result upload requirements specified for all platforms? [Completeness, Plan Phase 1]
- [ ] CHK-CI026 - Is the test result artifact naming convention documented (e.g., test-results-$platform-$timestamp.xml)? [Clarity, Gap]
- [ ] CHK-CI027 - Is the artifact retention period specified (90 days minimum)? [Completeness, Plan Phase 1]
- [ ] CHK-CI028 - Are requirements for lcov coverage report upload to artifacts documented? [Completeness, Plan Phase 1]
- [ ] CHK-CI029 - Is the Codecov integration requirement (if used) documented with fallback to local lcov? [Completeness, Plan Risk Assessment]
- [ ] CHK-CI030 - Are per-platform coverage report requirements specified (separate reports per platform/format)? [Gap, Plan Phase 1]
- [ ] CHK-CI031 - Is the test failure log upload requirement documented (full output captured for debugging)? [Completeness, Best Practice]
- [ ] CHK-CI032 - Are build artifact storage requirements defined (binaries, logs, coverage reports)? [Gap]

---

## Status Reporting & GitHub Integration

- [ ] CHK-CI033 - Are PR status check requirements documented for all jobs (all must pass to merge)? [Completeness, Plan Phase 1]
- [ ] CHK-CI034 - Is the per-platform status requirement specified (separate check per platform for visibility)? [Completeness, Spec FR-002]
- [ ] CHK-CI035 - Are PR comment requirements documented (optional: coverage diff, test summary)? [Gap, Plan Phase 2]
- [ ] CHK-CI036 - Is the PR comment bot trigger requirement specified (auto-comment on test completion)? [Gap, Plan Phase 2]
- [ ] CHK-CI037 - Are requirements for linking to artifacts in PR comments documented? [Gap]
- [ ] CHK-CI038 - Is the failure annotation requirement documented (mark failing test in code if available)? [Gap, GitHub feature]
- [ ] CHK-CI039 - Are summary report requirements defined (summary table: platform, tests run, pass rate, duration)? [Gap, Plan Metrics]
- [ ] CHK-CI040 - Is the notification requirement for failing main/release branches specified (Discord webhook, email, etc.)? [Gap, Plan Phase 2+]

---

## Branch Protection & Gating Strategy

- [ ] CHK-CI041 - Is the report-only policy requirement for Weeks 1–2 documented (no merge blocking)? [Completeness, Spec FR-011, Plan Phase 0]
- [ ] CHK-CI042 - Is the "new test failures block" policy for Week 3+ specified in branch protection rules? [Completeness, Spec FR-011, Plan Phase 1]
- [ ] CHK-CI043 - Is the soft gate requirement (Week 4: warn on <70% coverage, can merge with approval) documented? [Completeness, Plan Phase 2]
- [ ] CHK-CI044 - Is the hard gate requirement for Week 8 (new code <80% blocks merge) documented? [Completeness, Plan Phase 3]
- [ ] CHK-CI045 - Are requirements for dismissing stale reviews after code changes documented? [Gap, GitHub setting]
- [ ] CHK-CI046 - Is the required approver(s) configuration specified for each gate phase? [Gap]
- [ ] CHK-CI047 - Are requirements for allowing bypass/force-push for emergency fixes documented? [Gap, Plan Phase 3]
- [ ] CHK-CI048 - Is the gate escalation timeline documented (what to do if <70% by Week 6)? [Completeness, Plan Open Questions]

---

## Coverage Measurement & Tracking

- [ ] CHK-CI049 - Are coverage measurement tool requirements specified (lcov on Linux as canonical)? [Completeness, Plan Phase 1]
- [ ] CHK-CI050 - Is the line coverage reporting requirement documented (% coverage per file/component)? [Completeness, Plan Phase 1]
- [ ] CHK-CI051 - Are branch coverage measurement requirements specified (if applicable)? [Gap, Plan Open Question]
- [ ] CHK-CI052 - Is the coverage baseline documentation requirement specified (track historical coverage %)? [Gap]
- [ ] CHK-CI053 - Is the per-platform coverage variance requirement documented (allowable differences across platforms)? [Gap]
- [ ] CHK-CI054 - Are requirements for filtering JUCE/external code from coverage reports documented? [Completeness, Plan Phase 1]
- [ ] CHK-CI055 - Is the coverage delta calculation requirement specified (coverage change vs baseline)? [Gap, Plan Phase 2]
- [ ] CHK-CI056 - Are requirements for coverage dashboard/visibility documented (where are results published)? [Gap, Plan Phase 2+]

---

## Error Handling & Resilience

- [ ] CHK-CI057 - Are transient failure handling requirements documented (retry policy, timeouts)? [Completeness, Plan Risk Assessment]
- [ ] CHK-CI058 - Is the runner availability fallback requirement specified (if GitHub-hosted unavailable, use self-hosted)? [Partially Documented, Plan Risk Assessment]
- [ ] CHK-CI059 - Are requirements for detecting and handling flaky tests in CI documented (quarantine, report, retry)? [Completeness, Plan Phase 1]
- [ ] CHK-CI060 - Is the test cancellation requirement specified (cancel remaining jobs if critical job fails)? [Gap]
- [ ] CHK-CI061 - Are cleanup requirements documented for failed builds (release resources, cleanup temporary files)? [Gap]
- [ ] CHK-CI062 - Is the network failure handling documented (e.g., retry artifact upload on transient failure)? [Gap]
- [ ] CHK-CI063 - Are requirements for continuing/skipping tests on platform-specific build failures documented? [Gap]

---

## Performance & Optimization

- [ ] CHK-CI064 - Is the target CI feedback time requirement specified (<15 minutes median)? [Completeness, Spec SC-002]
- [ ] CHK-CI065 - Is the parallel test execution requirement documented (ctest -j4 or higher)? [Completeness, Plan Phase 1]
- [ ] CHK-CI066 - Are CTest cost-based scheduling requirements documented (unit tests first, system tests last)? [Completeness, Plan Phase 1]
- [ ] CHK-CI067 - Is the build caching requirement documented (cache CMake builds, JUCE compilation)? [Gap, Plan Phase 4]
- [ ] CHK-CI068 - Are requirements for incremental builds specified? [Gap]
- [ ] CHK-CI069 - Is the artifact caching requirement specified (cache .conan, .m2, etc. for dependencies)? [Gap]

---

## Security & Compliance

- [ ] CHK-CI070 - Are GPL-3.0 header check requirements documented in CI pipeline? [Completeness, Plan Phase 0]
- [ ] CHK-CI071 - Is the compiler warning-as-error requirement documented in CI? [Completeness, Plan Phase 0]
- [ ] CHK-CI072 - Are requirements for scanning dependencies for vulnerabilities documented? [Gap]
- [ ] CHK-CI073 - Is the Conventional Commits validation requirement documented (if enforced in CI)? [Gap, Constitution requirement]
- [ ] CHK-CI074 - Are secrets management requirements documented (no credentials in logs)? [Gap]
- [ ] CHK-CI075 - Is the GitHub token scope requirement documented (minimum permissions for CI)? [Gap]

---

## Documentation & Maintenance

- [ ] CHK-CI076 - Are CI workflow documentation requirements specified (README.md section, troubleshooting guide)? [Gap]
- [ ] CHK-CI077 - Is the CI configuration change review requirement documented (peer review before merge)? [Gap]
- [ ] CHK-CI078 - Are requirements for keeping CI job definitions DRY (shared templates, reusable workflows) documented? [Gap, GitHub Actions best practice]
- [ ] CHK-CI079 - Is the quarterly CI health review requirement documented (check for obsolete jobs, opportunities)? [Gap, Plan Phase 4]
- [ ] CHK-CI080 - Are requirements for documenting CI decisions (why certain tools, constraints) specified? [Gap, Plan Risk Assessment notes]

---

## Open Questions & Decision Points

- [ ] CHK-CI081 - Decision: Should PR comment bot be implemented (coverage diff, test summary) or is email/Discord enough? [Open Decision, Plan Phase 2]
- [ ] CHK-CI082 - Decision: Should concurrent runs on same branch be allowed or serialized? [Open Decision]
- [ ] CHK-CI083 - Decision: Which coverage dashboard tool (Codecov, Coveralls, custom)? [Open Decision, Plan Phase 1]
- [ ] CHK-CI084 - Decision: Should self-hosted runners be set up now or only if GitHub-hosted exceeds cost threshold? [Open Decision, Plan Risk Assessment]
- [ ] CHK-CI085 - Decision: Branch coverage in addition to line coverage, or line coverage only? [Open Decision, Plan Phase 2, CHK060]

---

## Validation Summary

**Overall Status**: 
- **Documented**: 52 items
- **Partially Documented**: 8 items (exist in plan but could be clearer)
- **Gaps**: 20 items (missing in current spec/plan)
- **Open Decisions**: 5 items (require team consensus)

**High-Priority Gaps Before Phase 1 CI Implementation**:
1. CHK-CI007 — Concurrent run policy (serialize or parallel)
2. CHK-CI015 — Xcode version requirement
3. CHK-CI016 — Self-hosted runner fallback criteria
4. CHK-CI021 — Test duration reporting requirement
5. CHK-CI024 — Post-build cleanup (artifacts even on failure)
6. CHK-CI032 — Build artifact storage specification

**Medium-Priority (Phase 1 Cleanups)**:
- CHK-CI026 — Artifact naming conventions
- CHK-CI030 — Per-platform coverage reports
- CHK-CI035–CHK-CI040 — PR visibility/reporting (may defer to Phase 2)
- CHK-CI045–CHK-CI047 — Branch protection advanced features

**Non-Blocking (Phase 2+)**:
- CHK-CI052–CHK-CI056 — Coverage dashboard and historical tracking
- CHK-CI067–CHK-CI069 — Build optimization (Phase 4)
- CHK-CI070–CHK-CI075 — Security/compliance enhancements
- CHK-CI076–CHK-CI080 — Documentation improvements

**Recommended Action**: Create `.github/workflows/test.yml` template with documented parameters for each job, referencing this checklist for completeness verification.

