# Data Model: Development Housekeeping

**Phase**: 1 (Design & Contracts)
**Date**: 2025-11-08
**Feature**: Documentation and configuration file structures

## Overview

This feature involves structured text files rather than application data. The "data model" describes the format and schema of documentation and configuration files.

---

## Entity: CHANGELOG.md

**Purpose**: Version history document tracking all user-visible changes across releases

**Format**: Keep a Changelog (Markdown)

### Structure

```yaml
Document:
  - Preamble:
      - Title: "Changelog"
      - Description: "All notable changes to this project"
      - Standard Link: "[Keep a Changelog](https://keepachangelog.com)"
      - Versioning Link: "[Semantic Versioning](https://semver.org)"
  
  - Unreleased Section:
      - Title: "[Unreleased]"
      - Categories:
          - Added: []        # New features
          - Changed: []      # Changes to existing
          - Deprecated: []   # Soon-to-be-removed
          - Removed: []      # Removed features
          - Fixed: []        # Bug fixes
          - Security: []     # Security fixes
  
  - Version Sections:
      - Version: "1.0.1"   # Semantic version
        Date: "2025-11-01" # ISO 8601 date
        Categories:
          - Added: [...]
          - Fixed: [...]
      
      - Version: "1.0.0"
        Date: "2025-10-15"
        Categories:
          - Added: [...]
```

### Validation Rules

- Version numbers MUST follow Semantic Versioning 2.0.0 (MAJOR.MINOR.PATCH)
- Versions MUST be listed in reverse chronological order (newest first)
- Dates MUST be in ISO 8601 format (YYYY-MM-DD)
- Categories MUST be one of: Added, Changed, Deprecated, Removed, Fixed, Security
- Each change item SHOULD link to issue/PR when applicable
- Each change item MUST be a single line starting with `-`

### Example Entry

```markdown
## [1.0.1] - 2025-11-01
### Fixed
- Resolved crash when disconnecting MIDI device during playback (#42)
- Corrected DPI scaling on 4K displays (#45)

### Changed
- Improved theme loading performance by 30%
```

---

## Entity: CONTRIBUTING.md

**Purpose**: Contributor guide providing setup instructions, workflow guidance, and code standards

**Format**: Markdown with sections

### Structure

```yaml
Document:
  - Title: "Contributing to ShowMIDI"
  
  - Table of Contents:
      - Getting Started
      - Development Workflow
      - Code Style
      - Testing
      - Pull Request Process
      - Community
  
  - Getting Started:
      - Prerequisites:
          - C++17 compiler
          - CMake 3.15+ or Xcode or Visual Studio 2022
          - JUCE (via git submodule)
      - Platform-Specific Setup:
          - macOS: [instructions]
          - Windows: [instructions]
          - Linux: [instructions]
      - Building: [CMake and Projucer instructions]
  
  - Development Workflow:
      - GitFlow Branching:
          - Link: ".specify/memory/gitflow-workflow.md"
          - Feature branch creation
          - Release process
      - Conventional Commits:
          - Format: "type(scope): subject"
          - ShowMIDI scopes: [midi, ui, theme, plugin, etc.]
      - Helper Scripts:
          - create-feature.sh
          - create-release.sh
          - create-hotfix.sh
  
  - Code Style:
      - GPL-3.0 Headers: [template]
      - JUCE Patterns: [examples]
      - DPI Scaling: sm::scaled() usage
      - EditorConfig: automatic formatting
      - clang-format: C++ style enforcement
  
  - Testing:
      - Manual Testing Requirements
      - No JUCE Leak Warnings Policy
      - Multi-platform Validation
  
  - Pull Request Process:
      - PR Template Usage
      - CI Validation Requirements
      - Constitution Compliance for Architecture Changes
  
  - Community:
      - Forum Link
      - Discord Link
      - Issue Reporting
```

### Validation Rules

- All external links MUST be valid and accessible
- Code examples MUST be syntax-highlighted with language tags
- GitFlow workflow link MUST point to `.specify/memory/gitflow-workflow.md`
- Constitution link MUST point to `.specify/memory/constitution.md`
- Scripts referenced MUST exist in `.specify/scripts/bash/`

