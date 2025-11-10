# Feature Specification: CI/CD Build Infrastructure Fix

**Feature Branch**: `003-ci-build-fix`  
**Created**: 2025-11-08  
**Status**: Draft  
**Input**: User description: "fix all the build infrastructure issues, and get our automated workflow running, working and polished"

## Clarifications

### Session 2025-11-08

- Q: When optional dependencies (CLAP extensions, LV2 libraries) are missing during a build, what should the build system do? → A: Skip building that plugin format and emit a warning, continue with other formats (partial build)
- Q: When GitHub Actions runner images update and remove a previously available Xcode version (e.g., 15.2 is removed), how should the workflow handle this? → A: Automatically select the latest available Xcode version on the runner with explicit verification (adaptive selection)
- Q: What is the acceptable CI workflow failure rate for successful PRs (those without actual code bugs)? → A: <5% failure rate
- Q: How should the build system handle finding JUCE framework when both PATH_TO_JUCE environment variable and git submodule are present? → A: Prioritize PATH_TO_JUCE environment variable first, fall back to git submodule (explicit override)
- Q: When a workflow exceeds GitHub Actions timeout limits, what recovery strategy should be implemented? → A: Automatically retry once, then fail with notification if second attempt times out (single auto-retry)

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Successful CMake Builds Across All Platforms (Priority: P1)

Contributors need all GitHub Actions workflows to complete successfully when code is pushed or PRs are created, ensuring build infrastructure doesn't block development workflow.

**Why this priority**: Without working CI, developers cannot merge PRs and the development workflow is blocked. This is the most critical issue preventing normal development.

**Independent Test**: Can be fully tested by pushing a trivial change (e.g., comment update) to a feature branch, creating a PR, and verifying all CI jobs complete with green status. Delivers immediate value by unblocking development workflow.

**Acceptance Scenarios**:

1. **Given** a feature branch with code changes, **When** a PR is opened to develop, **Then** the CI workflow runs successfully and all build jobs pass (Linux CMake, macOS Xcode, Windows Visual Studio)
2. **Given** CMake configuration on Linux, **When** the build system configures the project, **Then** all dependencies (JUCE, CLAP extensions, system libraries) are found and configured correctly
3. **Given** a push to develop branch, **When** the test-build workflow runs, **Then** all platform builds complete successfully and produce valid artifacts

---

### User Story 2 - Correct Xcode Environment Configuration (Priority: P1)

macOS builds in CI need to use the correct Xcode version that exists on GitHub Actions runners, preventing invalid developer directory errors.

**Why this priority**: macOS builds are currently failing due to hardcoded invalid Xcode path, preventing validation of macOS-specific code and plugin formats (AU, VST3, Standalone).

**Independent Test**: Can be fully tested by triggering the CI workflow and verifying the macOS build job completes successfully with the correct Xcode version detected and used. Delivers value by enabling macOS build validation.

**Acceptance Scenarios**:

1. **Given** a GitHub Actions macOS runner, **When** the CI workflow selects Xcode, **Then** it uses an Xcode version that exists on the runner (e.g., Xcode 15.0, 15.1, 15.4, not hardcoded 15.2)
2. **Given** the macOS build job starts, **When** xcode-select runs, **Then** no "invalid developer directory" error occurs
3. **Given** successful Xcode selection, **When** building Standalone, VST3, AU, and LV2 plugin formats, **Then** all builds complete without Xcode path errors

---

### User Story 3 - Optimized Workflow Triggers and Efficiency (Priority: P2)

Developers need workflows to run only when necessary (not on documentation-only changes) and complete in reasonable time, avoiding wasted CI minutes and long wait times.

**Why this priority**: Efficient CI reduces costs, provides faster feedback, and prevents developer frustration from waiting for unnecessary builds.

**Independent Test**: Can be fully tested by making a documentation-only change (update README.md), creating PR, and verifying heavy build workflows are skipped. Delivers value by saving CI minutes and developer time.

