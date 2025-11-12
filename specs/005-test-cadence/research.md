# Research: Phase 11 Testing Cadence & Governance (Run 1)

**Date**: 2025-11-12  
**Feature**: 005-test-cadence  
**Purpose**: Resolve unknowns and document technology/pattern decisions for testing governance framework

---

## Decision 1: Check Taxonomy & Tooling

**What was chosen**: 12-category taxonomy with specific tool recommendations per category

| Category | Purpose | Tools/Approach | Rationale |
|----------|---------|----------------|-----------|
| **Unit** | Test individual components in isolation | JUCE UnitTest (existing), CTest runner | Already adopted in 004-tdd-adoption; JUCE-native integration |
| **Integration** | Test component interactions, module boundaries | JUCE UnitTest with fixtures, mock adapters | Builds on unit infrastructure; constitution requires adapter pattern (IMidiDeviceAdapter) |
| **System** | End-to-end flows (standalone launch, plugin load) | Automated UI testing (headless where possible), smoke tests | Validates user stories; constitution requires multi-platform parity |
| **Performance** | Latency, throughput, memory benchmarks | Custom benchmarks (JUCE high-resolution timer), profiling tools | Constitution mandates <10ms latency; must be measurable |
| **UI/Visual Regression** | Detect unintended visual changes | Screenshot comparison (e.g., Percy, Chromatic) or manual review | DPI-aware design principle; theme support requires visual validation |
| **Static Analysis** | Code quality, bug detection | clang-tidy, cppcheck | C++17 JUCE codebase; detect memory issues, undefined behavior |
| **Formatting** | Code style consistency | clang-format (existing `.clang-format` from 004) | Constitution requires JUCE naming conventions; auto-fix on pre-commit |
| **Linting** | Style/idiom enforcement | clang-tidy (style checks), custom scripts for GPL headers | GPL-3.0 header requirement (constitution V. Maintainability) |
| **Security (SAST)** | Static application security testing | GitHub CodeQL, SonarCloud (free for public repos) | Public repo; detect vulnerabilities early |
| **Security (SCA)** | Software composition analysis (dependency scanning) | GitHub Dependabot, OWASP Dependency-Check | Track JUCE, VST3 SDK, submodule vulnerabilities |
| **License Compliance** | Verify GPL-3.0 compatibility of dependencies | FOSSA, license-checker scripts | Constitution requires GPL-3.0; detect incompatible licenses |
| **Build Verification** | Compile across platforms/configurations | Existing CI jobs (macOS Xcode, Windows VS2022, Linux CMake) | Constitution mandates multi-platform builds |
| **Packaging/Installer** | Validate distribution artifacts | Smoke tests: install, launch, uninstall on target OS | Constitution requires plugin formats (VST/VST3/AU/LV2/CLAP) |

**Alternatives considered**:
- **Catch2/Google Test** for unit testing → Rejected: JUCE UnitTest already integrated, switching adds migration cost
- **Jenkins/CircleCI** for CI → Rejected: GitHub Actions native integration, unlimited minutes for public repos
- **Manual visual review only** → Rejected: Doesn't scale; DPI-aware design requires automated validation

---

## Decision 2: Trigger Strategy & Local Hooks

**What was chosen**: Git hooks (pre-commit, pre-push) + GitHub Actions triggers

### Local Hooks

| Hook | Checks | Rationale |
|------|--------|-----------|
| **pre-commit** | clang-format (auto-fix), GPL header check | Fast (<10s); auto-fix reduces friction; catches style before push |
| **pre-push** | Unit tests (CTest) | Catches logic errors locally; 1-2 min budget; prevents broken pushes to remote |

**Implementation**: Use `.git/hooks/` or Husky-like tool (e.g., `pre-commit` framework for cross-platform consistency)

### Remote Triggers (GitHub Actions)

