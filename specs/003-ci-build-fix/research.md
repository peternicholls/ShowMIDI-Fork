# Research: CI/CD Build Infrastructure Fix

**Date**: 2025-11-08  
**Feature**: 003-ci-build-fix

## Overview

This document consolidates research findings for fixing ShowMIDI's CI/CD build infrastructure. The research focuses on: (1) GitHub Actions best practices for multi-platform builds, (2) adaptive Xcode version selection strategies, (3) CMake dependency detection patterns with graceful degradation, (4) workflow optimization techniques, and (5) CI/CD documentation standards.

---

## Research Area 1: Adaptive Xcode Version Selection on GitHub Actions Runners

### Decision
Use `xcode-select --print-path` to detect available Xcode installations and select the latest available version dynamically, with explicit path verification before use.

### Rationale
- GitHub Actions runner images regularly update and remove older Xcode versions without warning
- Hardcoded paths like `/Applications/Xcode_15.2.app` fail when that version is removed from runner image
- Adaptive selection provides resilience to runner image changes without requiring workflow updates
- Latest available Xcode ensures access to newest toolchain improvements and bug fixes
- Explicit verification prevents cryptic "invalid developer directory" errors

### Implementation Approach
```yaml
- name: Select Xcode
  run: |
    # List available Xcode versions
    ls /Applications/ | grep Xcode
    
    # Use xcode-select to get current default
    XCODE_PATH=$(xcode-select --print-path)
    echo "Current Xcode: $XCODE_PATH"
    
    # Verify path exists
    if [ ! -d "$XCODE_PATH" ]; then
      echo "Error: Xcode path does not exist: $XCODE_PATH"
      exit 1
    fi
    
    # Set for subsequent steps
    sudo xcode-select --switch "$XCODE_PATH"
    xcodebuild -version
```

### Alternatives Considered
- **Hardcoded version** (current approach): Rejected - brittle, requires manual updates, fails when runner image changes
- **Fallback list** (try 15.2, then 15.1, then 15.0): Rejected - still requires maintenance, doesn't adapt to new versions (e.g., 16.0)
- **Pin runner image version**: Rejected - prevents access to security updates and new features, eventually unsupported

### References
- GitHub Actions runner images: https://github.com/actions/runner-images/blob/main/images/macos/macos-13-Readme.md
- Xcode selection best practices: https://github.com/actions/runner-images/issues/6350

---

## Research Area 2: CMake Dependency Detection with Graceful Degradation

### Decision
Implement optional dependency detection using CMake's `find_package()` with `QUIET` and conditional target compilation based on availability. Emit clear warning messages when optional dependencies are missing.

### Rationale
- CLAP and LV2 are optional plugin formats; their absence should not block primary formats (Standalone, VST3, AU)
- Graceful degradation improves developer experience by allowing partial builds
- Clear warnings ensure developers know which formats are unavailable and why
- Aligns with clarification decision: "Skip building that plugin format and emit a warning"
- Reduces build brittleness - missing optional dependencies don't cause complete failure

### Implementation Approach
```cmake
# JUCE framework - REQUIRED
if(DEFINED ENV{PATH_TO_JUCE})
    set(JUCE_DIR $ENV{PATH_TO_JUCE})
    message(STATUS "Using JUCE from PATH_TO_JUCE: ${JUCE_DIR}")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
    message(STATUS "Using JUCE from submodule: ${JUCE_DIR}")
else()
    message(FATAL_ERROR "JUCE not found. Set PATH_TO_JUCE or initialize JUCE submodule.")
endif()

add_subdirectory(${JUCE_DIR} JUCE)

# CLAP extensions - OPTIONAL
set(CLAP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/clap-juce-extensions")
if(EXISTS "${CLAP_DIR}/CMakeLists.txt")
    message(STATUS "CLAP extensions found at: ${CLAP_DIR}")
    set(BUILD_CLAP ON)
    add_subdirectory(${CLAP_DIR})
else()
    message(WARNING "CLAP extensions not found at ${CLAP_DIR}. CLAP plugin format will be skipped.")
    set(BUILD_CLAP OFF)
endif()

# Conditional CLAP target
if(BUILD_CLAP)
    juce_add_plugin_format(ShowMIDI FORMATS CLAP)
endif()
```

### Alternatives Considered
- **Fail immediately**: Rejected - prevents developers from working on core functionality when optional dependencies unavailable
- **Auto-download dependencies**: Rejected - increases build complexity, network dependency, security concerns
- **Silent degradation**: Rejected - confusing for developers who expect a format but don't get it

### References
- CMake find_package documentation: https://cmake.org/cmake/help/latest/command/find_package.html
- JUCE CMake integration: https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md

---

## Research Area 3: GitHub Actions Workflow Path Filtering

### Decision
Use `paths-ignore` for documentation-only changes to skip heavy build jobs, combined with `paths` filters to ensure code changes trigger builds.

### Rationale
- Documentation-only PRs (README.md, CONTRIBUTING.md, etc.) don't require full platform builds
- Skipping unnecessary builds saves CI minutes and reduces developer wait time
- Path filtering is native GitHub Actions feature with good performance
- Meets success criteria: "Documentation-only PRs skip heavy build workflows, completing in under 30 seconds"

### Implementation Approach
```yaml
name: CI Build

on:
  pull_request:
    branches: [develop, main]
    paths-ignore:
      - '**.md'
      - 'docs/**'
      - '*.txt'
      - 'LICENSE'
      - 'COPYING.md'
  push:
    branches: [main, release/**, hotfix/**]
    paths-ignore:
      - '**.md'
      - 'docs/**'
      - '*.txt'

jobs:
  build-linux:
    runs-on: ubuntu-latest
    # This job will be skipped if only ignored paths changed
    steps:
      - name: Checkout
        uses: actions/checkout@v4
        with:
          submodules: recursive
```

