# Coverage Policy and Enforcement

**Version**: 1.0.0  
**Feature**: 004-tdd-adoption  
**Effective**: Week 1 of TDD Adoption

## Overview

This document defines code coverage targets, measurement methodology, enforcement timeline, and exceptions for ShowMIDI's TDD adoption.

## Coverage Targets

### Phase-Based Milestones

| Phase | Timeline | Coverage Goal | Enforcement Level | Scope |
|-------|----------|---------------|-------------------|-------|
| **0 Seeding** | Week 1 | 90% of critical flows (qualitative) | Report-only | Manual identification of critical paths |
| **1 Stabilize** | Weeks 2–3 | 50% overall line coverage | Report-only | All Source/ files |
| **2 Expansion** | Weeks 4–6 | 70% overall line coverage | Soft gate (warn on PR) | All Source/ files |
| **3 Enforcement** | Weeks 7–8 | 80% overall line coverage | Hard gate (block merges for new code) | New/modified files only |
| **4 Optimization** | Week 9+ | 80%+ sustained, push towards 85% | Hard gate maintained | All code |

### Critical Flows (Week 1 Target: 90%)

**Qualitative Definition**: User-visible behaviors that, if broken, render ShowMIDI unusable.

**Identified Critical Flows**:
1. **MIDI Message Reception**: MidiInput → DeviceListener → MidiDeviceComponent display
2. **Channel Auto-Discovery**: New MIDI device appears in sidebar automatically
3. **Channel Auto-Hide**: Inactive channels disappear after timeout
4. **Theme Loading**: User drops SVG file → theme applies
5. **Keyboard Shortcuts**: Spacebar (pause/resume), Delete (reset), V (toggle viz)
6. **Plugin Initialization**: Host loads plugin → UI renders without crash
7. **Standalone Device Selection**: User selects MIDI input from dropdown
8. **Settings Persistence**: Preferences saved on quit, restored on launch
9. **Multi-Channel Display**: 16 channels visualized simultaneously
10. **High-Throughput MIDI**: Dense CC automation processed without drops

**Coverage Measurement**: Each flow MUST have at least one integration or system test by end of Week 1.

### Line Coverage (Weeks 2–8)

**Measurement Tool**: 
- Linux: `lcov` (gcov-based)
- macOS: `llvm-cov` (Clang-based)
- Windows: `OpenCppCoverage`

**Scope**: All `.cpp` files in `Source/` directory

**Exclusions**:
- `JuceLibraryCode/` (generated code)
- `JUCE/` (framework code)
- `libs/` (third-party dependencies)
- `Build/` (platform-specific build artifacts)
- `Tests/` (test code itself not covered)

**Formula**:
```
Line Coverage % = (Covered Lines / Total Executable Lines) × 100
```

**Thresholds**:
- **Below 50%** (Weeks 1–3): Informational only
- **50–69%** (Weeks 4–6): Soft gate (PR warning, can merge with approval)
- **70–79%** (Weeks 7–8): Soft gate for all code, hard gate for new code
- **80%+** (Week 9+): Hard gate for all code

---

## Enforcement Policies

### Reporting Phase (Weeks 1–3)

**Policy**: Coverage data collected and reported, but does NOT block merges.

**GitHub Actions**:
```yaml
- name: Check coverage
  run: |
    coverage=$(lcov --summary coverage_filtered.info | grep "lines" | awk '{print $2}' | sed 's/%//')
    echo "Current coverage: ${coverage}%"
    echo "Target: 50% (informational only)"
```

**PR Comment Bot** (optional):
```
📊 **Coverage Report**
- Current: 45.3%
- Target (Week 3): 50%
- Trend: +2.1% from last PR ✅
- Status: ℹ️ Informational (no blocking)
```

### Soft Gate (Weeks 4–6)

**Policy**: PRs with coverage <70% trigger warnings, but can merge with maintainer approval.