**Acceptance Scenarios**:

1. **Given** a PR with only Markdown or documentation file changes, **When** the PR is created, **Then** heavy build workflows (macOS/Windows/Linux builds) are skipped
2. **Given** a PR with C++ source code changes, **When** the PR is created, **Then** all relevant build workflows run
3. **Given** a push to develop branch (not a tagged release), **When** the changelog workflow evaluates its trigger, **Then** it skips execution (only runs on version tags)
4. **Given** multiple commits pushed rapidly to a PR, **When** new commits arrive, **Then** in-progress workflow runs are cancelled automatically (concurrency control)

---

### User Story 4 - CLAP Plugin Build Support (Priority: P3)

Developers who want to build the CLAP plugin format need CMake configuration that correctly finds and integrates the clap-juce-extensions library.

**Why this priority**: CLAP is an optional plugin format (VST3, AU, Standalone are primary). Fixing CLAP support adds capability but doesn't block core functionality.

**Independent Test**: Can be fully tested by running CMake configuration with CLAP build enabled (`cmake -Bbuild_clap -DCMAKE_BUILD_TYPE=Release`) and verifying it completes without errors. Delivers value by enabling CLAP plugin builds for developers who need them.

**Acceptance Scenarios**:

1. **Given** the project is configured for CLAP build, **When** CMake configures the project, **Then** it finds the clap-juce-extensions directory at `libs/clap-juce-extensions`
2. **Given** CLAP CMake configuration succeeds, **When** building the CLAP target, **Then** the build completes and produces `ShowMIDI.clap` artifact
3. **Given** the CLAP build instructions in CMakeLists.txt, **When** a developer follows them, **Then** they can successfully build the CLAP plugin without errors

---

### User Story 5 - Workflow Documentation and Maintenance (Priority: P3)

Contributors need clear documentation about workflow behavior, triggers, and troubleshooting steps so they understand CI failures and know how to fix them.

**Why this priority**: Documentation improves maintainability and reduces confusion but doesn't directly fix broken builds.

**Independent Test**: Can be fully tested by reading the workflow documentation and successfully troubleshooting a simulated CI failure using the provided guidance. Delivers value by reducing support burden and enabling self-service troubleshooting.

**Acceptance Scenarios**:

1. **Given** a CI workflow failure, **When** a contributor checks the workflow documentation, **Then** they find clear explanation of what the workflow does, when it runs, and common failure scenarios
2. **Given** the CONTRIBUTING.md guide, **When** a contributor reads the CI/CD section, **Then** they understand workflow triggers, how to test locally, and how to interpret CI results
3. **Given** a workflow configuration file, **When** reviewing the YAML, **Then** comments explain non-obvious configuration choices (Xcode version selection, path filters, concurrency settings)

---

### Edge Cases

- What happens when GitHub Actions runner images update Xcode versions (e.g., 15.2 removed, 16.0 added)? → Automatically select latest available Xcode with verification
- How does the system handle CMake finding JUCE at different paths (environment variable vs. relative path)? → Prioritize PATH_TO_JUCE, fall back to submodule
- What happens when CLAP extensions submodule is not initialized? → Skip CLAP format, emit warning, continue with other formats
- How does the system handle builds when optional dependencies (CLAP, LV2) are missing? → Skip affected format, emit warning, continue with other formats
- What happens when workflows are triggered on stale branches that diverged from develop?
- How does the system handle concurrent PR builds from the same branch?
- What happens when a workflow exceeds GitHub Actions timeout limits? → Automatically retry once, then fail with notification if retry times out

## Requirements *(mandatory)*

### Functional Requirements

#### Build System Configuration

