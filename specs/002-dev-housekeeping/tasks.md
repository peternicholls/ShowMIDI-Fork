# Implementation Tasks: Development Housekeeping

**Feature**: 002-dev-housekeeping  
**Branch**: `002-dev-housekeeping`  
**Date**: 2025-11-08

---

## Overview

This document breaks down the implementation of development housekeeping infrastructure into concrete, executable tasks organized by user story. Each phase represents a complete, independently testable increment.

**Total Files**: 21 (6 new configs, 6 new docs, 8 moved workflows, 3 moved/renamed scripts, 3 updated files)

**User Stories**:
- **P1 Stories** (Critical): Version History Transparency, Contribution Guidelines
- **P2 Stories** (Important): Consistent Code Formatting, Automated C++ Style Enforcement
- **P3 Stories** (Quality-of-Life): Clean Repository Hygiene, Structured Pull Request Submissions

---

## Task Summary

| Phase | User Story | Task Count | Can Parallelize |
|-------|-----------|------------|-----------------|
| 1 | Setup | 3 | Yes (all) |
| 2 | Foundational | 4 | Yes (3 of 4) |
| 3 | US1 - Version History | 3 | Yes (all) |
| 4 | US2 - Contribution Guidelines | 2 | Yes (all) |
| 5 | US3 - Code Formatting | 3 | Yes (2 of 3) |
| 6 | US4 - C++ Style Enforcement | 3 | Yes (2 of 3) |
| 7 | US5 - Repository Hygiene | 1 | N/A |
| 8 | US6 - PR Templates | 1 | N/A |
| 9 | CI/CD Integration | 13 | Yes (11 of 13) |
| 10 | Polish & Validation | 4 | Yes (2 of 4) |
| **Total** | | **37** | **27 parallelizable** |

---

## Phase 1: Setup

**Goal**: Create directory structure and prepare for file migration

**Independent Test**: Verify directories exist and are ready to receive files

### Tasks

- [X] T001 [P] Create `.github/workflows/` directory for workflow files
- [X] T002 [P] Verify `scripts/` directory exists (should already exist)
- [X] T003 [P] Create `docs/data/` and `docs/images/` directories for auto-generated content

**Validation**:
```bash
# All directories should exist
test -d .github/workflows && echo "✓ workflows dir"
test -d scripts && echo "✓ scripts dir"
test -d docs/data && echo "✓ docs/data dir"
test -d docs/images && echo "✓ docs/images dir"
```

---

## Phase 2: Foundational (Blocking Prerequisites)

**Goal**: Create core configuration files that other stories depend on

**Independent Test**: Verify configuration files exist and are valid

### Tasks

- [X] T004 [P] Create `.editorconfig` file at repository root with cross-editor formatting rules (charset=utf-8, end_of_line=lf, indent_style=space, indent_size=4 for C++, trim_trailing_whitespace=true, insert_final_newline=true)
- [X] T005 [P] Create `.clang-format` file at repository root with JUCE-compatible settings (BasedOnStyle=LLVM, BreakBeforeBraces=Allman, IndentWidth=4, PointerAlignment=Left, ColumnLimit=120, Standard=c++17)
- [X] T006 [P] Update `.gitignore` file to add macOS (.DS_Store, .AppleDouble), Windows (Thumbs.db, Desktop.ini), Linux (*~, .directory), IDE (.vscode/, .idea/, .clangd/), and CMake (build/, cmake-build-*) patterns
- [X] T007 Validate `.editorconfig` and `.clang-format` against existing Source/ directory (expect < 5% formatting changes per SC-004)

**Validation**:
```bash
# Validate EditorConfig syntax
editorconfig-checker .editorconfig

# Test clang-format on sample file (dry-run)
clang-format --dry-run Source/PluginProcessor.cpp

# Verify gitignore patterns
git check-ignore .DS_Store build/ .vscode/
```

**Dependencies**: None (foundational tasks)

---

## Phase 3: User Story 1 - Version History Transparency (P1)

**Story Goal**: Users and contributors can view a comprehensive history of all changes, bug fixes, and new features across versions to understand what has changed between releases.

