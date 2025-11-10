# Parallel Execution Guide: CI/CD Build Infrastructure Fix

**Feature**: 003-ci-build-fix  
**Branch**: `003-ci-build-fix`  
**Date**: 2025-11-09

## 🎯 Parallel Execution Strategy

**Phase 1 (Setup)** - Sequential, quick verification tasks (can be done by one agent)

**Phase 2 (Foundation)** - Must complete before parallel work begins

**After Phase 2 completes**, here are the optimal parallel agent assignments:

---

## 👥 Agent Window Assignments

### **Agent 1: CMake Core & Linux Configuration** 
*Focus: User Story 1 - Linux build infrastructure*

```
You are working on feature 003-ci-build-fix on branch 003-ci-build-fix.

Review the implementation plan at specs/003-ci-build-fix/plan.md and task breakdown at specs/003-ci-build-fix/tasks.md.

Complete the following tasks in sequence:

Phase 3 - Linux CMake Configuration:
- T009: Add system dependency detection for ALSA in CMakeLists.txt with clear error message if missing
- T010: Add system dependency detection for X11 in CMakeLists.txt with clear error message if missing  
- T011: Add system dependency detection for Freetype2 in CMakeLists.txt with clear error message if missing
- T012: Configure Linux compiler flags in CMakeLists.txt: -Wall -Wextra -Werror

Phase 3 - Linux Workflow Configuration:
- T013: Update .github/workflows/ci.yml Linux job to install system dependencies (libasound2-dev, libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libfreetype6-dev)
- T014: Verify .github/workflows/ci.yml Linux job checks out submodules recursively
- T015: Set PATH_TO_JUCE environment variable in .github/workflows/ci.yml Linux job to ${{ github.workspace }}/JUCE
- T019: Configure artifact upload for Linux builds in .github/workflows/ci.yml (Standalone, VST3, LV2 if built)

Reference research.md for CMake dependency detection patterns and workflow-contracts.md for expected behavior.

Commit changes with conventional commits format: "feat(ci): add Linux CMake dependency detection"
```

---

### **Agent 2: macOS/Xcode Configuration**
*Focus: User Story 2 - Xcode adaptive selection*

```
You are working on feature 003-ci-build-fix on branch 003-ci-build-fix.

Review the implementation plan at specs/003-ci-build-fix/plan.md and task breakdown at specs/003-ci-build-fix/tasks.md.

Complete the following tasks in sequence:

Phase 4 - Adaptive Xcode Selection:
- T022: Add Xcode version detection step in .github/workflows/ci.yml macOS job to list available Xcode installations
- T023: Implement Xcode path verification in .github/workflows/ci.yml macOS job using xcode-select --print-path
- T024: Add explicit path existence check in .github/workflows/ci.yml macOS job before using Xcode
- T025: Add xcodebuild -version output step in .github/workflows/ci.yml macOS job for visibility

Phase 4 - macOS Build Configuration:
- T026: Verify .github/workflows/ci.yml macOS job checks out submodules recursively
- T027: Verify code signing disabled in .github/workflows/ci.yml macOS job
- T028: Configure artifact upload for macOS builds in .github/workflows/ci.yml (Standalone, VST3, AU)

Reference research.md section "Adaptive Xcode Version Selection" for implementation approach.

Commit changes with conventional commits format: "feat(ci): implement adaptive Xcode version selection"
```

---

### **Agent 3: Windows Configuration**
*Focus: User Story 1 - Windows build infrastructure*

```
You are working on feature 003-ci-build-fix on branch 003-ci-build-fix.

Review the implementation plan at specs/003-ci-build-fix/plan.md and task breakdown at specs/003-ci-build-fix/tasks.md.

Complete the following tasks in sequence:

Phase 3 - Windows Workflow Configuration:
- T016: Verify .github/workflows/ci.yml Windows job checks out submodules recursively
- T017: Verify MSVC environment setup in .github/workflows/ci.yml Windows job
- T018: Set consistent build type (Release) in .github/workflows/ci.yml Windows job
- T020: Configure artifact upload for Windows builds in .github/workflows/ci.yml (Standalone, VST3)
- T021: Set artifact retention to 90 days in .github/workflows/ci.yml

Reference workflow-contracts.md for Windows build job requirements.

Commit changes with conventional commits format: "feat(ci): configure Windows build environment"
```

---

### **Agent 4: CLAP Plugin Support**
*Focus: User Story 4 - Optional CLAP format (INDEPENDENT)*

```
You are working on feature 003-ci-build-fix on branch 003-ci-build-fix.

Review the implementation plan at specs/003-ci-build-fix/plan.md and task breakdown at specs/003-ci-build-fix/tasks.md.

Complete the following tasks in sequence:

Phase 6 - CLAP CMake Detection:
- T039: Add CLAP extensions directory detection in CMakeLists.txt (libs/clap-juce-extensions)
- T040: Implement conditional BUILD_CLAP flag in CMakeLists.txt based on directory existence
- T041: Add WARNING message when CLAP extensions missing in CMakeLists.txt (not FATAL_ERROR)
- T042: Add STATUS message when CLAP extensions found in CMakeLists.txt
- T043: Add conditional CLAP plugin format target in CMakeLists.txt (only if BUILD_CLAP=ON)

Phase 6 - CLAP Workflow Integration:
- T044: Update artifact upload in .github/workflows/ci.yml to include CLAP artifacts if built
- T045: Add inline comment in CMakeLists.txt explaining CLAP optional dependency handling

Reference research.md section "CMake Dependency Detection with Graceful Degradation" for the graceful degradation pattern.

Commit changes with conventional commits format: "feat(build): add optional CLAP plugin format support"
```

---

## 📋 Execution Plan

### Step 1: Setup (Single Agent)
Run Phase 1 tasks T001-T005 (verification only, quick)

