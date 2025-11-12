```markdown
# Implementation Plan: Phase 11 — Testing Cadence & Governance (Run 2)

**Feature Branch**: `005-test-cadence`  
**Plan Version**: 2  
**Status**: Draft  

## 1. Technical Context

| Category | Detail | Status |
|---|---|---|
| **CI/CD Platform** | GitHub Actions | `NEEDS CLARIFICATION` - Confirm runner specs (OS versions, hardware) and self-hosted vs. GitHub-hosted strategy. |
| **Build System** | CMake 3.15+ | `CONFIRMED` |
| **Test Frameworks** | JUCE UnitTest, CTest | `CONFIRMED` |
| **Static Analysis** | clang-tidy, cppcheck (TBD) | `NEEDS CLARIFICATION` - Confirm specific tools, rule sets (e.g., C++ Core Guidelines), and integration method. |
| **Code Formatting** | .clang-format | `CONFIRMED` |
| **Security Scanning** | SAST/SCA (TBD) | `NEEDS CLARIFICATION` - Select tools (e.g., CodeQL, Snyk, Dependabot) and define High/Critical severity criteria. |
| **License Compliance** | TBD | `NEEDS CLARIFICATION` - Select tool (e.g., FOSSA, Snyk) and define policy for compliant/non-compliant licenses. |
| **Packaging** | CMake/CPack, platform-specific tools (productbuild for .pkg, Inno Setup for .iss) | `CONFIRMED` |
| **Scripting** | Bash, PowerShell, Python (for automation) | `NEEDS CLARIFICATION` - Confirm Python version and dependency management for scripts. |

## 2. Constitution Check

| Principle | Check | Result | Notes |
|---|---|---|---|
| **1. Multi-Platform** | Does the plan validate across macOS, Windows, and Linux? | ✅ PASS | FR-007 and FR-011 define a balanced platform coverage strategy for PR, nightly, and release builds. |
| **2. JUCE Standards** | Does the plan align with JUCE testing idioms? | ✅ PASS | Leverages `JUCE UnitTest` and standard CTest integration. |
| **3. Real-Time Perf.** | Does the plan include performance validation? | ✅ PASS | FR-001 and FR-002 explicitly include performance tests in the nightly cadence. |
| **4. User-Centric** | Does the plan improve quality and stability for the user? | ✅ PASS | The entire premise is to build a robust quality gate, directly upholding user-centric values. |
| **5. Maintainability** | Does the plan enforce code quality and clarity? | ✅ PASS | FR-001 and FR-006 mandate static analysis, formatting, and linting, which are key to maintainability. |

## 3. Implementation Phases

### Phase 0: Research & Clarification

*Objective: Resolve all `NEEDS CLARIFICATION` items from the Technical Context table.*

1.  **Task**: Research and define GitHub Actions runner specifications.
    *   **Goal**: Determine OS versions (macOS, Windows, Linux), hardware profiles, and cost/benefit of GitHub-hosted vs. self-hosted runners.
    *   **Output**: Updated "CI/CD Platform" entry in Technical Context.
2.  **Task**: Select and configure static analysis tools.
    *   **Goal**: Evaluate `clang-tidy` and `cppcheck`. Define a baseline rule set (e.g., C++ Core Guidelines, performance, security) and integrate into the CMake build.
    *   **Output**: Updated "Static Analysis" entry; `research.md` with tool comparison.
3.  **Task**: Select SAST/SCA and License Compliance tools.
    *   **Goal**: Evaluate GitHub-native tools (CodeQL, Dependabot) vs. third-party (Snyk). Define what constitutes a "High/Critical" severity finding.
    *   **Output**: Updated "Security Scanning" and "License Compliance" entries; `research.md` with rationale.
4.  **Task**: Define scripting environment standards.
    *   **Goal**: Specify Python version (e.g., 3.9+) and a dependency management strategy (e.g., `requirements.txt`) for automation scripts.
    *   **Output**: Updated "Scripting" entry.

**Deliverable**: `research.md` containing decisions, rationale, and alternatives for all clarified items.

### Phase 1: Core CI Workflow & Local Scripts

*Objective: Implement foundational CI jobs and local scripts for fast feedback.*

1.  **Data Model**: Define `CheckCategory` and `Trigger` entities in `data-model.md`.
2.  **Contracts**: N/A for this feature.
3.  **Implementation**:
    *   **Local Scripts (Pre-commit/Pre-push)**:
        *   Create a `pre-commit-hook.sh` script to run `.clang-format` and a linter.
        *   Create a `pre-push-hook.sh` script to execute the "fast" unit test suite (`ctest -L fast`).
        *   Document setup in `CONTRIBUTING.md`.
    *   **GitHub Actions Workflow (`.github/workflows/ci.yml`)**:
        *   Create a matrix build job for macOS, Windows, and Linux.
        *   Implement a "Build & Unit Test" step that runs on every PR.
        *   Add a "Static Analysis" step that runs the selected tool.
        *   Add a "Formatting Check" step that fails if `clang-format` finds diffs.
4.  **Agent Context**: Run `.specify/scripts/bash/update-agent-context.sh copilot` to add `GitHub Actions`, `clang-tidy` to the agent's knowledge base.

**Deliverables**: `data-model.md`, updated `ci.yml`, local hook scripts, updated `CONTRIBUTING.md`.

### Phase 2: Extended & Scheduled Workflows

*Objective: Implement deeper, scheduled checks for comprehensive coverage.*

1.  **Implementation**:
    *   **Nightly Workflow**:
        *   Create a new workflow file (`.github/workflows/nightly.yml`) that runs on a `schedule: cron`.
        *   Add jobs for:
            *   **System & Integration Tests**: Run the full test suite (`ctest`).
            *   **Performance Tests**: Execute performance benchmarks and report against a baseline.
            *   **Security Scans**: Run SAST/SCA tools and report findings.
    *   **Release Workflow**:
        *   Modify the `ci.yml` to trigger on `release/*` branches.
        *   Add steps for:
            *   **Packaging**: Build installers (.pkg, .exe) and archives.
            *   **Installer Smoke Tests**: Run a basic install/uninstall test on each platform.
2.  **Constitutional Amendments**: Draft the proposed changes to `constitution.md` based on FR-014.

**Deliverables**: `nightly.yml`, updated `ci.yml`, draft `constitution-amendments.md`.
```