**Independent Test**: Release a test version and verify CHANGELOG.md contains all changes with proper semantic versioning headers (acceptance scenario 1-3 from spec.md).

### Tasks

- [X] T008 [P] [US1] Create `CHANGELOG.md` file at repository root with Keep a Changelog format structure (preamble, [Unreleased] section, version 1.0.1 and 1.0.0 entries based on git history)
- [X] T009 [P] [US1] Populate CHANGELOG.md version 1.0.0 section with initial release changes (Added: Real-time MIDI visualization, plugin formats, standalone app, theme system)
- [X] T010 [P] [US1] Populate CHANGELOG.md version 1.0.1 section with bug fixes and changes from git history between v1.0.0 and v1.0.1

**Validation**:
```bash
# Verify Keep a Changelog format
grep -q "^# Changelog" CHANGELOG.md
grep -q "## \[Unreleased\]" CHANGELOG.md
grep -q "## \[1.0.1\] - 2025-" CHANGELOG.md
grep -q "## \[1.0.0\] - 2025-" CHANGELOG.md

# Verify categories present
grep -q "### Added" CHANGELOG.md
grep -q "### Fixed" CHANGELOG.md
```

**Dependencies**: None (independent story)

**Acceptance Criteria Met**:
- ✓ Scenario 1: All changes organized by type (Added, Changed, Fixed)
- ✓ Scenario 2: Versions in reverse chronological order with dates
- ✓ Scenario 3: Each change has description (issue/PR links added manually in future)

---

## Phase 4: User Story 2 - Contribution Guidelines (P1)

**Story Goal**: External contributors can quickly understand how to set up the development environment, follow the GitFlow workflow, and submit quality pull requests that meet project standards.

**Independent Test**: Have a new contributor follow CONTRIBUTING.md to set up environment, create a feature branch, make changes, and submit a PR - verifying all steps are clear and complete (acceptance scenario 1-4 from spec.md).

### Tasks

- [X] T011 [P] [US2] Create `CONTRIBUTING.md` file at repository root with complete structure: Getting Started (prerequisites, platform setup for macOS/Windows/Linux), Development Workflow (GitFlow with link to .specify/memory/gitflow-workflow.md, conventional commits with ShowMIDI scopes), Code Style (GPL-3.0 header template, JUCE patterns, EditorConfig/clang-format usage, DPI scaling with sm::scaled()), Testing (manual testing requirements, no leak warnings policy), PR Process (template usage, CI requirements, constitution compliance), Community (forum/Discord links)
- [X] T012 [P] [US2] Update `README.md` to add links to CONTRIBUTING.md, CHANGELOG.md, and STATISTICS.md in appropriate sections

**Validation**:
```bash
# Verify all required sections present
grep -q "# Contributing to ShowMIDI" CONTRIBUTING.md
grep -q "## Getting Started" CONTRIBUTING.md
grep -q "## Development Workflow" CONTRIBUTING.md
grep -q "## Code Style" CONTRIBUTING.md
grep -q "## Testing" CONTRIBUTING.md
grep -q "## Pull Request Process" CONTRIBUTING.md
grep -q "## Community" CONTRIBUTING.md

# Verify GitFlow link
grep -q ".specify/memory/gitflow-workflow.md" CONTRIBUTING.md

# Verify README links
grep -q "CONTRIBUTING.md" README.md
grep -q "CHANGELOG.md" README.md
```

**Dependencies**: 
- Requires T008 (CHANGELOG.md) to exist for README links
- Links to .editorconfig (T004) and .clang-format (T005) in content

**Acceptance Criteria Met**:
- ✓ Scenario 1: Clear instructions for environment setup, code style, workflow
- ✓ Scenario 2: PR checklist items documented (tests, headers, conventional commits)
- ✓ Scenario 3: Links to style guides, examples, automated tools (.editorconfig, clang-format)
- ✓ Scenario 4: GitFlow branching strategy with concrete examples

**Success Criteria**: SC-001 (new contributors can set up in < 30 minutes)

---

## Phase 5: User Story 3 - Consistent Code Formatting (P2)

