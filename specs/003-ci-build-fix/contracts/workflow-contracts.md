# Workflow Contracts

**Date**: 2025-11-08  
**Feature**: 003-ci-build-fix

## Overview

This document defines the interface contracts for GitHub Actions workflows. While CI/CD workflows don't use traditional API contracts (REST/GraphQL), they do have well-defined input/output schemas that govern their behavior.

---

## Contract: CI Build Workflow

**File**: `.github/workflows/ci.yml`

### Input Schema (Triggers)

```yaml
on:
  pull_request:
    branches:
      - develop
      - main
    paths-ignore:
      - '**.md'
      - 'docs/**'
      - '*.txt'
      - 'LICENSE'
      - 'COPYING.md'
  
  push:
    branches:
      - main
      - 'release/**'
      - 'hotfix/**'
    paths-ignore:
      - '**.md'
      - 'docs/**'
      - '*.txt'
```

**Trigger Conditions**:
- ✅ Run when: PR to develop/main with code changes
- ✅ Run when: Push to main/release/hotfix with code changes
- ❌ Skip when: Only documentation files changed
- ❌ Skip when: Push to develop (tested at PR time)

### Output Schema (Artifacts)

**Success Case** (all jobs pass):
```yaml
artifacts:
  - name: ShowMIDI-Linux-Standalone
    path: build/ShowMIDI_artefacts/Release/Standalone/ShowMIDI
    retention-days: 90
  
  - name: ShowMIDI-Linux-VST3
    path: build/ShowMIDI_artefacts/Release/VST3/ShowMIDI.vst3
    retention-days: 90
  
  - name: ShowMIDI-macOS-Standalone
    path: Builds/MacOSX/build/Release/ShowMIDI.app
    retention-days: 90
  
  - name: ShowMIDI-macOS-VST3
    path: Builds/MacOSX/build/Release/ShowMIDI.vst3
    retention-days: 90
  
  - name: ShowMIDI-macOS-AU
    path: Builds/MacOSX/build/Release/ShowMIDI.component
    retention-days: 90
  
  - name: ShowMIDI-Windows-Standalone
    path: Builds/VisualStudio2022/x64/Release/Standalone Plugin/ShowMIDI.exe
    retention-days: 90
  
  - name: ShowMIDI-Windows-VST3
    path: Builds/VisualStudio2022/x64/Release/VST3/ShowMIDI.vst3
    retention-days: 90

status: success
exit_code: 0
```

**Failure Case** (any job fails):
```yaml
status: failure
exit_code: 1
failed_job: build-linux | build-macos | build-windows | code-quality
error_message: "Descriptive error indicating root cause"
```

### Concurrency Behavior

```yaml
concurrency:
  group: ci-${{ github.ref }}
  cancel-in-progress: true
```

**Contract**: When new commit pushed to same branch:
- Previous in-progress run MUST be cancelled within 10 seconds
- New run MUST start fresh (no state carried over)
- Cancelled runs MUST appear with "Cancelled" status in GitHub UI

---

## Contract: Build Job (Linux)

**Job Name**: `build-linux`  
**Runner**: `ubuntu-latest`

### Input Requirements

```yaml
inputs:
  submodules: recursive  # JUCE framework must be checked out
  dependencies:
    - cmake (>= 3.15)
    - build-essential
    - libasound2-dev
    - libx11-dev
    - libxrandr-dev
    - libxinerama-dev
    - libxcursor-dev
    - libfreetype6-dev
```

### Environment Variables

```yaml
env:
  CMAKE_BUILD_TYPE: Release
  PATH_TO_JUCE: ${{ github.workspace }}/JUCE  # Optional override
```

### Success Criteria

```yaml
preconditions:
  - JUCE submodule initialized
  - All system dependencies installed

postconditions:
  - CMake configuration completes without errors
  - Compilation completes with 0 warnings (warnings-as-errors enabled)
  - Standalone executable created at build/ShowMIDI_artefacts/Release/Standalone/ShowMIDI
  - VST3 plugin created at build/ShowMIDI_artefacts/Release/VST3/ShowMIDI.vst3
  - Optional: LV2 plugin created if dependencies found

performance:
  - CMake configuration < 2 minutes
  - Full build < 10 minutes
  - Total job time < 15 minutes
```