### Alternatives Considered
- **Manual workflow_dispatch only**: Rejected - forces manual triggering, loses automation benefits
- **Separate lightweight workflow for docs**: Rejected - adds complexity, still requires path filtering logic
- **Changed files analysis in workflow**: Rejected - path-ignore is simpler and more efficient

### References
- GitHub Actions path filtering: https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions#onpushpull_requestpull_request_targetpathspaths-ignore
- Workflow optimization patterns: https://docs.github.com/en/actions/using-workflows/events-that-trigger-workflows

---

## Research Area 4: Workflow Concurrency Control and Timeout Handling

### Decision
Implement concurrency groups with `cancel-in-progress: true` to cancel outdated runs, and use `timeout-minutes` with GitHub Actions' built-in retry on timeout (via matrix strategy or manual retry logic).

### Rationale
- Multiple rapid commits to PR waste resources running outdated builds
- Concurrency control meets success criteria: "cancel previous in-progress runs within 10 seconds"
- Timeout retry handles transient infrastructure issues (slow runners, network hiccups)
- Single auto-retry balances automation with visibility (failed retry indicates genuine problem)

### Implementation Approach
```yaml
name: CI Build

on:
  pull_request:
    branches: [develop, main]

concurrency:
  group: ci-${{ github.ref }}
  cancel-in-progress: true

jobs:
  build-macos:
    runs-on: macos-latest
    timeout-minutes: 30
    steps:
      - name: Checkout
        uses: actions/checkout@v4
        with:
          submodules: recursive
      
      # If timeout occurs, GitHub Actions will fail the job
      # For retry, use matrix strategy or conditional re-run
```

For explicit retry logic:
```yaml
jobs:
  build-macos:
    runs-on: macos-latest
    timeout-minutes: 30
    continue-on-error: true
    steps:
      - name: Build
        id: build
        run: |
          # Build commands
          
  build-macos-retry:
    needs: build-macos
    if: failure()
    runs-on: macos-latest
    timeout-minutes: 30
    steps:
      - name: Retry Build
        run: |
          # Same build commands
```

### Alternatives Considered
- **No retry**: Rejected - transient failures would require manual re-run, poor developer experience
- **Multiple retries**: Rejected - masks genuine problems, wastes CI minutes
- **Incremental timeout increases**: Rejected - adds complexity, doesn't address root cause of timeouts

### References
- GitHub Actions concurrency: https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions#concurrency
- Timeout handling: https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions#jobsjob_idtimeout-minutes

---

## Research Area 5: CI/CD Documentation Best Practices

### Decision
Add comprehensive CI/CD section to CONTRIBUTING.md covering: (1) workflow triggers and when they run, (2) local testing procedures, (3) common failure scenarios with troubleshooting steps, (4) how to interpret CI results. Add inline YAML comments for non-obvious configuration choices.

### Rationale
- Documentation improves maintainability and reduces support burden
- Self-service troubleshooting meets success criteria: "90% of workflow failures provide actionable error messages"
- Inline comments explain "why" decisions (adaptive Xcode, path filters, concurrency)
- Aligns with constitution principle V (Maintainability)

### Implementation Approach

**CONTRIBUTING.md section structure:**
```markdown
## CI/CD Pipeline

### Workflow Triggers

| Workflow | Triggers | Purpose |
|----------|----------|---------|
| CI Build | PR → develop/main, push → main/release/hotfix | Validate builds across all platforms |
| Test Build | Push → develop | Quick build matrix test |
| Changelog | Tags v*.*.* only | Generate release notes |

### Local Testing

Before opening PR:
1. Build on macOS: `cd Builds/MacOSX && xcodebuild ...`
2. Build on Linux: `cmake -B build && cmake --build build`
3. Verify no compiler warnings (warnings treated as errors in CI)

### Common Failures

**"invalid developer directory" (macOS)**
- Cause: Xcode version mismatch
- Solution: Workflow automatically selects latest available Xcode
- If persistent: Check runner image Xcode versions

**"JUCE not found" (Linux)**
- Cause: Submodules not initialized
- Solution: Workflow runs `checkout` with `submodules: recursive`
- Local fix: `git submodule update --init --recursive`
```

**Inline YAML comments:**
```yaml
# Adaptive Xcode selection: uses latest available version on runner
# to prevent failures when GitHub updates runner images
- name: Select Xcode
  run: |
    xcode-select --print-path
```

### Alternatives Considered
- **Separate CI.md file**: Rejected - fragments documentation, harder to discover
- **Wiki-only documentation**: Rejected - not version controlled, can diverge from code
- **Minimal comments**: Rejected - doesn't meet "clear explanation" success criteria

### References
- Documentation best practices: https://docs.github.com/en/communities/setting-up-your-project-for-healthy-contributions/setting-guidelines-for-repository-contributors
- Inline workflow comments: https://yaml.org/spec/1.2.2/#comments

---

## Summary

All research areas resolved with clear decisions:

1. ✅ **Adaptive Xcode Selection**: Use `xcode-select` with path verification, select latest available
2. ✅ **Graceful Dependency Degradation**: CMake optional dependencies with clear warnings
3. ✅ **Path Filtering**: Use `paths-ignore` for doc-only changes to skip heavy builds
4. ✅ **Concurrency & Timeout**: Cancel in-progress runs, single auto-retry on timeout
5. ✅ **Documentation**: Comprehensive CONTRIBUTING.md section + inline YAML comments

**No unresolved questions remaining.** Ready to proceed to Phase 1 (Design & Contracts).
