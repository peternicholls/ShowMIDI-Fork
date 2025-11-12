# ShowMIDI Testing Governance

**Status**: 🚧 In Development (Phase 11 - Testing Cadence & Governance)  
**Version**: 1.0.0  
**Last Updated**: 2025-11-12

---

## Overview

This directory contains the testing governance framework for ShowMIDI, establishing comprehensive testing cadence and quality assurance practices across all development stages (local, PR, nightly, release, hotfix).

### Key Components

1. **Configuration**: `testing-governance.yaml` - Central configuration for all testing policies
2. **Schemas**: JSON schemas for validation and tracking artifacts
3. **Scripts**: Validation and reporting automation

---

## Quick Start

### For Developers

1. **Install Git Hooks** (one-time setup):
   ```bash
   ./scripts/install-hooks.sh
   ```

2. **Daily Workflow**:
   - Commit: Auto-formats code with clang-format, checks GPL headers
   - Push: Runs unit tests before pushing to remote
   - PR: CI validates unit, integration, static analysis, formatting, linting, build

### For Reviewers

- All PRs must pass required checks before merge
- Median PR validation time: ≤5 minutes
- Flaky tests are automatically quarantined

### For Release Managers

- Release branches run full platform validation + packaging checks
- Hotfix branches run fast-track validation (≤20 min)

---

## Configuration Structure

### `testing-governance.yaml`

```yaml
version: "1.0.0"

owners:           # Team/area accountability
check_categories: # 14 types of checks (unit, integration, security, etc.)
trigger_contexts: # When checks run (pre-commit, PR, nightly, etc.)
policies:         # Governance rules (flake detection, retry, caching)
```

### Check Categories (14 total)

| Category | Purpose | Tools | Blocking |
|----------|---------|-------|----------|
| **unit-tests** | Component isolation | JUCE UnitTest, CTest | ✅ Always |
| **integration-tests** | Module boundaries | JUCE UnitTest, mocks | ✅ Always |
| **system-tests** | End-to-end flows | Automated UI, smoke | ⚠️ Conditional |
| **performance-tests** | <10ms latency validation | JUCE timer, benchmarks | ⚠️ Conditional |
| **ui-visual-regression** | DPI/theme validation | Screenshots | ❌ Advisory |
| **static-analysis** | Code quality | clang-tidy, cppcheck | ✅ Always |
| **formatting** | Style consistency | clang-format | ✅ Always |
| **linting** | GPL headers, idioms | clang-tidy, scripts | ✅ Always |
| **security-sast** | Vulnerability detection | GitHub CodeQL | ⚠️ High/Critical |
| **security-sca** | Dependency scanning | Dependabot, OWASP | ⚠️ High/Critical |
| **license-compliance** | GPL-3.0 compatibility | FOSSA, scripts | ✅ Always |
| **build-verification** | Cross-platform builds | CMake, Xcode, VS2022 | ✅ Always |
| **packaging-validation** | Artifacts (DMG/EXE/DEB) | Platform tools | ⚠️ Release only |
| **installer-smoke** | Install/uninstall tests | Scripts | ⚠️ Release only |

### Trigger Contexts