**Story Goal**: Developers using different editors (VS Code, CLion, Vim, etc.) produce consistently formatted code without manual effort or style debates.

**Independent Test**: Have developers on different editors open the same file and verify their editor automatically applies consistent indentation, line endings, and formatting rules as defined in .editorconfig (acceptance scenario 1-4 from spec.md).

### Tasks

- [X] T013 [P] [US3] Add EditorConfig section to CONTRIBUTING.md explaining automatic formatting, editor support, and how to verify it's working
- [X] T014 [P] [US3] Test .editorconfig on different editors (VS Code, CLion, Vim) by opening existing C++ files and verifying no formatting changes occur
- [X] T015 [US3] Document EditorConfig in quickstart.md (if not already present) with installation instructions for different editors

**Validation**:
```bash
# Verify EditorConfig documentation
grep -q "EditorConfig" CONTRIBUTING.md

# Test EditorConfig with sample file
# (Manual test: open Source/PluginProcessor.cpp in different editors)
```

**Dependencies**: 
- Requires T004 (.editorconfig file created)
- Requires T011 (CONTRIBUTING.md exists)

**Acceptance Criteria Met**:
- ✓ Scenario 1: Editor applies 4-space indent, LF, UTF-8 automatically
- ✓ Scenario 2: Trailing whitespace removed, final newline added on save
- ✓ Scenario 3: No formatting-only diffs in git history
- ✓ Scenario 4: New developers get project formatting rules immediately

**Success Criteria**: SC-003 (zero formatting changes when opening files with EditorConfig-aware editor)

---

## Phase 6: User Story 4 - Automated C++ Style Enforcement (P2)

**Story Goal**: Developers can automatically format C++ code to JUCE standards, ensuring consistency in indentation, spacing, braces, and naming conventions without manual effort.

**Independent Test**: Run clang-format on existing source files and verify output matches JUCE coding standards (brace placement, spacing, etc.) without changing functionality (acceptance scenario 1-4 from spec.md).

### Tasks

- [X] T016 [P] [US4] Add clang-format section to CONTRIBUTING.md explaining installation (brew install clang-format on macOS, apt-get on Linux, LLVM download on Windows), usage (clang-format -i file.cpp), and IDE integration (VS Code C++ extension, CLion settings, Xcode plugin)
- [X] T017 [P] [US4] Run clang-format validation on existing Source/ directory to verify < 5% formatting changes (dry-run with --dry-run -Werror flags)
- [X] T018 [US4] Document clang-format in quickstart.md with examples of formatting single file, formatting all files, and checking without modifying

**Validation**:
```bash
# Verify clang-format documentation
grep -q "clang-format" CONTRIBUTING.md

# Validate formatting impact (< 5% of lines)
total_lines=$(find Source/ -name "*.cpp" -o -name "*.h" | xargs wc -l | tail -1 | awk '{print $1}')
changed_lines=$(find Source/ -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run -Werror 2>&1 | grep -c "warning:" || echo 0)
echo "Changed: $changed_lines / Total: $total_lines"
# Should be < 5%
```

**Dependencies**: 
- Requires T005 (.clang-format file created)
- Requires T011 (CONTRIBUTING.md exists)

**Acceptance Criteria Met**:
- ✓ Scenario 1: clang-format applies JUCE style (Allman braces, 4-space indent)
- ✓ Scenario 2: Formatting becomes consistent without altering logic
- ✓ Scenario 3: CI can validate formatting (documented for future enhancement)
- ✓ Scenario 4: IDE integration available (format on save)

**Success Criteria**: SC-004 (< 5% of lines changed when running clang-format)

---

## Phase 7: User Story 5 - Clean Repository Hygiene (P3)

**Story Goal**: Developers' local build artifacts, IDE settings, and OS-specific files are automatically excluded from version control, keeping the repository clean and professional.

**Independent Test**: Build the project on macOS, Windows, and Linux, then verify `git status` shows no untracked build artifacts, .DS_Store files, or IDE folders (acceptance scenario 1-4 from spec.md).

### Tasks

