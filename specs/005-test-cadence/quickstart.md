# Quick Start: Testing Cadence & Governance (Run 1)

**Feature**: 005-test-cadence  
**Audience**: Developers, maintainers, release managers  
**Prerequisites**: Familiarity with Git, GitHub Actions, and ShowMIDI codebase

---

## Overview

Phase 11 establishes a comprehensive testing cadence and governance framework to protect ShowMIDI's constitutional principles (multi-platform reliability, real-time performance, user-centric quality, maintainability) through structured validation at every development stage.

**Key Concepts**:
- **12 Check Categories**: Unit, integration, system, performance, UI/visual, static analysis, format, lint, security (SAST/SCA), license, build, packaging/installer
- **8 Trigger Contexts**: Local on-demand, pre-commit, pre-push, PR, on-merge, nightly, release, hotfix
- **Progressive Gates**: feature → develop → release → main (increasing rigor)
- **Flake Governance**: Automated detection (3/10 failures) → quarantine → owner assignment → SLA remediation

---

## For Developers: Local Workflow

### Setup (One-Time)

1. **Install Git Hooks**:
   ```bash
   ./scripts/install-hooks.sh
   ```
   This installs pre-commit (format/lint) and pre-push (unit tests) hooks.

2. **Verify Configuration**:
   ```bash
   # Check hook installation
   ls -la .git/hooks/pre-commit .git/hooks/pre-push
   
   # Test pre-commit manually
   ./scripts/hooks/pre-commit
   ```

### Daily TDD Loop

1. **Write failing test** (red):
   ```bash
   # Edit Tests/Unit/MyComponentTest.cpp
   # Add new test case with expected behavior
   ```

2. **Run tests locally** (before committing):
   ```bash
   # Run all unit tests
   cd build && ctest --output-on-failure
   
   # Run specific test
   ctest -R MyComponentTest --verbose
   ```

3. **Implement feature** (green):
   ```bash
   # Edit Source/MyComponent.cpp
   # Make test pass
   ```

4. **Commit** (triggers pre-commit hook):
   ```bash
   git add Source/MyComponent.cpp Tests/Unit/MyComponentTest.cpp
   git commit -m "feat(component): add MyFeature with unit tests"
   # Pre-commit auto-runs: clang-format, GPL header check
   ```

5. **Push** (triggers pre-push hook):
   ```bash
   git push origin feature/123-my-feature
   # Pre-push runs: unit tests (1-2 min)
   # If tests fail, push is blocked
   ```

### What Runs When (Developer Perspective)

| Event | Checks | Duration | Can Skip? |
|-------|--------|----------|-----------|
| **Commit** | clang-format (auto-fix), GPL headers | <10s | No |
| **Push** | Unit tests | 1-2 min | No (but `--no-verify` for emergencies) |
| **PR Open** | Unit, integration, static analysis, format/lint, build (macOS) | Median 5 min | No (required for merge) |
| **Nightly** | All checks, all platforms | ~60 min | N/A (automatic) |

---

## For Reviewers: PR Validation

### What to Expect

When a PR is opened to `develop` or `main`, required checks run automatically:

1. **Status Checks** (visible in PR):
   - ✅ Unit Tests (macOS)
   - ✅ Integration Tests (core flows)
   - ✅ Static Analysis (clang-tidy, cppcheck)
   - ✅ Format/Lint (clang-format, GPL headers)
   - ✅ Build Verification (macOS Xcode)
   - ⚠️ Security (SAST/SCA) — advisory unless High/Critical severity

2. **Time Budget**: Results typically within 5 minutes (p95: 10 minutes)

3. **Flaky Tests**: If a check shows intermittent failures:
   - CI auto-detects flakes (3 failures in 10 runs)
   - Flaky check is quarantined (removed from required gates)
   - Owner assigned with 2-day SLA for remediation
   - PR can merge without passing quarantined check

### How to Review

1. **Check CI Status**: All required checks must be green before merge
2. **Review Test Coverage**: New code should include unit tests (goal: ≥80% line coverage for new code)
3. **Approve or Request Changes**: Use GitHub review flow
4. **Merge**: Squash and merge to `develop` (triggers post-merge validation)

### Skip/Bypass Process

**When Allowed**:
- Multi-feature integration requiring coordinated merge
- Critical hotfix with documented risk acceptance

**How to Skip**:
1. Add PR comment: `Skip: [check-category] — Reason: [justification] — Approved by: @release-manager`
2. Link tracking issue for remediation
3. Release manager approves (audit trail logged)

---

## For Release Managers: Release Workflow

### Release Branch Gates

When creating a `release/X.Y.Z` branch:

1. **Nightly Suite** runs automatically:
   - All check categories (except packaging/installer)
   - All platforms (macOS, Windows, Linux)
   - Time budget: ~60 min

2. **Additional Release Gates** (triggered on `release/*` push):
   - Packaging validation (DMG, EXE, RPM/DEB)
   - Installer smoke tests (install → launch → uninstall)
   - Time budget: ~90 min total

3. **Merge to Main** requires:
   - All gates green (100% pass rate)
   - Final smoke tests on all plugin formats (VST/VST3/AU/LV2/CLAP)
   - Version synchronized in `CMakeLists.txt` and `showmidi.jucer`

### Hotfix Fast-Track

For critical production bugs:

1. **Create hotfix branch** from `main`:
   ```bash
   git checkout main
   git pull
   git checkout -b hotfix/1.0.2-critical-bug
   ```

2. **Fix and test locally**:
   ```bash
   # Edit source
   git commit -m "fix(midi): resolve critical bug"
   git push origin hotfix/1.0.2-critical-bug
   ```