| Context | Event | Checks | Time Budget |
|---------|-------|--------|-------------|
| **pre-commit** | Local | formatting, linting | <10s |
| **pre-push** | Local | unit-tests | <2 min |
| **pr-to-develop** | PR | unit, integration, static, format, lint, build | 5 min median, 10 min p95 |
| **nightly-develop** | Cron | All checks, all platforms | <60 min |
| **release-gate** | release/* | Full validation + packaging + installer | <90 min |
| **hotfix-gate** | hotfix/* | Safety-critical subset | <20 min |

### Owners

| Team | Owned Categories | SLA |
|------|------------------|-----|
| **core-team** | unit, integration, build, packaging, installer | 48h |
| **platform-macos** | build-verification (macOS) | 48h |
| **platform-windows** | build-verification (Windows) | 48h |
| **platform-linux** | build-verification (Linux) | 48h |
| **security-team** | security-sast, security-sca, license-compliance | 24h |

---

## Git Hooks

### Pre-Commit Hook

**Location**: `scripts/hooks/pre-commit`

**What it does**:
1. Auto-formats staged C++ files with clang-format
2. Checks new files for GPL-3.0 headers
3. Fast execution (<10s typical)

**Bypass** (emergencies only):
```bash
git commit --no-verify
```

### Pre-Push Hook

**Location**: `scripts/hooks/pre-push`

**What it does**:
1. Runs CTest unit tests from `build/` directory
2. Blocks push if tests fail
3. Execution time: 1-2 minutes typical

**Bypass** (emergencies only):
```bash
git push --no-verify
```

---

## Validation

### Manual Validation

```bash
# Validate configuration file
./scripts/validation/validate-testing-config.sh .github/testing-governance.yaml

# Prerequisites: Install yq
brew install yq
```

### Automated Validation

- **On Config Change**: CI validates schema on PR
- **On Test Run**: Artifacts validated against schemas

---

## Schemas

### Flake Tracking Schema

**File**: `scripts/validation/flake-tracking.schema.json`

Tracks:
- Pass/fail history per check (rolling 50-run window)
- Quarantined checks with owner and SLA
- Failure rate metrics (7-day window, last 10 runs)

### Test Run Summary Schema

**File**: `scripts/validation/test-run-summary.schema.json`

Captures:
- Run metadata (ID, timestamp, platform, trigger)
- Per-check results (status, duration, details)
- Overall status and artifacts URL

---

## Governance Policies

### Retry on Infrastructure Failure

- **Max retries**: 2
- **Retry delay**: 30 seconds
- **Triggers**: Infrastructure failure, runner timeout

### Caching Strategy

- **Cache keys**: cmake-build, juce-modules, dependencies
- **TTL**: 7 days
- **Invalidation**: On CMakeLists.txt or dependency changes

### Flake Quarantine (Planned - Phase 6)

- **Detection**: ≥3 failures in 10 runs OR ≥20% failure rate over 7 days
- **Action**: Remove from required gates, assign owner
- **SLA**: 48 hours for remediation
- **Re-enablement**: ≥95% pass rate over 14 days + owner signoff

---

## Development Status

### ✅ Completed

- [x] Directory structure and schemas
- [x] Configuration file with 14 check categories
- [x] 5 owner definitions
- [x] 3 trigger contexts (pre-commit, pre-push, pr-to-develop)
- [x] Git hooks (pre-commit, pre-push)
- [x] Hook installation script
- [x] Configuration validation script
- [x] Basic policies (retry, caching)

### 🚧 In Progress

- [ ] CI workflow integration (reading from config)
- [ ] Time budget enforcement
- [ ] Parallelization matrix
- [ ] Cache implementation in workflows

### 📋 Planned

- [ ] Nightly workflow with full platform coverage
- [ ] Flake detection and quarantine automation
- [ ] Release and hotfix workflows
- [ ] Platform-specific conditional execution
- [ ] Skip/bypass approval workflow
- [ ] Constitution amendment (Section VI)

---

## Troubleshooting

### Issue: Validation script fails with "yq not found"

**Solution**:
```bash
brew install yq
```

### Issue: Pre-push hook fails with "Build directory not found"

**Solution**: Build the project first:
```bash
mkdir build && cd build
cmake .. && cmake --build .
```

### Issue: Pre-commit hook doesn't run

**Solution**: Reinstall hooks:
```bash
./scripts/install-hooks.sh
```

---

## Contributing

When adding new check categories or modifying policies:

1. Update `testing-governance.yaml`
2. Run validation: `./scripts/validation/validate-testing-config.sh .github/testing-governance.yaml`
3. Test locally before PR
4. Document changes in PR description

---

## References

- **Full Specification**: `specs/005-test-cadence/spec.md`
- **Implementation Plan**: `specs/005-test-cadence/plan.md`
- **Task Breakdown**: `specs/005-test-cadence/tasks.md`
- **Developer Guide**: `specs/005-test-cadence/quickstart.md`
- **Progress Tracking**: `specs/005-test-cadence/IMPLEMENTATION-PROGRESS.md`

---

## Support

- **Questions**: #showmidi-dev channel
- **Issues**: GitHub Issues with `testing-governance` label
- **Documentation**: This directory and `specs/005-test-cadence/`

---

**Maintained by**: Core Team  
**Contact**: #showmidi-dev