- [X] T019 [US5] Test .gitignore on all platforms (macOS, Windows, Linux) by building project and verifying git status shows clean (no build artifacts, IDE files, or OS-specific files)

**Validation**:
```bash
# Build and test gitignore
mkdir -p build && cd build && cmake .. && cmake --build .
cd ..

# Verify clean status
git status --short
# Should show no untracked files in build/, no .DS_Store, no IDE folders
```

**Dependencies**: 
- Requires T006 (.gitignore updated)

**Acceptance Criteria Met**:
- ✓ Scenario 1: .DS_Store and Xcode user data ignored on macOS
- ✓ Scenario 2: Visual Studio user files and build dirs ignored on Windows
- ✓ Scenario 3: CMake-generated files ignored
- ✓ Scenario 4: IDE folders (.vscode, .idea, .clangd) ignored

**Success Criteria**: SC-005 (zero untracked build artifacts after full builds on all platforms)

---

## Phase 8: User Story 6 - Structured Pull Request Submissions (P3)

**Story Goal**: Contributors submitting pull requests are guided by a template that ensures all necessary information is provided, including testing notes, constitution compliance, and linked issues.

**Independent Test**: Create a new PR on GitHub and verify the template auto-populates with checklist items for testing, version updates, and constitution compliance (acceptance scenario 1-4 from spec.md).

### Tasks

- [X] T020 [US6] Create `.github/pull_request_template.md` with structured sections: Description (with "Fixes #" placeholder), Type of Change (checkboxes for bug fix/feature/breaking/docs), Testing Checklist (macOS/Windows/Linux builds, manual testing, no leak warnings), Code Quality (GPL headers, conventional commits, JUCE patterns, DPI scaling), Related Issues, Constitution Compliance (multi-platform/performance/maintainability with link to constitution)

**Validation**:
```bash
# Verify PR template exists
test -f .github/pull_request_template.md

# Verify required sections
grep -q "## Description" .github/pull_request_template.md
grep -q "## Type of Change" .github/pull_request_template.md
grep -q "## Testing Checklist" .github/pull_request_template.md
grep -q "## Code Quality" .github/pull_request_template.md
grep -q "## Constitution Compliance" .github/pull_request_template.md

# Test on GitHub (manual: create test PR and verify template appears)
```

**Dependencies**: 
- Requires T011 (CONTRIBUTING.md exists for reference)
- Links to constitution (.specify/memory/constitution.md)

**Acceptance Criteria Met**:
- ✓ Scenario 1: Template guides description, testing checklist, related issues
- ✓ Scenario 2: Consistent format with all required information
- ✓ Scenario 3: Checklist allows reviewers to assess readiness
- ✓ Scenario 4: Template reminds about constitution compliance for architecture changes

**Success Criteria**: SC-006 (90% of PRs include completed checklist items), SC-007 (code review time decreases 25%)

---

## Phase 9: CI/CD Integration

**Goal**: Integrate existing CI/CD workflows and scripts into proper locations

**Independent Test**: Verify workflows execute from `.github/workflows/` and scripts run from `scripts/` directory

### Workflow Migration Tasks

- [X] T021 [P] Move `.github/prompts/workflows/ci.yml` to `.github/workflows/ci.yml`
- [X] T022 [P] Move `.github/prompts/workflows/test-build.yml` to `.github/workflows/test-build.yml`
- [X] T023 [P] Move `.github/prompts/workflows/realease.yml` to `.github/workflows/release.yml` (fix typo: realease→release)
- [X] T024 [P] Move `.github/prompts/workflows/release-with-readme-update.yml` to `.github/workflows/release-with-readme-update.yml`
- [X] T025 [P] Move `.github/prompts/workflows/download-stats.yml` to `.github/workflows/download-stats.yml`
- [X] T026 [P] Move `.github/prompts/workflows/update-readme-downloads.yml` to `.github/workflows/update-readme-downloads.yml`
- [X] T027 [P] Move `.github/prompts/workflows/update-file-sizes.yml` to `.github/workflows/update-file-sizes.yml`
- [X] T028 [P] Move `.github/prompts/workflows/changelog.yml` to `.github/workflows/changelog.yml`
- [X] T029 [P] Move `.github/prompts/workflows/README.md` to `.github/workflows/README.md`

