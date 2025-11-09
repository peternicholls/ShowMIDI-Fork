# Data Model: CI/CD Build Infrastructure Fix

**Date**: 2025-11-08  
**Feature**: 003-ci-build-fix

## Overview

This feature modifies CI/CD pipeline configuration and build system settings. While it doesn't involve traditional application data models (no databases, no user data), it does define structured entities within the build infrastructure domain.

---

## Entity: GitHubActionsWorkflow

Represents a GitHub Actions workflow YAML file that orchestrates automated build and validation processes.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| name | string | Yes | Workflow display name | Must be descriptive (e.g., "CI Build", "Test Build Matrix") |
| triggers | object | Yes | Event patterns that activate workflow | Must include at least one of: pull_request, push, workflow_dispatch |
| triggers.pull_request.branches | string[] | No | Target branches for PRs | Typically ["develop", "main"] |
| triggers.pull_request.paths-ignore | string[] | No | File patterns to ignore | Glob patterns (e.g., "**.md", "docs/**") |
| triggers.push.branches | string[] | No | Branches that trigger on push | Typically ["main", "release/**", "hotfix/**"] |
| concurrency.group | string | No | Concurrency group identifier | Should include `${{ github.ref }}` for branch isolation |
| concurrency.cancel-in-progress | boolean | No | Cancel outdated runs | Recommended: true for PR workflows |
| jobs | object | Yes | Build/test jobs to execute | At least one job required |

### Relationships
- **Contains** multiple Job entities (1-to-many)
- **Triggered by** GitHub events (push, pull_request, tag)
- **Produces** BuildArtifact entities

### State Transitions
1. **Queued**: Workflow triggered, waiting for runner
2. **In Progress**: Jobs executing on runners
3. **Cancelled**: Superseded by newer run (concurrency control)
4. **Success**: All jobs passed
5. **Failure**: One or more jobs failed
6. **Timeout**: Exceeded timeout-minutes limit

### Example (YAML)
```yaml
name: CI Build
on:
  pull_request:
    branches: [develop, main]
    paths-ignore: ['**.md', 'docs/**']
concurrency:
  group: ci-${{ github.ref }}
  cancel-in-progress: true
jobs:
  build-linux: { ... }
  build-macos: { ... }
  build-windows: { ... }
```

---

## Entity: BuildConfiguration

Represents CMake configuration settings that define how the ShowMIDI project is built.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| cmake_minimum_version | string | Yes | Minimum CMake version | Semver format (e.g., "3.15") |
| project_name | string | Yes | CMake project name | Must be "ShowMIDI" |
| project_version | string | Yes | Project version | Semver format, must match showmidi.jucer |
| juce_path | string | Yes | Path to JUCE framework | Resolved from PATH_TO_JUCE env var or submodule |
| build_clap | boolean | No | Enable CLAP plugin format | true if clap-juce-extensions found, false otherwise |
| build_lv2 | boolean | No | Enable LV2 plugin format | true on Linux, false on macOS/Windows |
| system_dependencies | string[] | Yes | Platform-specific system libs | ALSA, X11 on Linux; CoreAudio on macOS; etc. |
| compiler_flags | string[] | No | Additional compiler flags | ["-Wall", "-Wextra", "-Werror"] on Linux |

### Relationships
- **Depends on** ExternalDependency entities (JUCE, system libraries)
- **Produces** PluginFormat entities (Standalone, VST3, AU, etc.)
- **Used by** GitHubActionsWorkflow jobs

### Validation Rules
- `juce_path` MUST be checked in priority order: (1) PATH_TO_JUCE env var, (2) ./JUCE submodule
- If optional dependency (CLAP, LV2) missing: set corresponding build flag to false, emit warning
- `project_version` MUST match version in showmidi.jucer (synchronization requirement)

### Example (CMake)
```cmake
cmake_minimum_required(VERSION 3.15)
project(ShowMIDI VERSION 1.0.1)

# JUCE path resolution
if(DEFINED ENV{PATH_TO_JUCE})
    set(JUCE_DIR $ENV{PATH_TO_JUCE})
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
else()
    message(FATAL_ERROR "JUCE not found")
endif()

# Optional CLAP
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/clap-juce-extensions")
    set(BUILD_CLAP ON)
else()
    message(WARNING "CLAP skipped: clap-juce-extensions not found")
    set(BUILD_CLAP OFF)
endif()
```

---

## Entity: PluginFormat

