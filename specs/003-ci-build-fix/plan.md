# Implementation Plan: CI/CD Build Infrastructure Fix

**Branch**: `003-ci-build-fix` | **Date**: 2025-11-08 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/003-ci-build-fix/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/commands/plan.md` for the execution workflow.

## Summary

Fix all CI/CD build infrastructure issues to enable successful automated builds across all platforms (Linux, macOS, Windows). Primary issues include: (1) hardcoded invalid Xcode version causing macOS build failures, (2) CMake configuration errors on Linux runners, (3) missing workflow optimizations (path filters, concurrency control), (4) CLAP plugin build support, and (5) inadequate workflow documentation. Solution involves adaptive Xcode version selection, robust dependency resolution with graceful degradation for optional formats, intelligent workflow triggers with path-based filtering, and comprehensive inline documentation.

## Technical Context

**Language/Version**: YAML (GitHub Actions workflow syntax), CMake 3.15+, Shell scripting (Bash/PowerShell)
**Primary Dependencies**: GitHub Actions runners (ubuntu-latest, macos-latest, windows-latest), JUCE 7.0.5, CMake, Xcode (macOS), Visual Studio 2022 (Windows), system libraries (ALSA, X11, Freetype on Linux)
**Storage**: N/A (CI/CD configuration only)
**Testing**: Workflow validation via actual CI runs, local CMake builds for verification
**Target Platform**: GitHub Actions cloud infrastructure (Linux Ubuntu 22.04, macOS 13, Windows Server 2022)
**Project Type**: Build infrastructure / CI/CD pipeline (no source code changes to ShowMIDI application itself)
**Performance Goals**: CMake configuration <2min on Linux, macOS builds <15min, Windows builds <20min, doc-only PRs <30s, concurrency cancellation <10s
**Constraints**: <5% spurious failure rate, GitHub Actions timeout limits (360min/job), runner image tool versions may change, must maintain backward compatibility with existing local build processes
**Scale/Scope**: 3 platform build jobs (Linux/macOS/Windows), 5 plugin formats (Standalone/VST3/AU/LV2/CLAP), ~5 workflow files to modify, affects all contributors and PR validation

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### I. Multi-Platform Architecture
✅ **PASS** - This feature explicitly enables multi-platform CI/CD validation (Linux, macOS, Windows) with all plugin formats (VST3, AU, Standalone, LV2, CLAP). CMake and Projucer build paths are both maintained. Adaptive Xcode selection ensures resilience to runner image changes.

### II. JUCE Framework Standards
✅ **PASS** - No JUCE code changes required. Build infrastructure respects JUCE framework requirements (C++17, system library dependencies). CMake configuration properly handles JUCE submodule and PATH_TO_JUCE environment variable.

### III. Real-Time Performance
✅ **PASS** - No impact on runtime performance. This is purely build infrastructure; does not affect MIDI message handling, UI responsiveness, or threading model.

### IV. User-Centric Design
✅ **PASS** - Improves developer experience (contributors are users of CI/CD). Fast feedback (<2min config, <15min macOS builds), clear error messages (90% actionable), efficient triggers (skip doc-only builds). Documentation enables self-service troubleshooting.

### V. Maintainability
✅ **PASS** - Enhances maintainability through: (1) adaptive Xcode selection reduces manual updates when runners change, (2) inline workflow comments explain non-obvious choices, (3) comprehensive CI/CD documentation in CONTRIBUTING.md, (4) graceful degradation for optional dependencies prevents brittle builds. No GPL-3.0 header changes needed (workflow YAML files exempt).

**Overall Status**: ✅ ALL GATES PASS - No constitution violations. Feature aligns with maintainability and multi-platform principles.

## Project Structure

### Documentation (this feature)

```text
specs/[###-feature]/
├── plan.md              # This file (/speckit.plan command output)
├── research.md          # Phase 0 output (/speckit.plan command)
├── data-model.md        # Phase 1 output (/speckit.plan command)
├── quickstart.md        # Phase 1 output (/speckit.plan command)
├── contracts/           # Phase 1 output (/speckit.plan command)
└── tasks.md             # Phase 2 output (/speckit.tasks command - NOT created by /speckit.plan)
```

### Source Code (repository root)

```text
.github/
├── workflows/
│   ├── ci.yml                    # Main CI workflow (PR/push validation) - MODIFY
│   ├── test-build.yml           # Platform build matrix - MODIFY
│   ├── changelog.yml            # Release changelog generation - MODIFY (triggers)
│   └── [other workflows]        # Review for consistency
├── PULL_REQUEST_TEMPLATE.md     # May need CI troubleshooting guidance - REVIEW

CMakeLists.txt                   # Root CMake config - MODIFY (JUCE path, CLAP, dependency detection)

CONTRIBUTING.md                  # Add CI/CD section - MODIFY

libs/
├── clap-juce-extensions/        # CLAP plugin support (git submodule) - VERIFY EXISTS
└── vst2/                        # VST2 SDK (user-provided) - OPTIONAL

JUCE/                            # JUCE framework (git submodule) - VERIFY INITIALIZED

Builds/
├── MacOSX/                      # Xcode projects (Projucer-generated) - NO CHANGES
├── VisualStudio2022/            # VS solutions (Projucer-generated) - NO CHANGES
└── LinuxMakefile/               # Legacy makefiles - NO CHANGES

Source/                          # ShowMIDI C++ source - NO CHANGES (build infra only)
```

**Structure Decision**: This is a CI/CD infrastructure feature that modifies workflow YAML files in `.github/workflows/`, CMake configuration, and documentation. No changes to ShowMIDI application source code (`Source/`), JUCE code, or platform-specific build projects (Xcode/VS). The feature maintains separation between build configuration (CMake) and generated build files (Builds/).

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

No violations - this section is intentionally empty. All constitution gates passed.

---

## Phase 0: Research & Analysis

**Status**: ✅ Complete  
**Output**: [research.md](./research.md)

### Research Areas Completed

1. ✅ **Adaptive Xcode Version Selection** - Use `xcode-select --print-path` with path verification
2. ✅ **CMake Dependency Detection with Graceful Degradation** - Optional dependencies skip format with warning
3. ✅ **GitHub Actions Workflow Path Filtering** - `paths-ignore` for documentation-only changes
4. ✅ **Workflow Concurrency Control and Timeout Handling** - Cancel in-progress, single auto-retry
5. ✅ **CI/CD Documentation Best Practices** - CONTRIBUTING.md section + inline YAML comments

**Key Decisions**:
- JUCE path priority: PATH_TO_JUCE env var → submodule → fail
- Optional dependencies (CLAP, LV2): Skip format + warn if missing
- Xcode selection: Latest available on runner (adaptive to image changes)
- Workflow triggers: Path-ignore for docs, concurrency groups per branch
- Timeout retry: Single automatic retry, fail with notification if retry times out

---

## Phase 1: Design & Contracts

**Status**: ✅ Complete  
**Outputs**: 
- [data-model.md](./data-model.md)
- [contracts/workflow-contracts.md](./contracts/workflow-contracts.md)
- [quickstart.md](./quickstart.md)
- Agent context updated: `.github/copilot-instructions.md`

### Data Model Entities

1. **GitHubActionsWorkflow** - Orchestrates automated builds with triggers, concurrency, jobs
2. **BuildConfiguration** - CMake settings, dependency resolution, plugin format flags
3. **PluginFormat** - Output types (Standalone, VST3, AU, LV2, CLAP) with platform matrix
4. **ExternalDependency** - Third-party libraries (JUCE, ALSA, etc.) with resolution methods
5. **CIEnvironment** - GitHub Actions runner configurations (Ubuntu, macOS, Windows)
6. **BuildArtifact** - Compiled outputs uploaded for testing
7. **WorkflowTrigger** - Event patterns (PR, push, tag) with path filters

### Workflow Contracts

Defined input/output schemas for:
- **CI Build Workflow**: Triggers, artifacts, concurrency behavior
- **Build Jobs**: Linux (CMake), macOS (Xcode), Windows (Visual Studio)
- **CMake Configuration**: Dependency resolution priority, error handling
- **Timeout/Retry**: Recovery strategy contracts
- **Path Filtering**: Documentation-only change behavior

### Agent Context Update

✅ Updated `.github/copilot-instructions.md` with:
- Technologies: YAML, CMake 3.15+, Shell scripting
- Frameworks: GitHub Actions, JUCE 7.0.5, platform build tools
- Project type: Build infrastructure / CI/CD pipeline

---

## Phase 2: Task Breakdown

**Status**: ⏸️ Not started - Use `/speckit.tasks` command to generate tasks.md

This phase will decompose the implementation into concrete tasks covering:
- Workflow YAML modifications (ci.yml, test-build.yml, changelog.yml)
- CMake configuration updates (JUCE path, CLAP detection, warnings)
- Documentation additions (CONTRIBUTING.md CI/CD section)
- Testing and validation procedures

**Next Command**: `/speckit.tasks` to generate task breakdown from this plan.

---

## Constitution Re-Check (Post-Design)

*Re-evaluating constitution compliance after Phase 1 design:*

### I. Multi-Platform Architecture
✅ **PASS** - Design maintains all plugin formats across all platforms. Path filtering and graceful degradation ensure builds succeed even with missing optional dependencies.

### II. JUCE Framework Standards
✅ **PASS** - CMake configuration respects JUCE idioms. No JUCE code modified.

### III. Real-Time Performance
✅ **PASS** - No runtime impact. Build infrastructure only.

### IV. User-Centric Design
✅ **PASS** - Quickstart guide provides clear contributor workflows. Error scenarios documented with resolutions. Performance targets specified (<2min config, <15min macOS builds).

### V. Maintainability
✅ **PASS** - Comprehensive documentation (research.md, quickstart.md, contracts), adaptive behaviors reduce maintenance burden, inline comments explain non-obvious choices.

**Final Status**: ✅ ALL GATES PASS - Design maintains constitution compliance. Ready for task breakdown and implementation.