### Script Migration Tasks

- [X] T030 [P] Move `.github/prompts/workflows/test-build-local.sh` to `scripts/test-build-local.sh` and ensure executable permissions (chmod +x)
- [X] T031 [P] Move `.github/prompts/workflows/update-download-links.sh` to `scripts/update-download-links.sh` and ensure executable permissions (chmod +x)
- [X] T032 Rename `scripts/relase-post-process.sh` to `scripts/release-post-process.sh` (fix typo: relase→release)

### Documentation & Statistics Tasks

- [X] T033 [P] Create `DOWNLOAD_STATS.md` at repository root with detailed statistics tables structure (By Release table with columns: Version/Date/Total/macOS/Windows/Linux/Other, By Platform table with columns: Platform/Downloads/Percentage, By Asset table with columns: File Name/Version/Platform/Size/Downloads)

**Validation**:
```bash
# Verify workflows in correct location
test -d .github/workflows
ls .github/workflows/*.yml | wc -l
# Should be 8 workflow files

# Verify scripts in correct location
test -f scripts/test-build-local.sh
test -f scripts/update-download-links.sh
test -f scripts/release-post-process.sh

# Verify executable permissions
test -x scripts/test-build-local.sh
test -x scripts/update-download-links.sh

# Verify old locations empty
! test -f .github/prompts/workflows/ci.yml
! test -f scripts/relase-post-process.sh

# Verify DOWNLOAD_STATS.md exists
test -f DOWNLOAD_STATS.md
```

**Dependencies**: 
- Requires T001 (.github/workflows/ directory created)
- Requires T002 (scripts/ directory verified)

**Parallel Execution**: Tasks T021-T031, T033 can all run in parallel (different files)

---

## Phase 10: Polish & Validation

**Goal**: Final verification, documentation updates, and cleanup

**Independent Test**: All files in correct locations, all documentation complete, repository passes all validation checks

### Tasks