Represents an output artifact type (executable or plugin bundle) produced by the build process.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| format_name | enum | Yes | Plugin format identifier | One of: Standalone, VST3, AU, LV2, CLAP |
| platforms | string[] | Yes | Supported platforms | macOS, Windows, Linux, iOS |
| is_optional | boolean | Yes | Can build proceed without this format | true for CLAP/LV2, false for Standalone/VST3/AU |
| file_extension | string | Yes | Output file extension | .app, .vst3, .component, .lv2, .clap |
| dependencies | string[] | No | Format-specific dependencies | CLAP → clap-juce-extensions, LV2 → LV2 headers |

### Enum Values: format_name
- `Standalone`: Executable application (all platforms)
- `VST3`: VST3 plugin (macOS, Windows, Linux)
- `AU`: Audio Unit plugin (macOS only)
- `LV2`: LV2 plugin (Linux primarily)
- `CLAP`: CLAP plugin (all platforms, optional)

### Relationships
- **Produced by** BuildConfiguration
- **Uploaded as** BuildArtifact (in CI workflows)
- **Requires** ExternalDependency entities

### Platform Availability Matrix

| Format | macOS | Windows | Linux | iOS |
|--------|-------|---------|-------|-----|
| Standalone | ✅ | ✅ | ✅ | ❌ |
| VST3 | ✅ | ✅ | ✅ | ❌ |
| AU | ✅ | ❌ | ❌ | ✅ (AUv3) |
| LV2 | ❌ | ❌ | ✅ | ❌ |
| CLAP | ✅ (opt) | ✅ (opt) | ✅ (opt) | ❌ |

---

## Entity: ExternalDependency

Represents a third-party library or system component required for building ShowMIDI.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| dependency_name | string | Yes | Dependency identifier | e.g., "JUCE", "ALSA", "clap-juce-extensions" |
| dependency_type | enum | Yes | Required or optional | "required" or "optional" |
| resolution_method | enum | Yes | How dependency is located | "submodule", "env_var", "system_package", "user_provided" |
| platforms | string[] | Yes | Applicable platforms | Subset of [macOS, Windows, Linux, iOS] |
| version_constraint | string | No | Version requirement | e.g., "7.0.5" for JUCE, ">=2.0" for CMake |
| fallback_strategy | string | No | Action when missing (optional deps only) | "skip_format", "warn", "fail" |

### Enum Values: dependency_type
- `required`: Build MUST fail if dependency missing (JUCE, CMake, system libraries)
- `optional`: Build continues without this dependency, affected features skipped (CLAP, LV2)

### Enum Values: resolution_method
- `submodule`: Git submodule in repository (JUCE)
- `env_var`: Environment variable path (PATH_TO_JUCE)
- `system_package`: OS package manager (ALSA on Linux via apt/yum)
- `user_provided`: User must manually provide (VST2 SDK)

### Relationships
- **Required by** BuildConfiguration
- **Affects** PluginFormat availability
- **Resolved during** workflow job setup

### Dependency Table

| Dependency | Type | Resolution | Platforms | Fallback |
|------------|------|------------|-----------|----------|
| JUCE 7.0.5 | required | env_var → submodule | all | FAIL |
| CMake 3.15+ | required | system_package | all | FAIL |
| ALSA | required | system_package | Linux | FAIL |
| X11 | required | system_package | Linux | FAIL |
| Freetype2 | required | system_package | Linux | FAIL |
| Xcode | required | runner_preinstalled | macOS | FAIL |
| Visual Studio 2022 | required | runner_preinstalled | Windows | FAIL |
| clap-juce-extensions | optional | submodule | all | skip_format + warn |
| LV2 headers | optional | system_package | Linux | skip_format + warn |
| VST2 SDK | optional | user_provided | all | skip_format (silent) |

---

## Entity: CIEnvironment

Represents a GitHub Actions runner environment with pre-installed tools and system configuration.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| runner_label | string | Yes | Runner identifier | "ubuntu-latest", "macos-latest", "windows-latest" |
| os_name | string | Yes | Operating system | "Ubuntu", "macOS", "Windows Server" |
| os_version | string | Yes | OS version | e.g., "22.04", "13", "2022" |
| available_tools | object | Yes | Pre-installed tools with versions | CMake, Xcode, MSVC, etc. |
| architecture | string[] | Yes | CPU architectures | ["x86_64"], ["x86_64", "arm64"] |
| provisioning_time | string | No | Typical startup time | "3-5s" for Linux, "30-60s" for macOS |

### Relationships
- **Executes** GitHubActionsWorkflow jobs
- **Provides** ExternalDependency entities (pre-installed tools)
- **Produces** BuildArtifact entities

### Current Runner Configurations

