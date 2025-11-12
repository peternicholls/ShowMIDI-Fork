# Testing Framework - Agent Guide

**Purpose**: Machine-readable reference for AI agents working with ShowMIDI's testing infrastructure  
**Last Updated**: 2025-11-12  
**Status**: Phase 5 Complete (MVP through Nightly validation)

## Quick Reference

### File Locations

```yaml
Configuration:
  Primary: .github/testing-governance.yaml
  Schemas: scripts/validation/*.schema.json
  
Workflows:
  PR/Push: .github/workflows/ci.yml
  Nightly: .github/workflows/nightly.yml
  
Scripts:
  Hooks: scripts/hooks/pre-commit, scripts/hooks/pre-push
  Validation: scripts/validation/validate-testing-config.sh
  Reporting: scripts/reporting/generate-run-summary.sh
  Setup: scripts/install-hooks.sh
  
Documentation:
  Spec: specs/005-test-cadence/spec.md
  Plan: specs/005-test-cadence/plan.md
  Tasks: specs/005-test-cadence/tasks.md
  Quickstart: specs/005-test-cadence/quickstart.md
```

### Testing Commands

```bash
# Local Development
./scripts/install-hooks.sh              # One-time setup
cd build && ctest --output-on-failure   # Run all tests
ctest -L unit --verbose                 # Unit tests only
ctest -L integration --verbose          # Integration tests only

# Configuration Validation
./scripts/validation/validate-testing-config.sh .github/testing-governance.yaml

# Manual Test Run Summary
TRIGGER_CONTEXT=pr-to-develop PLATFORM=macos ./scripts/reporting/generate-run-summary.sh
```

## Check Categories (12 Total)

| ID | Purpose | Tools | Blocking | Timeout | Triggers |
|---|---|---|---|---|---|
| `unit-tests` | Component isolation | JUCE UnitTest, CTest | always | 180s | pre-push, PR, nightly |
| `integration-tests` | Module boundaries | JUCE UnitTest, mocks | always | 270s | PR, nightly |
| `system-tests` | End-to-end flows | UI automation, smoke | conditional | 900s | nightly, release |
| `performance-tests` | <10ms latency validation | JUCE timers, benchmarks | conditional | 450s | nightly, release |
| `ui-visual-regression` | DPI/theme validation | Screenshot comparison | never | 270s | nightly |
| `static-analysis` | Code quality | clang-tidy, cppcheck | always | 135s | PR, nightly |
| `formatting` | Style consistency | clang-format | always | 45s | pre-commit, PR |
| `linting` | GPL headers, idioms | clang-tidy, custom | always | 45s | pre-commit, PR |
| `security-sast` | Vulnerability detection | CodeQL | conditional | 270s | nightly |
| `security-sca` | Dependency scanning | Dependabot | conditional | 180s | nightly |
| `license-compliance` | GPL-3.0 compatibility | FOSSA | always | 135s | nightly |
| `build-verification` | Multi-platform builds | CMake, Xcode, VS2022 | always | 270s | PR, nightly |

**Conditional Blocking Logic**:
- `system-tests`, `performance-tests`: Block on `nightly-develop` and `release-gate` contexts
- `security-sast`, `security-sca`: Block if severity is `High` or `Critical`

## Trigger Contexts (3 Implemented)

### pre-commit (Local)
```yaml
enabled_checks: [formatting, linting]
time_budget: 10s (p95: 15s)
auto_fix: true (clang-format)
```

### pre-push (Local)
```yaml
enabled_checks: [unit-tests]
time_budget: 120s (p95: 180s)
can_bypass: --no-verify flag (emergencies only)
```

### pr-to-develop (GitHub Actions)
```yaml
enabled_checks: [unit-tests, integration-tests, static-analysis, formatting, linting, build-verification]
time_budget: 300s median, 600s p95 (5-10 min)
parallelization: matrix (macos-latest primary, windows/linux parallel)
fail_fast: true
platform_matrix:
  primary: macos-latest (always runs)
  conditional: windows-latest, ubuntu-latest (based on changed files)
```