### Step 2: Foundation (Single Agent)
Run Phase 2 tasks T006-T008 (BLOCKING - must complete before parallel work)

**Test**: `cmake -B build_test -DCMAKE_BUILD_TYPE=Release`

### Step 3: Parallel Execution (4 Agents)
```
Agent 1: Linux configuration (T009-T015, T019)
Agent 2: macOS/Xcode configuration (T022-T028)  
Agent 3: Windows configuration (T016-T018, T020-T021)
Agent 4: CLAP support (T039-T045)
```

**All 4 agents can work simultaneously - no dependencies between them**

### Step 4: Integration Test
After all agents complete:
- Push changes
- Create PR
- Verify all CI jobs pass (US1, US2 acceptance criteria met)

### Step 5: Workflow Optimization (After Step 4)
Phase 5 tasks T029-T038 (depends on working builds)

### Step 6: Documentation (After all implementation)
Phase 7 tasks T046-T057 (documents everything)

### Step 7: Polish
Phase 8 tasks T058-T068 (final validation)

---

## ⚡ Quick Start Commands

### For each agent, start with:
```bash
cd /Users/peternicholls/Dev/ShowMIDI.git
git checkout 003-ci-build-fix
git pull origin 003-ci-build-fix
```

### Option A: Separate Branches (Recommended for clean separation)
```bash
# Agent 1
git checkout -b 003-ci-build-fix-linux

# Agent 2  
git checkout -b 003-ci-build-fix-macos

# Agent 3
git checkout -b 003-ci-build-fix-windows

# Agent 4
git checkout -b 003-ci-build-fix-clap
```

### Option B: Same Branch (If coordinating file sections)
Work directly on `003-ci-build-fix` branch with careful coordination:
- **Agent 1**: CMakeLists.txt (Linux sections) + ci.yml (Linux job)
- **Agent 2**: .github/workflows/ci.yml (macOS job only)
- **Agent 3**: .github/workflows/ci.yml (Windows job only)
- **Agent 4**: CMakeLists.txt (CLAP sections) + ci.yml (CLAP artifacts)

**Note**: Agent 1 and Agent 4 both edit CMakeLists.txt - coordinate to avoid conflicts or use separate branches

---

## 🔀 File Conflict Matrix

| File | Agent 1 | Agent 2 | Agent 3 | Agent 4 | Strategy |
|------|---------|---------|---------|---------|----------|
| CMakeLists.txt | ✅ Linux deps | ❌ | ❌ | ✅ CLAP | Separate branches or sequential |
| .github/workflows/ci.yml | ✅ Linux job | ✅ macOS job | ✅ Windows job | ✅ CLAP artifacts | Different sections - can parallel |

**Recommended**: Use separate branches for Agent 1 and Agent 4, merge sequentially to avoid CMakeLists.txt conflicts.

---

## ⏱️ Time Estimate

- **Setup**: 10 minutes (1 agent)
- **Foundation**: 20 minutes (1 agent)  
- **Parallel work**: 30-45 minutes (4 agents simultaneously)
- **Integration**: 15 minutes
- **Total**: ~1-1.5 hours vs 3+ hours sequential

---

## 📝 Coordination Notes

### Before Starting Parallel Work
1. ✅ Complete Phase 1 (Setup) - verify environment
2. ✅ Complete Phase 2 (Foundation) - JUCE path resolution
3. ✅ Test Foundation: `cmake -B build_test` should work
4. ✅ Push Foundation changes to remote
5. ✅ All agents pull latest `003-ci-build-fix`

### During Parallel Work
- Agents 2 & 3: Can work on same branch (different jobs in ci.yml)
- Agents 1 & 4: Should use separate branches (both edit CMakeLists.txt)
- Coordinate in shared doc/chat which sections each agent owns

### After Parallel Work
1. Agent 2 & 3 push to `003-ci-build-fix`
2. Agent 1 creates PR: `003-ci-build-fix-linux` → `003-ci-build-fix`
3. Agent 4 creates PR: `003-ci-build-fix-clap` → `003-ci-build-fix`
4. Merge sequentially: Linux → CLAP (or vice versa)
5. Test complete integration

---

## ✅ Success Criteria Per Agent

### Agent 1 (Linux)
- CMake finds ALSA, X11, Freetype2 on Linux
- Linux job installs correct system dependencies
- Linux artifacts uploaded (Standalone, VST3)
- Compiler flags include `-Wall -Wextra -Werror`

### Agent 2 (macOS)
- Xcode version detected and verified
- No "invalid developer directory" errors
- macOS artifacts uploaded (Standalone, VST3, AU)
- Code signing disabled

### Agent 3 (Windows)
- Submodules checked out recursively
- MSVC environment configured
- Windows artifacts uploaded (Standalone, VST3)
- Artifact retention set to 90 days

### Agent 4 (CLAP)
- CLAP extensions detected if present
- BUILD_CLAP flag set conditionally
- WARNING (not error) when CLAP missing
- CLAP artifacts uploaded if built

---

## 🚨 Common Issues & Solutions

### Issue: CMakeLists.txt Merge Conflict
**Solution**: Use separate branches for Agent 1 and Agent 4, merge sequentially

### Issue: ci.yml Formatting Inconsistencies
**Solution**: Use consistent YAML indentation (2 spaces), validate with `yamllint`

### Issue: Agent Can't Find Spec Files
**Solution**: Ensure all agents are in `/Users/peternicholls/Dev/ShowMIDI.git` directory

### Issue: Foundation Phase Not Complete
**Solution**: Block parallel work until Phase 2 tested successfully with `cmake -B build_test`

---

**Document Version**: 1.0  
**Last Updated**: 2025-11-09  
**Ready for Execution**: ✅ Yes