- **FR-001**: CMake MUST successfully configure the project on Linux runners with all required dependencies (JUCE, ALSA, X11, Freetype) found and linked
- **FR-002**: CMake MUST locate JUCE framework by checking PATH_TO_JUCE environment variable first, then falling back to relative path to JUCE submodule if not set
- **FR-003**: CMake MUST locate clap-juce-extensions at `libs/clap-juce-extensions` when CLAP build is requested
- **FR-004**: CMake MUST provide clear error messages when required dependencies are missing, indicating which package to install
- **FR-005**: Build system MUST support all plugin formats: Standalone, VST3, AU (macOS), LV2 (Linux), CLAP (optional)
- **FR-005a**: When optional dependencies (CLAP, LV2) are missing, CMake MUST skip building that plugin format, emit a clear warning message, and continue building available formats

#### Xcode Configuration

- **FR-006**: macOS CI workflows MUST use an Xcode version that exists on the GitHub Actions runner (not hardcoded 15.2)
- **FR-007**: Xcode selection MUST verify the path exists before attempting to use it
- **FR-007a**: When a specific Xcode version is unavailable, workflow MUST automatically select the latest available Xcode version on the runner
- **FR-008**: macOS builds MUST successfully compile Standalone, VST3, and AU plugin formats
- **FR-009**: macOS builds MUST disable code signing in CI environment (already configured)
- **FR-010**: Xcode configuration MUST work with both Intel and Apple Silicon architectures

#### Workflow Triggers and Efficiency