**GitHub Actions**:
```yaml
- name: Check coverage soft gate
  run: |
    coverage=$(lcov --summary coverage_filtered.info | grep "lines" | awk '{print $2}' | sed 's/%//')
    if (( $(echo "$coverage < 70" | bc -l) )); then
      echo "⚠️ Coverage ${coverage}% below soft gate (70%)"
      echo "::warning::Coverage below target. Consider adding tests before merging."
    else
      echo "✅ Coverage ${coverage}% meets soft gate"
    fi
```

**PR Status**: Warning badge (yellow), not blocking

**Override**: Maintainer can approve with comment: "Acknowledged low coverage, tracked in issue #XXX"

### Hard Gate (Weeks 7–8)

**Policy**: New/modified code MUST have ≥80% coverage; legacy code allowed <80% if unchanged.

**Incremental Coverage Check**:
```bash
# Compare coverage only on changed files
git diff --name-only origin/develop | grep -E '^Source/.*\.(cpp|h)$' > changed_files.txt
lcov --extract coverage_filtered.info $(cat changed_files.txt) --output-file new_coverage.info
new_coverage=$(lcov --summary new_coverage.info | grep "lines" | awk '{print $2}' | sed 's/%//')

if (( $(echo "$new_coverage < 80" | bc -l) )); then
  echo "❌ New code coverage ${new_coverage}% below hard gate (80%)"
  exit 1
fi
```

**PR Status**: Failing check (blocks merge)

**Exception Process**: See "Exceptions" section below

### Sustained Enforcement (Week 9+)

**Policy**: All code (new and legacy) MUST maintain ≥80% coverage.

**Legacy Remediation**: By Week 9, all previously untested code should have baseline coverage added incrementally.

---

## Coverage Measurement

### Local Development

**Build with Coverage**:
```bash
# Linux/macOS
cmake -B build -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build
lcov --capture --directory build --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/JUCE/*' --output-file coverage_filtered.info
genhtml coverage_filtered.info --output-directory coverage_html
open coverage_html/index.html  # View report
```

**Windows**:
```powershell
cmake -B build -DBUILD_TESTS=ON
cmake --build build --config Debug
OpenCppCoverage.exe --sources Source --excluded_sources JUCE `
  --export_type html:coverage_html -- ctest --test-dir build -C Debug
start coverage_html/index.html
```

### CI Pipeline

**Automated Report Upload**:
- **Linux**: Codecov integration (primary)
- **macOS**: Codecov (secondary validation)
- **Windows**: Codecov (tertiary validation)

**Codecov Configuration** (`.codecov.yml`):
```yaml
coverage:
  status:
    project:
      default:
        target: 80%
        threshold: 2%  # Allow 2% drop without failing
    patch:
      default:
        target: 80%   # New code must be 80%+
        
ignore:
  - "JuceLibraryCode/**"
  - "JUCE/**"
  - "libs/**"
  - "Tests/**"