| Event | Scope | Checks | Time Budget |
|-------|-------|--------|-------------|
| **Pull Request** | feature → develop, any → main | Unit, integration, static analysis, format/lint, build verification, High/Critical security | Median ≤5 min, p95 ≤10 min |
| **On-merge (develop)** | Post-merge validation | Full required suite re-run | ≤15 min |
| **Nightly (cron)** | develop branch | All categories except packaging/installer; all platforms (with change detection) | ≤60 min (skipped if no changes) |
| **Release branch** | release/* | Nightly suite + packaging + installer smoke | ≤90 min |
| **Hotfix branch** | hotfix/* | Safety-critical subset (unit, integration, build, High+ security) | ≤20 min (fast turnaround) |
| **Main (tag)** | Post-release validation | Full suite including all platforms/formats | No time limit (blocking artifact publish) |

**Alternatives considered**:
- **Pre-commit unit tests** → Rejected: Too slow (1-2 min), breaks TDD flow; moved to pre-push
- **PR on all platforms** → Rejected: Exceeds 10 min budget; macOS primary + nightly comprehensive coverage balances speed/confidence
- **No change detection** → Rejected: Wastes CI capacity on unchanged nightly runs

---

## Decision 3: Flake Detection & Quarantine Workflow

**What was chosen**: Automated flake detection with 3/10 failure threshold

### Detection Algorithm

```
IF (failures_in_last_10_runs >= 3) OR (failure_rate_over_7_days >= 20%)
THEN
  flag_as_flaky()
  quarantine()  # Remove from required gates
  assign_to_owner()
  create_tracking_issue(SLA: 2 business days median)
END
```

### Quarantine Process

1. **Detection**: CI pipeline tracks pass/fail per check over rolling window
2. **Notification**: Slack/GitHub issue auto-created, owner assigned (from CODEOWNERS or FR-008 mapping)
3. **Quarantine**: Check moved to "advisory" status; PR can merge without passing
4. **Remediation**: Owner investigates (timing? platform-specific? external dependency?)
5. **Re-enablement**: After fix, check must sustain ≥95% pass rate over 14 days before returning to required gates

**Ownership Mapping** (FR-008):
- Unit tests: Component owner (e.g., MIDI team for MidiDeviceComponent tests)
- Integration tests: Feature owner
- Platform-specific: Platform maintainer (macOS/Windows/Linux lead)
- Security/license: Security team or release manager

**Alternatives considered**:
- **Manual flake triage** → Rejected: Doesn't scale; delays remediation
- **5/20 threshold (permissive)** → Rejected: Misses genuine flakes; 3/10 is industry standard
- **Immediate blocking** → Rejected: One-off transients would block PRs; rolling window smooths noise

---

## Decision 4: Platform Coverage Strategy

**What was chosen**: Tiered coverage by trigger context

### PR (feature → develop)

- **Primary**: macOS (ubuntu-latest runner for speed; macOS most common dev platform)
- **Conditional**: Platform-specific checks if diff touches `#ifdef JUCE_MAC`, `#ifdef JUCE_WINDOWS`, etc.
- **Rationale**: Balances speed (≤10 min p95) with risk (nightly catches cross-platform regressions)

### Nightly

- **Comprehensive**: macOS, Windows, Linux (all supported platforms)
- **Change Detection**: Skip if `git diff main..develop --name-only | grep -E '\.(cpp|h|cmake|jucer)$'` is empty since last successful run
- **Rationale**: Public repo unlimited minutes; comprehensive coverage without blocking PRs

### Release

- **Full Validation**: All platforms, all formats (VST/VST3/AU/LV2/CLAP), packaging, installer smoke
- **No Change Detection**: Always run (release artifacts must be fully validated)
- **Rationale**: Constitution mandates multi-platform parity; production readiness requires exhaustive checks

**Alternatives considered**:
- **PR all platforms** → Rejected: Exceeds time budget; nightly sufficient for cross-platform confidence
- **Nightly macOS only** → Rejected: Misses Windows/Linux-specific regressions; unlimited minutes allow comprehensive coverage

---

## Decision 5: Time Budget Enforcement

**What was chosen**: Fail-fast + parallel execution

### Strategies

1. **Parallelization**: Run independent jobs concurrently (e.g., macOS build + Linux build in parallel)
2. **Fail-fast**: Abort remaining jobs if any required check fails (saves CI time; faster feedback)
3. **Caching**: Cache JUCE builds, dependencies (CMake `ccache`, GitHub Actions cache)
4. **Selective Execution**: Run only changed test suites (e.g., if only `Source/Theme/` changed, skip MIDI tests)

### Budget Allocation (PR context)

| Check Category | Budget | Enforcement |
|----------------|--------|-------------|
| Build verification | 3 min | Cached dependencies; incremental builds |
| Unit tests | 2 min | Parallelized by module; fail-fast on first failure |
| Integration tests | 3 min | Cached fixtures; selective execution |
| Static analysis | 1.5 min | Cached compile database; incremental analysis |
| Format/lint | 0.5 min | Cached formatting index |
| **Total (p50)** | **4-5 min** | Sum of parallel critical path |
| **Total (p95)** | **8-10 min** | Includes retry overhead, cold cache |

**Alternatives considered**:
- **No time limits** → Rejected: Slow feedback degrades TDD; constitution emphasizes velocity
- **10s format/lint budget** → Rejected: Unrealistic for full codebase; 30s acceptable with caching

---

## Decision 6: Skip/Bypass Governance

**What was chosen**: Documented exception process with audit trail

### Policy

1. **Allowed Scenarios**:
   - Multi-feature integration (spec FR-006 exception): Temporary test breakage during coordinated merge
   - Hotfix urgency: Critical production bug requires fast-track (must document risk acceptance)
   - Flaky test blocking urgent PR: Quarantine workflow allows merge with owner assignment

2. **Required Documentation**:
   - GitHub PR comment: "Skip: [category] — Reason: [justification] — Approved by: [@maintainer]"
   - Tracking issue: Link to remediation plan (SLA: 1 week for multi-feature; 2 business days for hotfix follow-up)

3. **Audit Trail**:
   - CI logs skip events with reason, approver, timestamp
   - Monthly report: All skip events reviewed by release manager

**Alternatives considered**:
- **No skip allowed** → Rejected: Hotfix urgency and multi-feature scenarios require flexibility
- **Self-service skip** → Rejected: Abuse risk; requires approval for accountability

---

## Decision 7: Constitution Amendment Deliverable

**What was chosen**: Explicit amendment proposal as outcome of Phase 11

### Amendment Structure

New **Section VI: Testing & Quality Assurance** in constitution.md:

```markdown
## VI. Testing & Quality Assurance

ShowMIDI maintains comprehensive testing practices to uphold constitutional principles:

### Test Taxonomy
- **Unit**: Component isolation (JUCE UnitTest)
- **Integration**: Module boundaries (mock adapters)
- **System**: End-to-end flows (smoke tests)
- **Performance**: <10ms latency validation (benchmarks)
- **Visual**: DPI-aware design validation (screenshot comparison)
- **Static**: Code quality (clang-tidy, cppcheck)
- **Security**: SAST/SCA (CodeQL, Dependabot)
- **License**: GPL-3.0 compliance (FOSSA)

### Trigger Cadence
- **Local**: Pre-commit (format/lint), pre-push (unit)
- **PR**: Unit, integration, static, build (median ≤5 min)
- **Nightly**: Comprehensive + all platforms (change detection)
- **Release**: Full validation + packaging/installer

### Governance
- **Flake Detection**: 3/10 failures → quarantine (2 business day SLA)
- **Platform Coverage**: PR macOS primary; nightly all platforms
- **Progressive Gates**: feature → develop → release → main (increasing rigor)
- **Skip/Bypass**: Documented exception with approval and audit trail
```

**Rationale**: Codifies practices to prevent regression; aligns with constitution's amendment process (semantic versioning for constitution itself)

**Alternatives considered**:
- **Separate testing policy document** → Rejected: Constitution is single source of truth; fragmentation reduces discoverability
- **Inline in GitFlow workflow doc** → Rejected: Scope mismatch; constitution is architectural, not procedural

---

## Integration Patterns

### Pattern 1: GitHub Actions Workflow Structure

**Approach**: Extend existing `.github/workflows/ci.yml` with structured jobs

```yaml
name: CI

on:
  pull_request:
    branches: [develop, main]
  push:
    branches: [main, 'release/**', 'hotfix/**']
  schedule:
    - cron: '0 2 * * *'  # Nightly at 2 AM UTC

jobs:
  detect-changes:
    # Change detection for nightly optimization
    
  unit-tests:
    needs: detect-changes
    if: needs.detect-changes.outputs.has_changes == 'true'
    strategy:
      matrix:
        os: [macos-latest, ubuntu-latest, windows-latest]
    # ...
    
  flake-detector:
    needs: [unit-tests, integration-tests]
    # Analyze pass/fail history, flag flaky tests
```

**Rationale**: Reuses existing infrastructure; modular jobs enable selective execution

### Pattern 2: Pre-commit Hook Installation

**Approach**: Add setup script `scripts/install-hooks.sh`

```bash
#!/bin/bash
# Install pre-commit and pre-push hooks
cp scripts/hooks/pre-commit .git/hooks/
cp scripts/hooks/pre-push .git/hooks/
chmod +x .git/hooks/pre-commit .git/hooks/pre-push
```

**Rationale**: Cross-platform (Bash/Zsh); manual install avoids forcing workflow on contributors

### Pattern 3: Flake Tracking Database

**Approach**: CI artifact (JSON) tracking pass/fail history

```json
{
  "check_id": "unit_tests_MidiDeviceComponent",
  "history": [
    {"run_id": 12345, "result": "pass", "timestamp": "2025-11-12T10:00:00Z"},
    {"run_id": 12346, "result": "fail", "timestamp": "2025-11-12T11:00:00Z"}
  ],
  "failure_rate_7d": 0.15,
  "status": "active"
}
```

**Rationale**: Lightweight; no external database; CI artifact storage is free

---

## Best Practices

1. **Determinism First**: All tests must use seeded randomness (`juce::Random::setSeedRandomly()` → `Random(42)`)
2. **Environment Isolation**: Reset state between tests (fixtures with setup/teardown)
3. **Parallel Safety**: No shared mutable state in tests (each test owns its data)
4. **Fast Feedback**: Unit tests <100ms each; integration <1s; system <10s
5. **Clear Failures**: Assertions include context (`EXPECT(result == expected, "Expected X for input Y")`)
6. **Platform Parity**: Same test suite runs on all platforms (conditional skip with `#if JUCE_MAC` only for platform-specific features)

---

## Open Questions (for Runs 2-3)

1. **UI/Visual Regression Tooling**: Percy vs Chromatic vs manual review? Cost/benefit for open-source project?
2. **Performance Baseline Storage**: Where to persist benchmark results for regression detection? Git LFS? CI artifacts?
3. **Multi-feature Integration Staging**: Dedicated `staging/*` branch or use `develop` with feature flags?
4. **Hotfix Safety-Critical Subset**: Exact scope of "safety-critical" tests? All MIDI + build, or broader?

These will be explored in Runs 2 and 3 with alternative approaches.
