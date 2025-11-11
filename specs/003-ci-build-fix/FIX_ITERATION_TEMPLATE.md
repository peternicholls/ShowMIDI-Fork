# Phase 2: Fix Iteration Template

Use this template to drive each Phase 2 fix cycle end-to-end. Duplicate this section per issue and replace placeholders.

---

## Fix Iteration: CI Issue <number> - <short-description>

**Detected in**: <link-to-failing-run-or-PR>

**Category**: config | cmake | build | xcode | artifact | timeout | infrastructure

### Context
- Commit/PR: <sha-or-PR-number>
- Platform(s): Linux | macOS | Windows
- Workflow: `.github/workflows/ci.yml` job: <job-id>

### Failure Evidence
- Logs: <key-log-snippets>
- Error message: "<copy exact error>"

### Root Cause Analysis
- What happened:
- Why it happened:
- Scope of impact:

### Fix Design
- Proposed change:
- Files to modify:
  - <path>
- Alternatives considered:

### Execution Checklist (T2XX)
- [ ] T2XX Collect failure data (logs, error messages)
- [ ] T2XX Categorize failure type (config, CMake, build, Xcode, artifact, timeout, infrastructure)
- [ ] T2XX Document root cause analysis
- [ ] T2XX Design fix based on failure category
- [ ] T2XX Create fix branch `fix/ci-issue-<number>-<description>`
- [ ] T2XX Implement fix in appropriate file(s)
- [ ] T2XX Test fix locally (if applicable)
- [ ] T2XX Commit fix with detailed message
- [ ] T2XX Push branch and create PR to `003-ci-build-fix`
- [ ] T2XX Wait for CI to pass on fix PR
- [ ] T2XX Merge fix PR to `003-ci-build-fix`
- [ ] T2XX Re-run original failing test to verify fix
- [ ] T2XX Verify no regression (other tests still pass)
- [ ] T2XX Document fix in iteration log

### Validation Results
- CI Run: <link>
- Status: PASS | FAIL
- Notes: <summary>

---

Tip: Keep each iteration focused on a single root cause to maintain clear auditability and reduce risk of regressions.