### Error Scenarios

**Scenario 1: JUCE Not Found**
```yaml
exit_code: 1
error_message: "JUCE not found. Set PATH_TO_JUCE or initialize JUCE submodule."
resolution: "Ensure checkout action has 'submodules: recursive'"
```

**Scenario 2: Missing System Dependency**
```yaml
exit_code: 1
error_message: "Could not find ALSA (missing: ALSA_LIBRARY ALSA_INCLUDE_DIR)"
resolution: "Install libasound2-dev: sudo apt-get install -y libasound2-dev"
```

**Scenario 3: Compilation Warning**
```yaml
exit_code: 1
error_message: "warning: unused variable 'foo' [-Wunused-variable]"
resolution: "Fix compiler warning in source code (warnings treated as errors)"
```

---

## Contract: Build Job (macOS)

**Job Name**: `build-macos`  
**Runner**: `macos-latest`

### Input Requirements

```yaml
inputs:
  submodules: recursive
  xcode_version: latest  # Adaptively selected
```

### Xcode Selection Contract

**Preconditions**:
- macOS runner has Xcode installed
- At least one Xcode version available in /Applications/

**Process**:
```bash
# 1. List available Xcode versions
ls /Applications/ | grep Xcode

# 2. Get current default Xcode path
XCODE_PATH=$(xcode-select --print-path)

# 3. Verify path exists
if [ ! -d "$XCODE_PATH" ]; then
  exit 1  # FAIL
fi

# 4. Set Xcode for build
sudo xcode-select --switch "$XCODE_PATH"

# 5. Verify selection
xcodebuild -version
```

**Postconditions**:
- `xcode-select --print-path` returns valid path
- `xcodebuild -version` succeeds
- No "invalid developer directory" errors

**Failure Contract**:
- If no Xcode found: Exit 1 with error "No Xcode installation found"
- If path verification fails: Exit 1 with error "Xcode path does not exist: {path}"

### Build Success Criteria

```yaml
postconditions:
  - Standalone app created: Builds/MacOSX/build/Release/ShowMIDI.app
  - VST3 plugin created: Builds/MacOSX/build/Release/ShowMIDI.vst3
  - AU plugin created: Builds/MacOSX/build/Release/ShowMIDI.component
  - Optional: CLAP plugin created if clap-juce-extensions found
  - Code signing disabled (skip signing in CI)
  - Universal binary (x86_64 + arm64) or native architecture

performance:
  - Xcode selection < 30 seconds
  - Full build < 15 minutes
  - Total job time < 20 minutes
```

---

## Contract: Build Job (Windows)

**Job Name**: `build-windows`  
**Runner**: `windows-latest`

### Input Requirements

```yaml
inputs:
  submodules: recursive
  visual_studio_version: "2022"
```

### MSVC Configuration Contract

**Environment Setup**:
```powershell
# 1. Locate Visual Studio installation
vswhere -latest -property installationPath

# 2. Initialize MSVC environment
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

# 3. Verify CMake available
cmake --version
```

### Build Success Criteria

```yaml
postconditions:
  - Standalone exe created: Builds/VisualStudio2022/x64/Release/Standalone Plugin/ShowMIDI.exe
  - VST3 plugin created: Builds/VisualStudio2022/x64/Release/VST3/ShowMIDI.vst3
  - Optional: CLAP plugin created if clap-juce-extensions found

performance:
  - MSVC setup < 1 minute
  - Full build < 20 minutes
  - Total job time < 25 minutes
```

---

## Contract: CMake Configuration

**File**: `CMakeLists.txt`

### Dependency Resolution Contract

**JUCE Framework (Required)**:
```cmake
# Priority order:
# 1. PATH_TO_JUCE environment variable
if(DEFINED ENV{PATH_TO_JUCE})
    set(JUCE_DIR $ENV{PATH_TO_JUCE})
    message(STATUS "Using JUCE from PATH_TO_JUCE: ${JUCE_DIR}")
    
# 2. Git submodule at ./JUCE
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
    message(STATUS "Using JUCE from submodule: ${JUCE_DIR}")
    
# 3. Fail if neither found
else()
    message(FATAL_ERROR "JUCE not found. Set PATH_TO_JUCE or initialize JUCE submodule.")
endif()
```