- [X] T034 [P] Update `.github/copilot-instructions.md` "Recent Changes" section to document 002-dev-housekeeping completion (configuration files, documentation, CI/CD workflows)
- [X] T035 [P] Verify all workflow files reference correct script paths (scripts/*.sh instead of .github/prompts/workflows/*.sh)
- [X] T036 Run complete validation suite: EditorConfig check, clang-format dry-run, gitignore test, CHANGELOG format check, CONTRIBUTING.md section check, PR template format check, workflow YAML validation
- [X] T037 Remove `.github/prompts/workflows/` directory if empty after migration

**Validation**:
```bash
# Verify copilot-instructions updated
grep -q "002-dev-housekeeping" .github/copilot-instructions.md

# Verify workflow script paths
grep -r "scripts/" .github/workflows/*.yml
! grep -r ".github/prompts/workflows/" .github/workflows/*.yml

# Run validation checks
editorconfig-checker .editorconfig
clang-format --dry-run Source/*.cpp
git check-ignore .DS_Store build/
grep -q "## \[Unreleased\]" CHANGELOG.md
grep -q "# Contributing" CONTRIBUTING.md
grep -q "## Description" .github/pull_request_template.md

# Check empty directory
! test -d .github/prompts/workflows || test -z "$(ls -A .github/prompts/workflows)"
```

**Dependencies**: 
- Requires all previous phases complete
- T035 depends on T021-T032 (workflow/script migrations)
- T037 depends on T021-T032 (migrations complete)

---

## Dependencies & Execution Order

### Story Dependencies

```
Phase 1 (Setup) → Phase 2 (Foundational) → All other phases can proceed in parallel

Phase 3 (US1) ─┐
Phase 4 (US2) ─┤
Phase 5 (US3) ─┼→ Can run independently
Phase 6 (US4) ─┤
Phase 7 (US5) ─┤
Phase 8 (US6) ─┘

Phase 9 (CI/CD) → Can run after Phase 1 (directory setup)

Phase 10 (Polish) → Requires all previous phases complete
```

### Critical Path

```
T001 (create workflows dir) → T021-T029 (move workflows)
T002 (verify scripts dir) → T030-T032 (move/rename scripts)
T004 (create .editorconfig) → T013-T015 (US3 tasks)
T005 (create .clang-format) → T016-T018 (US4 tasks)
T006 (update .gitignore) → T019 (US5 task)
T008 (create CHANGELOG.md) → T009-T010 (populate versions) → T012 (update README)
T011 (create CONTRIBUTING.md) → T013, T016 (add sections)
```

### Parallel Execution Opportunities

**Phase 1**: All 3 tasks parallel  
**Phase 2**: Tasks T004, T005, T006 parallel; T007 sequential after them  
**Phase 3**: All 3 tasks parallel  
**Phase 4**: Both tasks parallel  
**Phase 5**: Tasks T013, T014 parallel; T015 can be parallel  
**Phase 6**: Tasks T016, T017 parallel; T018 can be parallel  
**Phase 9**: Tasks T021-T031, T033 all parallel (different files)  
**Phase 10**: Tasks T034, T035 parallel; T036, T037 sequential  

**Total Parallelizable**: 27 of 37 tasks (73%)

---

## Implementation Strategy

### MVP Scope (Minimum Viable Product)

**Phase 1-4** delivers P1 stories:
- CHANGELOG.md (version history transparency)
- CONTRIBUTING.md (contribution guidelines)
- Basic configuration files (.editorconfig, .clang-format, .gitignore)

This provides immediate value for contributors and establishes the foundation for the GitFlow workflow.

### Incremental Delivery

1. **Iteration 1** (P1): Phases 1-4 (Setup, Foundational, US1, US2)
2. **Iteration 2** (P2): Phases 5-6 (US3, US4 - formatting automation)
3. **Iteration 3** (P3): Phases 7-8 (US5, US6 - quality-of-life improvements)
4. **Iteration 4** (CI/CD): Phase 9 (workflow and script integration)
5. **Iteration 5** (Polish): Phase 10 (validation and cleanup)

### Testing Strategy

Each phase includes validation commands that can be run immediately after implementation to verify correctness. All user stories have independent test criteria defined in spec.md.

**Manual Tests Required**:
- EditorConfig: Open files in different editors (VS Code, CLion, Vim)
- clang-format: Run on Source/ directory and verify formatting
- .gitignore: Build on macOS, Windows, Linux and check git status
- PR template: Create test PR on GitHub and verify template appears
- Workflows: Trigger workflows manually and verify execution

**Automated Tests** (via validation commands):
- File existence checks
- Content validation (grep for required sections)
- Format validation (YAML, Markdown)
- Permission checks (executable scripts)

---

## Success Metrics

Upon completion, verify all success criteria from spec.md:

- [ ] **SC-001**: New contributors can set up environment in < 30 minutes (test with CONTRIBUTING.md)
- [ ] **SC-002**: 100% of releases include CHANGELOG.md updates (establish process)
- [ ] **SC-003**: EditorConfig-aware editors show zero formatting changes on existing files
- [ ] **SC-004**: clang-format produces < 5% line changes on existing Source/ directory
- [ ] **SC-005**: git status clean after builds on macOS, Windows, Linux
- [ ] **SC-006**: 90% of PRs include completed checklist items (measure after template deployed)
- [ ] **SC-007**: Code review time decreases 25% (measure over time)
- [ ] **SC-008**: Repository has "excellent" community health score on GitHub

---

## Notes

**File Typos**: Two typos identified and corrected:
- `realease.yml` → `release.yml` (T023)
- `relase-post-process.sh` → `release-post-process.sh` (T032)

**Directory Cleanup**: `.github/prompts/workflows/` should be removed after migration (T037)

**Executable Permissions**: Ensure moved scripts maintain +x permissions (T030, T031)

**Constitution Alignment**: All tasks support constitution principles (Maintainability, Development Workflow, Multi-Platform Architecture)

**No Tests Generated**: Per spec.md, no automated tests required for this feature (manual validation only)