3. **Hotfix Gates** (fast turnaround: ~20 min):
   - Unit tests
   - Integration tests (safety-critical subset)
   - Build verification
   - Security (High+ severity only)
   - **Skipped**: Performance, UI/visual, packaging (defer to post-release validation)

4. **Merge to Main and Develop**:
   ```bash
   git checkout main
   git merge --no-ff hotfix/1.0.2-critical-bug
   git tag v1.0.2
   git push origin main --tags
   
   git checkout develop
   git merge --no-ff hotfix/1.0.2-critical-bug
   git push origin develop
   ```

---

## For Maintainers: Flake Management

### Detecting Flakes

**Automated**: CI tracks pass/fail history per check. Flake flagged if:
- ≥3 failures in last 10 runs, OR
- ≥20% failure rate over 7 days

**Manual**: Developer reports intermittent test failure in issue.

### Quarantine Workflow

1. **Detection**: CI auto-creates GitHub issue:
   ```
   Title: [Flake] unit-tests failing intermittently
   Labels: flaky-test, needs-triage
   Assigned: @core-team (from CODEOWNERS)
   SLA: 2 business days
   ```

2. **Triage**: Owner investigates:
   - Timing-dependent? → Add deterministic sleep or mock time
   - Platform-specific? → Add conditional skip with tracking issue
   - External dependency? → Mock or stub (constitution requires adapters)
   - Genuine bug? → Fix root cause

3. **Remediation**: Owner fixes test, monitors pass rate

4. **Re-enablement**: After ≥95% pass rate over 14 days + owner sign-off, check returns to required gates

### Monitoring Dashboard (Future)

Planned: Web dashboard showing:
- Current quarantined checks
- SLA deadline countdowns
- Pass rate trends per check
- Owner workload distribution

---

## Configuration Files

### Primary Config: `testing-governance.yaml`

Location: `.github/testing-governance.yaml` (git-versioned)

**Editing**:
```bash
# Edit configuration
vim specs/005-test-cadence/contracts/testing-governance.yaml

# Validate schema
scripts/validate-testing-config.sh specs/005-test-cadence/contracts/testing-governance.yaml

# Deploy to CI
cp specs/005-test-cadence/contracts/testing-governance.yaml .github/testing-governance.yaml
git add .github/testing-governance.yaml
git commit -m "config(testing): update PR time budget to 6 min median"
```

### Runtime Data: Flake Tracking

Location: GitHub Actions artifacts (not git-versioned)

**Viewing**:
```bash
# Download latest artifact
gh run download <run-id> -n flake-tracking

# Parse JSON
cat flake-tracking.json | jq '.quarantined_checks'
```

---

## Common Scenarios

### Scenario 1: PR Failing on "Integration Tests"

1. **Check CI logs**: Click failing check in PR → view detailed output
2. **Reproduce locally**:
   ```bash
   cd build
   ctest -R IntegrationTests --verbose
   ```
3. **Fix and re-push**: CI re-runs automatically on new commits

### Scenario 2: Flaky Test Blocking Urgent PR

1. **Verify flakiness**: Check if test has failed intermittently in recent PRs
2. **Request quarantine**: Comment on PR: `@release-manager please quarantine integration-tests — flaky on macOS, blocking urgent fix`
3. **Owner assigns**: Release manager verifies, adds skip approval, assigns owner
4. **PR merges**: After approval, PR can merge; tracking issue created for remediation

### Scenario 3: Adding New Check Category

1. **Define in config**:
   ```yaml
   # specs/005-test-cadence/contracts/testing-governance.yaml
   check_categories:
     - id: accessibility-tests
       name: Accessibility Tests
       purpose: Validate keyboard navigation and screen reader support
       tools: [axe-core, custom JUCE accessibility checks]
       owner: ui-team
       blocking_policy: conditional
       condition_expression: "trigger_context == 'nightly-develop'"
       expected_duration_sec: 300
       timeout_sec: 450
   ```

2. **Add to trigger context**:
   ```yaml
   trigger_contexts:
     - id: nightly-develop
       enabled_checks:
         - accessibility-tests  # Add new check
   ```

3. **Deploy**: Follow config editing workflow above

4. **Monitor**: Observe nightly runs for 2 weeks before promoting to PR gates

---

## Troubleshooting

### Pre-commit Hook Not Running

**Symptom**: Commits succeed without format/lint checks

**Solution**:
```bash
# Re-install hooks
./scripts/install-hooks.sh

# Verify permissions
chmod +x .git/hooks/pre-commit
```

### Unit Tests Pass Locally, Fail in CI

**Causes**:
- Platform differences (macOS local, Linux CI)
- Timing/concurrency issues
- File path assumptions

**Debugging**:
```bash
# Run tests in CI-like environment (Docker)
docker run -v $(pwd):/workspace ubuntu:22.04
cd /workspace/build
cmake .. && cmake --build .
ctest --output-on-failure
```

### Time Budget Exceeded on PR

**Symptom**: CI jobs timeout or exceed 10 min p95

**Solutions**:
- **Cache dependencies**: Verify CMake ccache is working
- **Parallelize tests**: Split into multiple jobs
- **Move slow tests to nightly**: Update `testing-governance.yaml` to exclude from PR
- **Optimize test setup**: Reduce fixture initialization time

---

## Next Steps

1. **Phase 2 Tasks** (via `/speckit.tasks`): Break down implementation into actionable tickets
2. **Pilot on Single Check**: Start with unit tests only, validate workflow
3. **Incremental Rollout**: Add check categories one at a time (nightly → PR)
4. **Constitution Amendment**: Ratify Section VI after successful pilot

---

## Support

- **Questions**: #showmidi-dev channel
- **Issues**: GitHub Issues with `testing-governance` label
- **Documentation**: `specs/005-test-cadence/` directory