**ubuntu-latest (Ubuntu 22.04)**
```yaml
os_name: Ubuntu
os_version: 22.04
available_tools:
  cmake: "3.27.7"
  gcc: "11.4.0"
  clang: "14.0.0"
architecture: [x86_64]
provisioning_time: 3-5s
```

**macos-latest (macOS 13)**
```yaml
os_name: macOS
os_version: 13 (Ventura)
available_tools:
  cmake: "3.27.7"
  xcode: ["15.0", "15.1", "15.4"]  # Available versions vary
  clang: "Xcode bundled"
architecture: [x86_64, arm64]
provisioning_time: 30-60s
```

**windows-latest (Windows Server 2022)**
```yaml
os_name: Windows Server
os_version: 2022
available_tools:
  cmake: "3.27.7"
  visual_studio: "2022 (17.x)"
  msvc: "19.37"
architecture: [x86_64]
provisioning_time: 15-30s
```

### Adaptive Behavior
- **Xcode Version Selection**: Workflow queries available Xcode versions at runtime using `ls /Applications/ | grep Xcode`, then selects latest
- **Tool Version Tolerance**: Build system specifies minimum versions (CMake 3.15+) rather than exact versions
- **Runner Image Updates**: GitHub may change pre-installed tool versions; workflows must adapt

---

## Entity: BuildArtifact

Represents a compiled output file uploaded from CI workflow for manual testing or release.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| artifact_name | string | Yes | GitHub Actions artifact name | e.g., "ShowMIDI-macOS-Standalone" |
| plugin_format | string | Yes | Format type | References PluginFormat.format_name |
| platform | string | Yes | Build platform | "macOS", "Windows", "Linux" |
| build_type | enum | Yes | Debug or Release | "Debug" or "Release" |
| file_path | string | Yes | Relative path in artifact | e.g., "Standalone/ShowMIDI.app" |
| retention_days | integer | No | How long to keep artifact | Default 90, range 1-400 |

### Relationships
- **Produced by** GitHubActionsWorkflow job
- **Contains** PluginFormat output
- **Stored in** GitHub Actions artifact storage

### Example Artifact Structure
```
ShowMIDI-macOS-Standalone/
├── ShowMIDI.app/
│   └── Contents/
│       ├── MacOS/ShowMIDI
│       └── Info.plist

ShowMIDI-macOS-VST3/
├── ShowMIDI.vst3/
│   └── Contents/...

ShowMIDI-Windows-Standalone/
├── ShowMIDI.exe
```

---

## Entity: WorkflowTrigger

Represents an event pattern that activates a GitHub Actions workflow.

### Attributes

| Attribute | Type | Required | Description | Validation Rules |
|-----------|------|----------|-------------|------------------|
| event_type | enum | Yes | GitHub event type | "pull_request", "push", "workflow_dispatch", "tag" |
| branch_pattern | string | No | Branch name pattern | Glob pattern (e.g., "release/**") |
| path_filter | object | No | Include/exclude file patterns | {include: [...], exclude: [...]} |
| tag_pattern | string | No | Tag pattern (for tag events) | Semver pattern (e.g., "v*.*.*") |

### Enum Values: event_type
- `pull_request`: Triggered when PR opened/updated
- `push`: Triggered on commit push to branch
- `workflow_dispatch`: Manual workflow trigger
- `tag`: Triggered when tag is pushed

### Path Filter Rules
- `exclude` (paths-ignore): Skip workflow if only these paths changed
- Include implicitly all paths not in exclude list
- Documentation pattern: `['**.md', 'docs/**', '*.txt']`

### Trigger Matrix

| Workflow | Event | Branches | Path Filters |
|----------|-------|----------|--------------|
| CI Build | pull_request | develop, main | Exclude: **.md, docs/** |
| CI Build | push | main, release/**, hotfix/** | Exclude: **.md, docs/** |
| Test Build | push | develop | None |
| Changelog | tag | N/A (all) | Pattern: v*.*.* |

---

## Summary

This data model defines 7 key entities in the CI/CD build infrastructure:

1. **GitHubActionsWorkflow**: Orchestrates automated builds
2. **BuildConfiguration**: CMake settings and dependency resolution
3. **PluginFormat**: Output artifact types (Standalone, VST3, AU, LV2, CLAP)
4. **ExternalDependency**: Third-party libraries and system components
5. **CIEnvironment**: GitHub Actions runner configurations
6. **BuildArtifact**: Compiled outputs uploaded for testing
7. **WorkflowTrigger**: Event patterns that activate workflows

All entities support the feature requirements: adaptive Xcode selection (CIEnvironment), graceful degradation (ExternalDependency), path filtering (WorkflowTrigger), and timeout handling (GitHubActionsWorkflow).