**Contract**:
- MUST check PATH_TO_JUCE first (explicit override)
- MUST fall back to submodule if env var not set
- MUST emit informative STATUS message showing which path used
- MUST emit FATAL_ERROR with resolution steps if neither found

**CLAP Extensions (Optional)**:
```cmake
set(CLAP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/clap-juce-extensions")
if(EXISTS "${CLAP_DIR}/CMakeLists.txt")
    message(STATUS "CLAP extensions found at: ${CLAP_DIR}")
    set(BUILD_CLAP ON)
    add_subdirectory(${CLAP_DIR})
else()
    message(WARNING "CLAP extensions not found at ${CLAP_DIR}. CLAP plugin format will be skipped.")
    set(BUILD_CLAP OFF)
endif()
```

**Contract**:
- MUST check if clap-juce-extensions directory exists
- MUST set BUILD_CLAP flag based on availability
- MUST emit WARNING (not ERROR) if missing
- MUST continue configuration with other formats

**System Libraries (Platform-Specific Required)**:
```cmake
# Linux
if(UNIX AND NOT APPLE)
    find_package(ALSA REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Freetype REQUIRED)
    # Error message if not found includes package name
endif()

# macOS
if(APPLE)
    find_library(CORE_AUDIO CoreAudio REQUIRED)
    find_library(CORE_MIDI CoreMIDI REQUIRED)
endif()
```

**Contract**:
- MUST use `REQUIRED` keyword for mandatory dependencies
- MUST fail configuration with clear error if system library missing
- Error message MUST include package name and installation hint

---

## Contract: Timeout and Retry Behavior

### Timeout Configuration

```yaml
jobs:
  build-macos:
    timeout-minutes: 30  # Per job timeout
```

**Contract**:
- Job MUST be cancelled if exceeds timeout-minutes
- GitHub Actions MUST mark job as "Timeout"
- Exit code: special timeout indicator (not 0 or 1)

### Retry Strategy

**Implementation Option 1: Dependent Job Retry**
```yaml
jobs:
  build-macos:
    timeout-minutes: 30
    continue-on-error: true
    id: first-attempt
  
  build-macos-retry:
    needs: build-macos
    if: failure()  # Only run if first attempt failed
    timeout-minutes: 30
```

**Contract**:
- Retry job MUST only run if first attempt failed
- Retry job MUST have same timeout as original
- If retry succeeds: Overall status = success
- If retry fails: Overall status = failure, error message indicates "Failed after retry"

---

## Contract: Path Filtering

### Documentation-Only Changes

**Filter Definition**:
```yaml
paths-ignore:
  - '**.md'
  - 'docs/**'
  - '*.txt'
  - 'LICENSE'
  - 'COPYING.md'
```

**Behavior Contract**:
```
Input: PR changes = [README.md, CONTRIBUTING.md]
Output: Workflow skipped, status = "Skipped", no jobs run

Input: PR changes = [README.md, Source/Main.cpp]
Output: Workflow runs, all jobs execute (code change overrides ignore)

Input: PR changes = [docs/api.md, docs/images/logo.png]
Output: Workflow skipped (docs/** pattern matches)
```

**Performance Contract**:
- Skipped workflows MUST complete GitHub Actions decision < 5 seconds
- Skipped workflows MUST show "Skipped" status (not "Success" or "Failure")
- Total PR check time for doc-only changes < 30 seconds

---

## Summary

This contract specification defines:

1. ✅ **Workflow Triggers**: When workflows run (PR, push, tag patterns)
2. ✅ **Artifact Outputs**: What files are uploaded and where
3. ✅ **Job Inputs**: Required dependencies and environment setup
4. ✅ **Success Criteria**: Postconditions and performance targets
5. ✅ **Error Scenarios**: Common failures with resolution steps
6. ✅ **Adaptive Behavior**: Xcode selection, dependency resolution priorities
7. ✅ **Timeout/Retry**: Recovery strategies for transient failures
8. ✅ **Path Filtering**: Skip rules for documentation-only changes

All contracts support the feature requirements and align with clarification decisions.