### nightly-develop (GitHub Actions)
```yaml
enabled_checks: [ALL 12 categories]
time_budget: 3600s median, 5400s p95 (60-90 min)
schedule: "0 2 * * *" (2 AM UTC daily)
change_detection: true (skip if no code changes since last success)
platforms: [macos-14, windows-latest, ubuntu-latest] (all run unconditionally)
```

## Configuration Schema Reference

### CheckCategory
```yaml
id: string (unique, kebab-case)
name: string (display name)
purpose: string (1-2 sentences)
tools: string[] (non-empty)
owner: string (references owners.id)
blocking_policy: "always" | "conditional" | "never"
condition_expression: string? (if blocking_policy == conditional)
expected_duration_sec: number (> 0)
timeout_sec: number (>= expected_duration_sec)
flake_threshold:
  failure_count_window: 10
  failure_threshold: 3
  failure_rate_7d_threshold: 0.20
```

### TriggerContext
```yaml
id: string (unique)
event_type: "local" | "pre-commit" | "pre-push" | "pull_request" | "nightly" | "release" | "hotfix"
scope: string (branch pattern or "local")
enabled_checks: string[] (references check_categories[].id)
time_budget_sec: number (> 0)
time_budget_p95_sec: number (>= time_budget_sec)
parallelization: "sequential" | "parallel" | "matrix"
change_detection_enabled: boolean
platform_matrix?: object
```

### Policy
```yaml
id: string (unique)
policy_type: "flake" | "skip" | "retry" | "caching" | "reporting"
applies_to: string[] (TriggerContext or CheckCategory IDs)
rules: object (varies by policy_type)
enforcement: "hard" | "soft" | "advisory"
sla_hours?: number
```

## Workflow Integration Points

### ci.yml Job Dependencies
```
config-loader (reads YAML)
  ↓
code-quality (format/lint checks)
  ↓
[build-and-test-macos, build-windows, build-linux] (parallel)
  ↓
test-suite (matrix: macOS/Windows/Linux, fail-fast)
  ↓
test-summary (aggregates results, posts PR comment)
```

### nightly.yml Job Dependencies
```
change-detection (git diff analysis)
  ↓
config-loader (if changes detected)
  ↓
[system-tests, performance-tests, full-platform-matrix] (parallel, fail-fast: false)
  ↓
nightly-summary (report generation)
```

## Modifying Configuration

### Add New Check Category
1. Edit `.github/testing-governance.yaml` → `check_categories[]`
2. Add to relevant `trigger_contexts[].enabled_checks`
3. Validate: `./scripts/validation/validate-testing-config.sh .github/testing-governance.yaml`
4. Update CI jobs in `.github/workflows/ci.yml` if new steps required

### Change Time Budget
1. Edit `.github/testing-governance.yaml` → `trigger_contexts[].time_budget_sec` and `time_budget_p95_sec`
2. Config loader auto-reads on next run (no workflow changes needed)
3. Verify: Check `config-loader` job output in next CI run

### Add Platform to Matrix
1. Edit `.github/testing-governance.yaml` → `trigger_contexts[].platform_matrix`
2. Update workflow matrix in `.github/workflows/ci.yml` → `test-suite.strategy.matrix`

## Common Agent Tasks

### Parse Test Results
```bash
# Extract pass/fail counts from CTest
grep -c "^[0-9]" build/Testing/Temporary/LastTest.log  # Total tests
grep -c "Passed" build/Testing/Temporary/LastTest.log   # Passed
grep -c "Failed" build/Testing/Temporary/LastTest.log   # Failed
```

### Analyze Configuration
```bash
# Install yq (YAML parser)
brew install yq  # macOS
sudo wget -qO /usr/local/bin/yq https://github.com/mikefarah/yq/releases/latest/download/yq_linux_amd64  # Linux

# Query examples
yq eval '.check_categories[] | select(.id == "unit-tests") | .timeout_sec' .github/testing-governance.yaml
yq eval '.trigger_contexts[] | select(.id == "pr-to-develop") | .enabled_checks' .github/testing-governance.yaml
```