---

## Entity: .editorconfig

**Purpose**: Editor configuration defining cross-platform formatting rules

**Format**: INI-style configuration

### Structure

```ini
root = true

[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
trim_trailing_whitespace = true

[*.{cpp,h,mm}]
indent_style = space
indent_size = 4

[*.{md,txt}]
indent_style = space
indent_size = 2
trim_trailing_whitespace = false

[*.{json,yml,yaml}]
indent_style = space
indent_size = 2

[Makefile]
indent_style = tab

[*.{jucer,xml}]
indent_style = space
indent_size = 2
```

### Validation Rules

- `root = true` MUST be first line
- Section headers MUST use glob patterns in square brackets
- Values MUST be lowercase (utf-8, lf, space, tab)
- indent_size MUST be numeric
- Boolean values: true/false (lowercase)

---

## Entity: .clang-format

**Purpose**: C++ code formatter configuration enforcing JUCE style

**Format**: YAML

### Structure

```yaml
BasedOnStyle: LLVM
Language: Cpp

# Brace wrapping (Allman style - JUCE standard)
BreakBeforeBraces: Allman

# Indentation
IndentWidth: 4
TabWidth: 4
UseTab: Never
NamespaceIndentation: None

# Line length
ColumnLimit: 120

# Pointers and references
PointerAlignment: Left
ReferenceAlignment: Left

# Spacing
SpaceAfterCStyleCast: false
SpaceBeforeParens: ControlStatements
SpacesInParentheses: false
SpacesInSquareBrackets: false

# Alignment
AlignConsecutiveAssignments: false
AlignConsecutiveMacros: true
AlignConsecutiveDeclarations: false

# Breaking
AllowShortFunctionsOnASingleLine: None
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false

# Comments
AlignTrailingComments: true
ReflowComments: true

# Includes
SortIncludes: true
IncludeBlocks: Preserve
```

### Validation Rules

- BasedOnStyle MUST be valid clang-format base (LLVM, Google, Mozilla, etc.)
- BreakBeforeBraces MUST match JUCE Allman style
- IndentWidth MUST be 4 (matching JUCE convention)
- ColumnLimit SHOULD be 100-140 (readability vs line length)
- Boolean values: true/false (lowercase)

---

## Entity: .github/pull_request_template.md

**Purpose**: GitHub PR template providing structured format for submissions

**Format**: Markdown with checkboxes

### Structure

```yaml
Document:
  - Description Section:
      - What does this PR do?
      - Why is this change needed?
      - Link to spec (optional)
  
  - Type of Change:
      - [ ] Bug fix
      - [ ] New feature
      - [ ] Breaking change
      - [ ] Documentation
  
  - Testing Checklist:
      - [ ] Built on macOS
      - [ ] Built on Windows
      - [ ] Built on Linux
      - [ ] Manual testing
      - [ ] No leak warnings
  
  - Code Quality:
      - [ ] GPL-3.0 headers
      - [ ] Conventional commits
      - [ ] JUCE patterns
      - [ ] DPI scaling (if UI)
  
  - Related Issues:
      - Fixes #
      - Related to #
  
  - Constitution Compliance:
      - [ ] Multi-platform
      - [ ] Performance
      - [ ] Maintainability
      - Link: (if applicable)
```

### Validation Rules

- Checkboxes MUST use `- [ ]` format (GitHub markdown)
- Issue references MUST use `#<number>` format for auto-linking
- Constitution link SHOULD point to specific section anchor
- All sections MUST be present (can be marked N/A if not applicable)

---

## Entity: STATISTICS.md & DOWNLOAD_STATS.md

**Purpose**: Download statistics dashboard and detailed data tables

**Format**: Markdown with tables and auto-updated metrics

###STATISTICS.md Structure

