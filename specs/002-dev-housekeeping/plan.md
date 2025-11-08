# Implementation Plan: Development Housekeeping

**Branch**: `002-dev-housekeeping` | **Date**: 2025-11-08 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/002-dev-housekeeping/spec.md`

**Note**: This plan integrates both the original housekeeping items (CHANGELOG, CONTRIBUTING, .editorconfig, .clang-format, .gitignore, PR templates) AND the comprehensive CI/CD infrastructure you've already created.

## Summary

Establish comprehensive developer infrastructure and automation for ShowMIDI, including contribution guidelines, code formatting standards, version control hygiene, changelog management, and CI/CD workflows for automated releases, testing, and statistics tracking. This creates a professional open-source development environment that supports the GitFlow workflow and reduces manual maintenance burden.

## Technical Context

**Language/Version**: 
- Bash shell scripts (zsh-compatible for macOS developers)
- YAML (GitHub Actions workflows)
- Markdown (documentation)
- Python 3.x (for statistics/automation scripts within workflows)
- C++17 (target for formatting rules)

**Primary Dependencies**: 
- GitHub Actions (CI/CD platform)
- EditorConfig (cross-editor configuration standard)
- clang-format 14+ (C++ code formatting)
- Git 2.x (version control with GitFlow workflow)
- JUCE framework (style guide reference for .clang-format)

**Storage**: 
- Git repository (version controlled text files)
- GitHub API (release metadata, download statistics)
- Local filesystem (docs/data/ for statistics JSON, docs/images/ for generated charts)

**Testing**: 
- Workflow simulation via local scripts (test-build-local.sh)
- CI validation on PR and release branches
- Manual verification of generated files (CHANGELOG.md, STATISTICS.md)

**Target Platform**: 
- Developer workstations (macOS, Windows, Linux)
- GitHub Actions runners (ubuntu-latest, macos-latest, windows-latest)
- Git hosting (GitHub repository)

**Project Type**: Single repository with documentation and automation infrastructure

**Performance Goals**: 
- Workflow execution < 15 minutes for full build matrix
- Statistics updates < 2 minutes daily
- Documentation updates < 1 minute per commit

**Constraints**: 
- Must work within GitHub Free tier limits (2,000 CI minutes/month)
- Must not break existing build processes (Projucer, CMake)
- Must preserve manual additions in auto-generated files (copilot-instructions.md)
- Must respect GPL-3.0 licensing (headers required on scripts)

**Scale/Scope**: 
- 15 total files: 8 workflows, 3 scripts, 3 documentation files, 1 template
- 6 core configuration files: CHANGELOG.md, CONTRIBUTING.md, .editorconfig, .clang-format, .gitignore, .github/pull_request_template.md
- Covers 3 automation domains: build/test, release management, statistics/documentation

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### I. Multi-Platform Architecture ✅

**Compliance**: PASS  
**Rationale**: 
- .editorconfig, .clang-format, and .gitignore support all platforms (macOS, Windows, Linux)
- CI workflows include platform-specific build jobs (macos-latest, windows-latest, ubuntu-latest)
- CONTRIBUTING.md will document setup for all three major platforms
- Statistics and automation are platform-agnostic (GitHub Actions)

**No violations**: All configuration files and workflows are designed for cross-platform development.

---

### II. JUCE Framework Standards ✅

**Compliance**: PASS  
**Rationale**: 
- .clang-format will be configured to match JUCE coding style (Allman braces, 4-space indentation)
- .editorconfig aligns with JUCE conventions (UTF-8, LF line endings, 4-space indent)
- CONTRIBUTING.md will reference JUCE best practices (Pimpl, leak detector macros, namespace usage)
- No changes to C++ code structure - only formatting rules

**No violations**: Configuration enforces JUCE standards rather than conflicting with them.

---

### III. Real-Time Performance ✅

**Compliance**: PASS (Not Applicable)  
**Rationale**: 
- This feature adds developer tooling and documentation only
- No changes to MIDI thread, audio processing, or real-time code paths
- CI workflows run offline and do not affect runtime performance
- Statistics collection uses GitHub API (external, non-blocking)

**No violations**: No impact on real-time performance characteristics.

---

### IV. User-Centric Design ✅

**Compliance**: PASS  
**Rationale**: 
- CHANGELOG.md provides transparent version history for users
- CONTRIBUTING.md lowers barrier to entry for new contributors
- Download statistics (STATISTICS.md) provide transparency about project adoption
- PR templates improve communication quality between contributors and maintainers

**No violations**: Enhances user and contributor experience through better documentation and automation.

---

### V. Maintainability ✅

**Compliance**: PASS (Enhanced)  
**Rationale**: 
- CONTRIBUTING.md explicitly documents GPL-3.0 header requirement
- .clang-format reduces formatting debates in code review
- CHANGELOG.md provides historical context for future maintainers
- CI workflows automate repetitive tasks (release builds, statistics updates)
- Scripts include GPL-3.0 headers as required
- .gitignore prevents accidental commits of build artifacts

**No violations**: Directly supports long-term maintainability through automation and documentation.

---

### Platform Standards & Requirements ✅

**Compliance**: PASS  
**Rationale**: 
- CI workflows validate builds for all declared platforms (macOS 11.7+, Windows 10+, Linux, iOS 12+)
- .gitignore excludes platform-specific files (.DS_Store, Thumbs.db, etc.)
- Build scripts (build-macos.sh, build-windows.ps1, build-linux.sh) already exist and support declared targets
- Version synchronization (CMakeLists.txt + .jucer) is documented in CONTRIBUTING.md

**No violations**: Supports existing platform standards with improved documentation.

---

### Development Workflow ✅

**Compliance**: PASS (Enhanced)  
**Rationale**: 
- CONTRIBUTING.md codifies GitFlow workflow already documented in `.specify/memory/gitflow-workflow.md`
- PR template enforces conventional commits and checklist items
- CI workflows trigger on correct branches (PRs to develop/main, pushes to main/release/hotfix)
- CHANGELOG.md follows Keep a Changelog format, compatible with semantic versioning
- Scripts support release process (release-post-process.sh)

**No violations**: Strengthens adherence to GitFlow workflow and quality gates.

---

### Summary

**Overall Status**: ✅ FULL COMPLIANCE  
**Violations Requiring Justification**: 0  
**Constitution Enhancements**: 
- Maintainability (automation reduces manual errors)
- Development Workflow (tooling enforces documented processes)

**Proceed to Phase 0**: Approved

---

## Project Structure

### Documentation (this feature)

```text
specs/002-dev-housekeeping/
├── spec.md              # Feature specification (user scenarios, requirements)
├── plan.md              # This file (implementation plan)
├── research.md          # Phase 0 output (research findings) ✅ COMPLETE
├── data-model.md        # Phase 1 output (entity definitions) ✅ COMPLETE
├── quickstart.md        # Phase 1 output (developer guide) ✅ COMPLETE
├── contracts/           # Phase 1 output (file templates) ✅ COMPLETE
│   ├── README.md
│   └── file-migration.md
└── tasks.md             # Phase 2 output (NOT created by this command)
```

### Source Code (repository root)

This feature adds configuration, documentation, and automation files:

```text
ShowMIDI/
├── .editorconfig                    # NEW: Cross-editor formatting rules
├── .clang-format                    # NEW: C++ code formatting rules
├── .gitignore                       # UPDATED: Add multi-platform patterns
├── CHANGELOG.md                     # NEW: Version history (Keep a Changelog format)
├── CONTRIBUTING.md                  # NEW: Contributor guide
├── STATISTICS.md                    # EXISTING: Statistics dashboard (already created)
├── DOWNLOAD_STATS.md                # NEW: Detailed download statistics
├── README.md                        # UPDATED: Add links to new documentation
│
├── .github/
│   ├── copilot-instructions.md      # UPDATED: Add CI/CD technologies
│   ├── pull_request_template.md     # NEW: PR submission template
│   └── workflows/                   # NEW DIRECTORY: Move from prompts/workflows
│       ├── ci.yml                   # MOVED: Already documented in constitution
│       ├── test-build.yml           # MOVED: Test builds
│       ├── release.yml              # MOVED: Release builds (fix typo: realease→release)
│       ├── release-with-readme-update.yml  # MOVED: Post-release orchestration
│       ├── download-stats.yml       # MOVED: Daily statistics collection
│       ├── update-readme-downloads.yml  # MOVED: Update README download links
│       ├── update-file-sizes.yml    # MOVED: Update README file sizes
│       ├── changelog.yml            # MOVED: Changelog generation
│       └── README.md                # MOVED: Workflow documentation
│
├── scripts/
│   ├── test-build-local.sh          # MOVED: From .github/prompts/workflows/
│   ├── update-download-links.sh     # MOVED: From .github/prompts/workflows/
│   ├── release-post-process.sh      # RENAMED: Fix typo (relase→release)
│   ├── build-macos.sh               # EXISTING: macOS build script
│   ├── build-windows.ps1            # EXISTING: Windows build script
│   ├── build-linux.sh               # EXISTING: Linux build script
│   └── auv3_aum_size.sh             # EXISTING: AUv3 size calculator
│
├── docs/
│   ├── templates/
│   │   └── README-downloads-template.md  # EXISTING: Download section template
│   ├── data/                        # Auto-generated by workflows
│   │   └── stats_YYYYMMDD.json
│   └── images/                      # Auto-generated by workflows
│       ├── download-trends.png
│       └── platform-distribution.png
```

**Structure Decision**: This is a repository infrastructure feature that adds development tooling and automation. The structure follows GitHub conventions (.github/ for workflows and templates), standard open-source practices (CHANGELOG.md, CONTRIBUTING.md at root), and consolidates scripts in a single directory.

---

## Complexity Tracking

**Not Applicable** - No constitution violations. All gates passed with FULL COMPLIANCE (see Constitution Check section above).

---

## Post-Design Constitution Re-Evaluation

After completing Phase 0 (research) and Phase 1 (design), re-evaluating constitution compliance:

### I. Multi-Platform Architecture ✅

**Re-Evaluation**: PASS  
**Design Impact**: 
- CI/CD workflows defined for all platforms (macOS, Windows, Linux)
- Scripts support platform-specific builds (build-macos.sh, build-windows.ps1, build-linux.sh)
- .gitignore covers all platform-specific files
- .editorconfig and .clang-format are platform-agnostic

**Conclusion**: Design reinforces multi-platform support.

---

### II. JUCE Framework Standards ✅

**Re-Evaluation**: PASS  
**Design Impact**:
- .clang-format configured with Allman braces (JUCE standard)
- CONTRIBUTING.md documents JUCE patterns (leak detector, namespace, types)
- EditorConfig settings match JUCE conventions (4-space indent, UTF-8, LF)

**Conclusion**: Design codifies JUCE standards in automated tooling.

---

### III. Real-Time Performance ✅

**Re-Evaluation**: PASS  
**Design Impact**: No impact - this feature is developer tooling only, no runtime code changes.

**Conclusion**: No effect on real-time performance.

---

### IV. User-Centric Design ✅

**Re-Evaluation**: PASS (Enhanced)  
**Design Impact**:
- CHANGELOG.md provides transparent version history for users
- STATISTICS.md gives users visibility into download trends and platform adoption
- Quick start guide (quickstart.md) reduces onboarding time for new contributors

**Conclusion**: Design improves transparency and lowers contribution barriers.

---

### V. Maintainability ✅

**Re-Evaluation**: PASS (Significantly Enhanced)  
**Design Impact**:
- Automated workflows reduce manual release tasks (release.yml, changelog.yml)
- Configuration files prevent formatting debates (.editorconfig, .clang-format)
- Documentation makes onboarding faster (CONTRIBUTING.md, quickstart.md)
- CI/CD workflows catch issues before merge (ci.yml, test-build.yml)
- Statistics automation reduces manual README updates

**Conclusion**: Design significantly improves long-term maintainability through automation and documentation.

---

### Platform Standards & Requirements ✅

**Re-Evaluation**: PASS  
**Design Impact**:
- CI workflows validate all platform builds automatically
- Scripts support all declared platforms
- .gitignore prevents platform-specific file commits

**Conclusion**: Design enforces platform standards through automation.

---

### Development Workflow ✅

**Re-Evaluation**: PASS (Enhanced)  
**Design Impact**:
- CONTRIBUTING.md codifies GitFlow workflow
- PR template enforces workflow steps (conventional commits, testing checklist)
- CI workflows trigger on correct branches (align with GitFlow)
- Helper scripts support workflow (test-build-local.sh, release-post-process.sh)

**Conclusion**: Design strengthens GitFlow workflow compliance.

---

### Final Post-Design Assessment

**Overall Status**: ✅ FULL COMPLIANCE (No Violations)  
**Constitution Enhancements**:
- **Maintainability**: Significantly improved through automation
- **Development Workflow**: Codified and enforced through tooling
- **User-Centric Design**: Enhanced through transparency (CHANGELOG, STATISTICS)

**Recommendation**: Proceed with implementation (Phase 2: Tasks)

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
<!--
  ACTION REQUIRED: Replace the placeholder tree below with the concrete layout
  for this feature. Delete unused options and expand the chosen structure with
  real paths (e.g., apps/admin, packages/something). The delivered plan must
  not include Option labels.
-->

```text
# [REMOVE IF UNUSED] Option 1: Single project (DEFAULT)
src/
├── models/
├── services/
├── cli/
└── lib/

tests/
├── contract/
├── integration/
└── unit/

# [REMOVE IF UNUSED] Option 2: Web application (when "frontend" + "backend" detected)
backend/
├── src/
│   ├── models/
│   ├── services/
│   └── api/
└── tests/

frontend/
├── src/
│   ├── components/
│   ├── pages/
│   └── services/
└── tests/

# [REMOVE IF UNUSED] Option 3: Mobile + API (when "iOS/Android" detected)
api/
└── [same as backend above]

ios/ or android/
└── [platform-specific structure: feature modules, UI flows, platform tests]
```

**Structure Decision**: [Document the selected structure and reference the real
directories captured above]

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| [e.g., 4th project] | [current need] | [why 3 projects insufficient] |
| [e.g., Repository pattern] | [specific problem] | [why direct DB access insufficient] |