- **FR-011**: CI workflow MUST run on pull requests to develop and main branches
- **FR-012**: CI workflow MUST run on pushes to main, release/*, and hotfix/* branches
- **FR-013**: Heavy build workflows (macOS/Windows/Linux) MUST be skipped when PR contains only documentation files (*.md, docs/*, *.txt in repo root)
- **FR-014**: Changelog workflow MUST run only on version tags (v*.*.*) and manual dispatch (not on regular pushes to develop)
- **FR-015**: Workflows MUST implement concurrency control to cancel in-progress runs when new commits are pushed to the same branch

#### Build Quality and Validation

- **FR-016**: Linux CMake builds MUST compile with warnings treated as errors (-Wall -Wextra -Werror)
- **FR-017**: Code quality checks MUST validate GPL-3.0 headers in all Source/*.cpp and Source/*.h files
- **FR-018**: Build artifacts MUST be uploaded for manual testing (apps, plugins)
- **FR-019**: Test build workflow MUST generate a summary table showing pass/fail status for each platform
- **FR-020**: All workflows MUST complete within GitHub Actions timeout limits (typically 360 minutes per job, 6 hours per workflow)
- **FR-020a**: CI infrastructure MUST maintain <5% spurious failure rate (failures not caused by code bugs) to ensure developer trust and workflow reliability
- **FR-020b**: When a workflow job times out, the workflow MUST automatically retry once; if the retry also times out, fail with clear notification to developer

#### Environment Configuration

- **FR-021**: Workflows MUST check out git submodules recursively (for JUCE framework)
- **FR-022**: Linux workflows MUST install all required system dependencies before building
- **FR-023**: Windows workflows MUST configure MSVC environment correctly
- **FR-024**: All workflows MUST use consistent build types (Debug or Release) across steps
- **FR-025**: Environment variables MUST be clearly documented (PATH_TO_JUCE, RELEASE_VERSION)

### Key Entities *(include if feature involves data)*

- **GitHub Actions Workflow**: Represents automated build/test process triggered by events (PR, push, tag), contains jobs and steps
- **Build Configuration**: CMake settings, compiler flags, dependency paths that define how the project is built
- **Plugin Format**: Output artifact type (Standalone, VST3, AU, LV2, CLAP) with specific build requirements
- **CI Environment**: GitHub Actions runner with OS, tools (CMake, Xcode, MSVC), and dependencies
- **Workflow Trigger**: Event pattern (branch name, file path, tag format) that activates a workflow
- **Build Artifact**: Compiled output (executable, plugin bundle) uploaded for testing or release

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 100% of CI workflow runs succeed when triggered by PRs containing only code fixes or new features (no build system changes)
- **SC-002**: CMake configuration on Linux completes in under 2 minutes without missing dependency errors
- **SC-003**: macOS Xcode builds complete successfully for all plugin formats (Standalone, VST3, AU) in under 15 minutes
- **SC-004**: Windows Visual Studio builds complete successfully for all plugin formats in under 20 minutes
- **SC-005**: Documentation-only PRs skip heavy build workflows, completing in under 30 seconds
- **SC-006**: Workflow runs triggered by new commits to same branch cancel previous in-progress runs within 10 seconds
- **SC-007**: CLAP plugin builds complete successfully when following documented CMake instructions
- **SC-008**: Zero "invalid developer directory" errors occur in macOS CI jobs over 10 consecutive runs
- **SC-009**: Test build workflow summary table displays within 5 seconds of all platform builds completing
- **SC-010**: 90% of workflow failures provide actionable error messages that clearly indicate the problem (missing dependency, compilation error, test failure)
- **SC-011**: Infrastructure-related failures (runner unavailability, network issues, timeout) occur in <5% of workflow runs over a 30-day period

## Assumptions *(optional)*

- GitHub Actions runner images are maintained by GitHub and may update Xcode/tool versions without notice
- The project uses JUCE 7.0.5 framework as a git submodule
- clap-juce-extensions library is present in `libs/clap-juce-extensions` directory
- Contributors have basic understanding of CMake and CI/CD concepts
- Workflow runs have access to GitHub Actions secrets for any required credentials
- Primary plugin formats are Standalone, VST3, and AU; LV2 and CLAP are secondary
- Linux builds use Ubuntu latest runner (currently 22.04)
- macOS builds use macos-latest runner (currently macOS 13)
- Windows builds use windows-latest runner (currently Windows Server 2022)
- Build warnings as errors is desired for code quality but may be relaxed for third-party dependencies

## Dependencies *(optional)*

- **JUCE Framework**: Audio plugin framework (version 7.0.5) - required for all builds
- **clap-juce-extensions**: Library for CLAP plugin format support - required only for CLAP builds
- **GitHub Actions Runner Images**: Maintained by GitHub, determines available tools and versions
- **System Libraries**: Platform-specific (ALSA, X11 on Linux; CoreAudio, CoreMIDI on macOS; WASAPI on Windows)
- **CMake**: Build system generator (minimum version 3.15)
- **Xcode**: macOS/iOS development tools (version determined by runner image)
- **Visual Studio 2022**: Windows development tools
- **Git Submodules**: JUCE framework must be initialized and updated before builds

## Out of Scope *(optional)*

- Implementing new plugin formats or audio processing features
- Changing the core build system from CMake to alternative (Bazel, Meson, etc.)
- Adding automated testing of plugin functionality (VST3 validator, AU validation)
- Implementing automated performance benchmarking
- Adding code coverage reporting
- Implementing automated release artifact signing and notarization (macOS)
- Creating Docker-based build environments for reproducible local builds
- Adding support for older OS versions (e.g., macOS 10.15, Windows 7)
- Implementing distributed build caching (ccache, sccache)
- Adding automated dependency vulnerability scanning

## Technical Constraints *(optional)*

- GitHub Actions free tier has limited concurrent job slots (20 for free accounts, 60 for Pro)
- GitHub Actions runners have fixed tool versions that may change without notice
- macOS runners are significantly slower to provision than Linux runners (30-60 seconds vs 3-5 seconds)
- CMake must support C++17 standard (required by JUCE and ShowMIDI)
- Xcode command-line builds require valid developer directory path
- JUCE framework requires specific system libraries on each platform
- VST3 and AU plugin formats have platform-specific build requirements
- Git submodules must be initialized before builds (recursive checkout required)
- Workflow YAML syntax is constrained by GitHub Actions schema
- Build artifacts uploaded to GitHub Actions expire after configurable retention period (default 90 days, can be set to 1-400 days)