```

**Badge Display** (README.md):
```markdown
[![Coverage](https://codecov.io/gh/owner/ShowMIDI/branch/develop/graph/badge.svg)](https://codecov.io/gh/owner/ShowMIDI)
```

---

## Exceptions

### When to Request Exception

Coverage exceptions may be granted for:

1. **Platform-Specific Code**: Hardware-dependent logic untestable in CI (e.g., ASIO audio buffer handling on Windows)
2. **GUI-Heavy Components**: Complex UI rendering requiring visual inspection (e.g., custom OpenGL visualizations)
3. **Third-Party Integration**: Vendor SDK calls without mock/stub support
4. **Performance-Critical Paths**: Code where test overhead distorts profiling (rare)

### Exception Request Process

**Template** (GitHub Issue):
```markdown
## Coverage Exception Request

**File**: Source/MyComponent.cpp  
**Lines**: 145–180  
**Current Coverage**: 45%  
**Target Coverage**: N/A (exception requested)

**Justification**:
This code interacts with Windows ASIO driver APIs that require physical audio hardware.
Mocking ASIO is impractical due to complex driver state machine.

**Alternative Validation**:
- Manual testing on physical devices (documented in TESTING.md)
- Integration test with simulated buffer (covers logic, not driver calls)

**Approver**: @maintainer-username
```

**Approval Criteria**:
- Justification is specific and detailed
- Alternative validation method provided
- Code review shows no obvious testability improvements

**Tracking**: Exceptions tracked in `.codecov.yml`:
```yaml
ignore:
  - "Source/AsioDriverInterface.cpp:145-180"  # Exception #12: Hardware-dependent
```

---

## Coverage Anti-Patterns

### Avoid

❌ **Testing for Coverage Percentage**: Writing meaningless tests just to hit 80%
```cpp
// BAD: No assertions, just executes code
beginTest("Coverage filler");
component.doSomething();
// No expect() calls
```

❌ **Ignoring Untested Branches**: Hitting line coverage but missing edge cases
```cpp
// 100% line coverage, 50% branch coverage
if (condition) {
    doA();  // Tested ✅
} else {
    doB();  // Never tested ❌
}
```

❌ **Mocking Everything**: Over-mocking defeats integration testing
```cpp
// BAD: Mocking JUCE String is pointless
MockString mockString;
mockString.setReturnValue("test");
```

### Encourage

✅ **Meaningful Assertions**: Tests validate behavior, coverage is byproduct
```cpp
beginTest("MIDI Note On updates channel state");
mockDevice.sendMessage(MidiMessage::noteOn(1, 60, 0.8f));
expect(component.isChannelActive(1));  // Real assertion
```

✅ **Branch Coverage Focus**: Explicitly test if/else, switch cases
```cpp
beginTest("Channel hides after timeout");
mockTime.advance(5.0);  // Timeout condition = true
expect(!component.isChannelVisible(1));

beginTest("Channel stays visible during activity");
mockTime.advance(2.0);  // Timeout condition = false
mockDevice.sendMessage(MidiMessage::noteOn(1, 60, 0.8f));
expect(component.isChannelVisible(1));
```

✅ **Adapter Pattern Over Mocking**: Real JUCE objects in tests when possible
```cpp
// GOOD: Real MidiBuffer, fake adapter
MidiBuffer realBuffer;
MockMidiAdapter fakeDevice;
fakeDevice.setBuffer(realBuffer);  // Adapter wraps real JUCE type
```

---

## Dashboard and Reporting

### Weekly Coverage Report (Automated)

**GitHub Actions** (runs every Monday):
```yaml
schedule:
  - cron: '0 9 * * 1'  # 9 AM UTC Monday

steps:
  - name: Generate weekly report
    run: |
      echo "## Week $(date +%U) Coverage Report" > report.md
      echo "- Overall: ${coverage}%" >> report.md
      echo "- Top 5 untested files:" >> report.md
      lcov --list coverage_filtered.info | sort -k4 -n | head -5 >> report.md
  
  - name: Post to Discord
    uses: sarisia/actions-status-discord@v1
    with:
      webhook: ${{ secrets.DISCORD_WEBHOOK }}
      title: "Weekly Coverage Report"
      description: $(cat report.md)
```

### PR Coverage Diff

**GitHub Actions** (on pull request):
```yaml
- name: Compare coverage
  run: |
    base_coverage=$(curl -s https://codecov.io/api/gh/owner/ShowMIDI/branch/develop | jq '.commit.totals.c')
    pr_coverage=$(lcov --summary coverage_filtered.info | grep "lines" | awk '{print $2}' | sed 's/%//')
    diff=$(echo "$pr_coverage - $base_coverage" | bc)
    
    if (( $(echo "$diff > 0" | bc -l) )); then
      echo "✅ Coverage improved by ${diff}%"
    else
      echo "⚠️ Coverage decreased by ${diff}%"
    fi
```

---

## Questions?

**Coverage Not Generating**: Check build type (`CMAKE_BUILD_TYPE=Coverage`)  
**False Positives**: Review `.codecov.yml` ignore patterns  
**Exception Requests**: Open GitHub issue with template above

---

**Version History**:
- 1.0.0 (2025-11-11): Initial coverage policy for TDD adoption