### Validate Schema Compliance
```bash
# Check categories schema
jq -e . scripts/validation/checks.schema.json > /dev/null && echo "Valid JSON"

# Validate against schema (requires ajv-cli)
npm install -g ajv-cli
ajv validate -s scripts/validation/checks.schema.json -d .github/testing-governance.yaml
```

## Implementation Status

### ✅ Completed (Phases 1-5)
- [x] Scripts directory structure (`scripts/hooks/`, `scripts/validation/`, `scripts/reporting/`)
- [x] Testing governance configuration (`.github/testing-governance.yaml`)
- [x] All 12 CheckCategory definitions
- [x] Owner mappings (core-team, platform-*, security-team)
- [x] Git hooks (pre-commit: format/lint, pre-push: unit tests)
- [x] Config loader job (parses YAML, sets outputs)
- [x] PR validation workflow with time budgets and fail-fast
- [x] Caching strategy (CMake, JUCE, ccache)
- [x] Platform matrix (macOS primary + conditional Windows/Linux)
- [x] Test summary reporter with PR comments
- [x] Nightly workflow with change detection
- [x] System and performance test jobs
- [x] Flake-quarantine policy definition

### ⏸️ Pending (Phases 6-11)
- [ ] Flake detection workflow (`.github/workflows/flake-detector.yml`)
- [ ] Quarantine automation scripts
- [ ] Release/hotfix workflows
- [ ] CodeQL, Dependabot integration
- [ ] Skip approval workflow
- [ ] Constitutional amendment (Section VI)
- [ ] Final validation and cleanup

## Troubleshooting

### Config Loader Fails
**Symptom**: `config-loader` job fails with YAML parse error  
**Solution**: Validate YAML syntax
```bash
yq eval . .github/testing-governance.yaml
./scripts/validation/validate-testing-config.sh .github/testing-governance.yaml
```

### Test Timeout Exceeded
**Symptom**: CTest aborts with timeout  
**Solution**: Check actual vs expected duration
```bash
# View timeout setting
yq eval '.check_categories[] | select(.id == "unit-tests") | .timeout_sec' .github/testing-governance.yaml

# Increase if needed (edit YAML, commit)
```

### Platform-Specific Test Failure
**Symptom**: Test passes on macOS, fails on Linux  
**Solution**: 
1. Check platform-specific code paths (`#if JUCE_LINUX`)
2. Run locally in Docker: `docker run -v $(pwd):/workspace ubuntu:22.04`
3. Review CTest output artifact: `test-results-Linux-<run_number>/LastTest.log`

## Agent Workflow Example

```python
# Pseudocode for agent analyzing test failure

def analyze_test_failure(run_id):
    # 1. Download artifacts
    artifacts = github.download_artifacts(run_id, pattern="test-results-*")
    
    # 2. Parse test logs
    for artifact in artifacts:
        log = read_file(f"{artifact}/LastTest.log")
        failures = parse_failures(log)  # Extract failed test names
        
    # 3. Check if flaky
    history = query_flake_tracking(failures[0].test_name)
    if history.failure_rate_7d > 0.20:
        trigger_quarantine(failures[0].test_name)
    
    # 4. Generate report
    report = {
        "run_id": run_id,
        "failed_tests": failures,
        "flaky": history.is_flaky,
        "recommended_action": "investigate" if not history.is_flaky else "quarantine"
    }
    
    return report
```

## References

- **Spec**: `specs/005-test-cadence/spec.md` - User stories and requirements
- **Plan**: `specs/005-test-cadence/plan.md` - Technical architecture
- **Tasks**: `specs/005-test-cadence/tasks.md` - Implementation breakdown
- **Quickstart**: `specs/005-test-cadence/quickstart.md` - Human-friendly guide
- **Data Model**: `specs/005-test-cadence/data-model.md` - Entity definitions
- **Research**: `specs/005-test-cadence/research.md` - Technology decisions

---

**Next Steps for Agents**:
1. For new check category → Edit `testing-governance.yaml`, validate, update workflows
2. For test failure → Download artifacts, parse logs, check flake history
3. For configuration query → Use `yq` to parse YAML programmatically
4. For workflow modification → Read job dependencies, preserve timeout/caching patterns