```yaml
Document:
  - Header:
      - Badges: [Total Downloads, Latest Release, Last Update]
      - Last Updated Timestamp
      - Automation Note
  
  - Overview Section:
      - All-Time Downloads Count
      - Link to DOWNLOAD_STATS.md
  
  - Latest Release Performance:
      - Latest Version
      - Release Date
      - Downloads (7 days, 30 days, total)
  
  - Platform Distribution:
      - Current Release (ASCII chart)
      - All Time (ASCII chart)
  
  - Historical Trends:
      - Monthly Summary Table
      - Release Performance Table
  
  - Visualizations:
      - Download Trends Chart (PNG)
      - Platform Distribution Chart (PNG)
```

### DOWNLOAD_STATS.md Structure

```yaml
Document:
  - Header:
      - Last Updated Timestamp
  
  - By Release Table:
      - Columns: [Version, Release Date, Total, macOS, Windows, Linux, Other]
  
  - By Platform Table:
      - Columns: [Platform, Downloads, Percentage]
  
  - By Asset Table:
      - Columns: [File Name, Version, Platform, Size, Downloads]
```

### Validation Rules

- Timestamps MUST be ISO 8601 with UTC timezone
- Numbers MUST use comma separators (1,234)
- Percentages MUST sum to ~100% (within rounding error)
- Links to images MUST use relative paths (docs/images/*)
- Tables MUST be properly formatted Markdown

### Update Mechanism

- Updated by: `download-stats.yml` workflow (daily cron)
- Data source: GitHub Releases API
- Historical data: `docs/data/stats_YYYYMMDD.json`

---

## Entity: GitHub Actions Workflows

**Purpose**: CI/CD automation for builds, releases, and documentation

**Format**: YAML (GitHub Actions schema)

### Workflow Entity Schema

```yaml
Workflow:
  name: <string>
  
  on:
    push:
      branches: [<branch_patterns>]
      tags: [<tag_patterns>]
    pull_request:
      branches: [<branch_patterns>]
    schedule:
      - cron: <cron_expression>
    workflow_dispatch:
      inputs:
        <input_name>:
          description: <string>
          required: <boolean>
          type: <string|boolean|choice>
  
  jobs:
    <job_id>:
      name: <string>
      runs-on: <ubuntu-latest|macos-latest|windows-latest>
      
      steps:
        - name: <string>
          uses: <action@version>
          with:
            <param>: <value>
        
        - name: <string>
          run: |
            <shell_commands>
```

### Workflow Inventory

| Workflow File | Trigger | Purpose | Status |
|---------------|---------|---------|--------|
| `ci.yml` | PR to develop/main, push to main/release/hotfix | Multi-platform builds + code quality | Exists (documented in constitution) |
| `test-build.yml` | Manual, PR to develop/main | Test builds for all platforms | Ready to move |
| `release.yml` | Tag push (`v*.*.*`) | Build release binaries | Ready to move (fix typo: realease→release) |
| `release-with-readme-update.yml` | Release published | Orchestrate post-release updates | Ready to move |
| `download-stats.yml` | Daily cron (00:00 UTC), manual | Collect statistics from GitHub API | Ready to move |
| `update-readme-downloads.yml` | Release published, manual | Update README download links | Ready to move |
| `update-file-sizes.yml` | Release published, manual | Update README file sizes | Ready to move |
| `changelog.yml` | Tag push, manual | Generate changelog from commits | Ready to move |

### Validation Rules

- `name` MUST be descriptive and unique
- `runs-on` MUST be valid GitHub runner
- Cron expressions MUST be valid 5-field format
- Actions MUST specify version (`@v2`, `@v3`, etc.)
- Secrets MUST use `${{ secrets.NAME }}` syntax
- Branch patterns MUST match GitFlow structure (develop, main, feature/*, release/*, hotfix/*)

### Workflow Relationships

```
release.yml (Tag pushed: v1.1.0)
    ↓ (on: release.published)
release-with-readme-update.yml
    ├── triggers → update-readme-downloads.yml
    ├── triggers → update-file-sizes.yml
    └── triggers → changelog.yml

download-stats.yml (Daily at 00:00 UTC)
    ├── fetches → GitHub Releases API
    ├── updates → STATISTICS.md
    ├── updates → DOWNLOAD_STATS.md
    ├── generates → docs/images/*.png
    └── stores → docs/data/stats_YYYYMMDD.json
```

---

## Entity: Shell Scripts

**Purpose**: Executable automation logic called by workflows or developers

**Format**: Bash shell scripts (zsh-compatible for macOS)

### Script Entity Schema

```yaml
Script:
  shebang: "#!/bin/bash"
  
  header:
    - GPL-3.0 License Header
    - Description
    - Usage instructions
  
  configuration:
    - set -e  # Exit on error
    - set -u  # Undefined variables are errors (optional)
  
  input_validation:
    - Argument count check
    - Argument type/format validation
    - Environment variable checks
  
  main_logic:
    - Commands
    - Function calls
  
  exit_handling:
    - exit 0 (success)
    - exit 1 (failure)
```

### Script Inventory

| Script File | Location | Purpose | Inputs | Outputs |
|-------------|----------|---------|--------|---------|
| `test-build-local.sh` | `scripts/` | Local build testing | Platform (macos/linux/all) | Build artifacts, exit code |
| `update-download-links.sh` | `scripts/` | Manual README updater | Tag version | Updated README.md |
| `release-post-process.sh` | `scripts/` | Post-release orchestration | Tag version | Multiple updated files |
| `build-macos.sh` | `scripts/` | macOS build | Build type (debug/release) | .app, .vst3, .component |
| `build-windows.ps1` | `scripts/` | Windows build (PowerShell) | Build type | .exe, .vst3 |
| `build-linux.sh` | `scripts/` | Linux build | Build type | Binary, .vst3, .lv2 |
| `auv3_aum_size.sh` | `scripts/` | AUv3 size calculator | .app path | Size metrics |

### Validation Rules

- MUST include shebang (`#!/bin/bash` or `#!/usr/bin/env zsh`)
- MUST include GPL-3.0 header
- MUST validate required inputs before execution
- SHOULD use `set -e` for error propagation
- SHOULD be executable (`chmod +x`)
- MUST exit with appropriate code (0=success, 1+=error)
- SHOULD include usage message when called without arguments

### Script Relationships

```
Workflows
    ├── ci.yml → build-macos.sh, build-linux.sh
    ├── release.yml → build-macos.sh, build-windows.ps1, build-linux.sh
    ├── test-build.yml → test-build-local.sh
    └── release-with-readme-update.yml → release-post-process.sh → update-download-links.sh

Developers (local)
    ├── test-build-local.sh (before pushing)
    ├── update-download-links.sh (manual README updates)
    └── release-post-process.sh (manual release tasks)
```

---

## Relationships

```
CONTRIBUTING.md
    ├── references → .specify/memory/gitflow-workflow.md
    ├── references → .specify/memory/constitution.md
    ├── references → .editorconfig (formatting tool)
    ├── references → .clang-format (C++ formatting tool)
    ├── references → .github/workflows/README.md (CI/CD docs)
    └── references → .specify/scripts/bash/* (helper scripts)

CHANGELOG.md
    ├── links to → GitHub issues (#<number>)
    ├── links to → GitHub PRs (#<number>)
    └── may be generated by → changelog.yml workflow

.github/pull_request_template.md
    ├── references → CONTRIBUTING.md (implicitly)
    ├── references → .specify/memory/constitution.md (compliance section)
    └── links to → specs/*/spec.md (feature specs)

.editorconfig
    └── applies to → Source/**/*.{cpp,h,mm}

.clang-format
    └── applies to → Source/**/*.{cpp,h,mm}

.gitignore
    └── filters → All repository files

STATISTICS.md
    ├── updated by → download-stats.yml workflow
    ├── links to → DOWNLOAD_STATS.md
    ├── links to → docs/images/download-trends.png
    └── links to → docs/images/platform-distribution.png

DOWNLOAD_STATS.md
    ├── updated by → download-stats.yml workflow
    └── data from → docs/data/stats_YYYYMMDD.json

Workflows (.github/workflows/*.yml)
    ├── triggered by → Git events (push, PR, tag, release)
    ├── triggered by → Schedule (cron)
    ├── calls → scripts/*.sh
    ├── updates → STATISTICS.md, DOWNLOAD_STATS.md, CHANGELOG.md, README.md
    └── uses → GitHub API (releases, artifacts)

Scripts (scripts/*.sh)
    ├── called by → Workflows
    ├── called by → Developers (local execution)
    └── uses → Build tools (CMake, xcodebuild, msbuild)
```

---

## State Transitions

### CHANGELOG.md Lifecycle

```
1. [Unreleased] Section Active
   ↓ (feature PR merged to develop)
2. Changes Added to [Unreleased]
   ↓ (release branch created)
3. [Unreleased] → [X.Y.Z] (version tagged)
   ↓ (continue development)
4. New [Unreleased] Section Created
```

### CONTRIBUTING.md Updates

```
1. Initial Creation (this feature)
   ↓
2. Minor Updates (workflow changes, new tools)
   ↓
3. Major Revisions (new platforms, architecture changes)
```

### Configuration Files (.editorconfig, .clang-format)

```
1. Initial Creation (this feature)
   ↓
2. Validation Against Existing Code
   ↓
3. Refinement (if diff impact > 5%)
   ↓
4. Stable (minimal changes unless JUCE style evolves)
```

### STATISTICS.md / DOWNLOAD_STATS.md Lifecycle

```
1. Initial Creation (manual templates)
   ↓
2. First Workflow Run (populate with real data)
   ↓
3. Daily Updates (download-stats.yml cron)
   ↓
4. Release Events (spike in download activity)
   ↓ (continuous)
5. Historical Data Accumulation (docs/data/*.json)
```

### Workflow Execution States

```
Queued → In Progress → [Success | Failure | Cancelled]
                            ↓
                    Artifacts Published
                    (build files, updated docs)
```

---

## File Locations Summary

| Entity | Path | Type | Managed By | Frequency |
|--------|------|------|------------|-----------|
| `.editorconfig` | `/` | Config | Manual | Rarely |
| `.clang-format` | `/` | Config | Manual | Rarely |
| `.gitignore` | `/` | Config | Manual | Occasionally |
| `CHANGELOG.md` | `/` | Doc | Manual / Workflow | Per release |
| `CONTRIBUTING.md` | `/` | Doc | Manual | Occasionally |
| `STATISTICS.md` | `/` | Doc | Workflow | Daily |
| `DOWNLOAD_STATS.md` | `/` | Doc | Workflow | Daily |
| `pull_request_template.md` | `/.github/` | Template | Manual | Rarely |
| `*.yml` | `/.github/workflows/` | Workflow | Manual | Occasionally |
| `test-build-local.sh` | `/scripts/` | Script | Manual | Rarely |
| `update-download-links.sh` | `/scripts/` | Script | Manual | Rarely |
| `release-post-process.sh` | `/scripts/` | Script | Manual | Rarely |
| `build-*.sh` / `build-*.ps1` | `/scripts/` | Script | Manual | Occasionally |
| `README-downloads-template.md` | `/docs/templates/` | Template | Manual | Rarely |
| `stats_YYYYMMDD.json` | `/docs/data/` | Data | Workflow | Daily |
| `*.png` (charts) | `/docs/images/` | Image | Workflow | Daily |
| `README.md` (workflows) | `/.github/workflows/` | Doc | Manual | Occasionally |

---

## Summary

All file structures defined with validation rules, including:

- ✅ Configuration files (.editorconfig, .clang-format, .gitignore)
- ✅ Documentation files (CHANGELOG, CONTRIBUTING, PR template)
- ✅ Statistics files (STATISTICS.md, DOWNLOAD_STATS.md)
- ✅ Workflow entities (8 GitHub Actions workflows)
- ✅ Script entities (7 shell scripts)
- ✅ Relationships mapped between all entities
- ✅ State transitions documented
- ✅ File locations and management specified

No traditional database entities - this is a documentation and automation infrastructure feature with structured text file schemas.

**Data Model Complete** - Proceed to Phase 1: Contracts (file templates)
