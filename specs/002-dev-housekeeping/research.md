# Research: Development Housekeeping

**Phase**: 0 (Outline & Research)
**Date**: 2025-11-08
**Feature**: Development documentation and repository configuration

## Research Questions

### RQ1: What clang-format rules align with JUCE framework coding style?

**Decision**: Use LLVM base style with custom modifications for JUCE compatibility

**Rationale**:
- JUCE uses Allman brace style (braces on new line)
- 4-space indentation is standard across JUCE codebase
- JUCE prefers `juce::` namespace prefix over `using namespace`
- Column limit typically 120-140 characters for readability
- Pointer/reference alignment: `Type* ptr` (asterisk with type)

**Key clang-format settings**:
```yaml
BasedOnStyle: LLVM
BreakBeforeBraces: Allman
IndentWidth: 4
ColumnLimit: 120
PointerAlignment: Left
AlignConsecutiveAssignments: false
AlignConsecutiveMacros: true
```

**Alternatives considered**:
- Google style: Rejected (uses attached braces, conflicts with JUCE)
- Mozilla style: Rejected (2-space indent, conflicts with JUCE 4-space standard)
- WebKit style: Partially compatible but requires more customization than LLVM

**Validation approach**: Run clang-format on existing Source/ directory to measure diff impact (should be < 5% of lines per SC-004)

---

### RQ2: What CHANGELOG format best supports GitFlow workflow and semantic versioning?

**Decision**: Keep a Changelog format (https://keepachangelog.com)

**Rationale**:
- Industry standard with widespread recognition
- Explicitly designed for semantic versioning
- Clear categorization (Added, Changed, Fixed, etc.) maps to conventional commits
- Human-readable format (Markdown)
- Supports comparison links between versions
- Aligns with GitFlow release process (one changelog section per release)

**Format structure**:
```markdown
# Changelog

## [Unreleased]
### Added
- New features committed to develop but not yet released

## [1.1.0] - 2025-11-15
### Added
- Feature X
### Changed
- Improvement Y
### Fixed
- Bug Z

## [1.0.1] - 2025-11-01
...
```

**Alternatives considered**:
- Conventional Changelog (auto-generated): Rejected (requires tooling, loses human curation)
- Simple git log dump: Rejected (not user-friendly, includes implementation details)
- Release notes in GitHub only: Rejected (should be version-controlled in repo)

---

### RQ3: What should CONTRIBUTING.md cover for C++/JUCE project with GitFlow?

**Decision**: Comprehensive guide covering environment setup, GitFlow workflow, code style, and PR process

**Rationale**:
Based on open-source best practices and ShowMIDI's specific needs:

**Required sections**:
1. **Getting Started**
   - Prerequisites (C++17 compiler, CMake/Xcode/VS2022, JUCE submodule)
   - Platform-specific setup (macOS, Windows, Linux)
   - Building from source (CMake and Projucer paths)

2. **Development Workflow**
   - GitFlow branching (link to `.specify/memory/gitflow-workflow.md`)
   - Feature branch creation (use helper scripts)
   - Conventional Commits format with ShowMIDI scopes
   - PR submission process

3. **Code Style**
   - GPL-3.0 header requirement on new files
   - JUCE framework patterns (leak detector, namespace, types)
   - DPI scaling using `sm::scaled()`
   - EditorConfig and clang-format usage

4. **Testing & Quality**
   - Manual testing requirements (macOS minimum)
   - No JUCE leak warnings policy
   - Constitution compliance for architecture changes

5. **Community**
   - Forum and Discord links
   - Code of conduct reference
   - Issue reporting process

**Alternatives considered**:
- Minimal README-only instructions: Rejected (insufficient for complex C++/JUCE setup)
- Separate BUILDING.md + CONTRIBUTING.md: Rejected (creates fragmentation, prefer single source)

**References**:
- GitHub's CONTRIBUTING.md guide
- JUCE forum best practices
- Existing ShowMIDI README.md

---

### RQ4: What .editorconfig rules ensure cross-platform consistency?

**Decision**: UTF-8 encoding, LF line endings, 4-space indent for C++, trim whitespace

**Rationale**:
- **UTF-8**: Universal encoding, handles all characters including copyright symbols in headers
- **LF line endings**: Git default, works on all platforms with proper git config
- **4-space indent**: Matches JUCE and existing ShowMIDI code
- **Trim whitespace**: Prevents noisy diffs
- **Final newline**: POSIX standard, required by some compilers

**.editorconfig** core settings:
```ini
root = true

[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
trim_trailing_whitespace = true

[*.{cpp,h}]
indent_style = space
indent_size = 4

[*.{md,txt}]
indent_style = space
indent_size = 2
trim_trailing_whitespace = false  # Markdown uses trailing spaces

[Makefile]
indent_style = tab
```

**Alternatives considered**:
- CRLF line endings: Rejected (causes cross-platform diffs despite git autocrlf)
- Tab indentation: Rejected (JUCE uses spaces, mixing creates inconsistency)
- 2-space indent: Rejected (conflicts with existing 4-space standard)

**Editor support**: VS Code, CLion, Vim, Emacs, Sublime Text, Atom (all major editors)

---

### RQ5: What .gitignore patterns are missing from current configuration?

**Decision**: Add macOS (.DS_Store), Linux, and IDE-specific patterns

**Current .gitignore analysis**:
```
✅ Already covered:
- Build artifacts (Builds/*/build)
- Xcode user data
- Visual Studio (.vs/, x64/)
- VST2 SDK (libs/vst2)

❌ Missing patterns:
- .DS_Store (macOS Finder metadata)
- Thumbs.db (Windows thumbnails)
- CMake build directories (build/, cmake-build-*)
- IDE folders (.vscode/, .idea/, .clangd/)
- Python cache (__pycache__/, *.pyc) - for any future scripts
- Temporary files (*~, *.swp, *.swo)
```

**Additions to .gitignore**:
```gitignore
# macOS
.DS_Store
.AppleDouble
.LSOverride

# Windows
Thumbs.db
ehthumbs.db
Desktop.ini

# Linux
*~
.directory

# IDEs
.vscode/
.idea/
.clangd/
*.code-workspace

# CMake
build/
cmake-build-*/
CMakeCache.txt
CMakeFiles/

# Python (for future scripts)
__pycache__/
*.pyc
*.pyo

# Temporary files
*.swp
*.swo
*.bak
*.tmp
```

**Rationale**: Comprehensive coverage prevents accidental commits of local configuration and build artifacts across all developer platforms.

---

### RQ6: What should GitHub PR template include for ShowMIDI?

**Decision**: Structured template with description, testing checklist, related issues, and constitution compliance

**Template sections**:

1. **Description**
   - What does this PR do?
   - Why is this change needed?
   - Link to feature spec (if applicable)

2. **Type of Change**
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Breaking change
   - [ ] Documentation update

3. **Testing Checklist**
   - [ ] Built successfully on macOS
   - [ ] Built successfully on Windows (if applicable)
   - [ ] Built successfully on Linux (if applicable)
   - [ ] Manual testing completed
   - [ ] No JUCE leak detector warnings

4. **Code Quality**
   - [ ] GPL-3.0 headers on new source files
   - [ ] Conventional Commit message format
   - [ ] Code follows JUCE patterns
   - [ ] DPI scaling using `sm::scaled()` (if UI changes)

5. **Related Issues**
   - Fixes #
   - Related to #

6. **Constitution Compliance** (for architecture changes)
   - [ ] Multi-platform architecture considered
   - [ ] Real-time performance impact assessed
   - [ ] Maintainability reviewed
   - Link to constitution section: 

**Rationale**: 
- Ensures consistent PR quality
- Reduces reviewer burden by providing complete context
- Enforces constitution checks for significant changes
- Aligns with GitFlow workflow (references issues, links specs)

**Alternatives considered**:
- No template: Rejected (leads to inconsistent PRs)
- Minimal template: Rejected (misses critical quality checks)
- Multiple templates (bug/feature): Rejected (creates confusion, single template with type selection is clearer)

---

### RQ7: How should existing CI/CD workflows be organized and integrated?

**Decision**: Move workflows from `.github/prompts/workflows/` to `.github/workflows/`, consolidate scripts in `scripts/`, and document in CONTRIBUTING.md

**Current State Analysis**:

**Workflows** (currently in `.github/prompts/workflows/`):
1. `ci.yml` - Multi-platform build validation (already documented in constitution)
2. `test-build.yml` - Manual and PR-triggered test builds
3. `realease.yml` - Release build workflow (note: typo in filename)
4. `update-readme-downloads.yml` - Auto-update download links in README
5. `download-stats.yml` - Daily download statistics collector
6. `changelog.yml` - Automatic changelog generator
7. `update-file-sizes.yml` - Auto-update file sizes in README
8. `release-with-readme-update.yml` - Orchestrator for post-release updates
9. `README.md` - CI/CD workflows documentation

**Scripts** (split locations):
- `.github/prompts/workflows/test-build-local.sh` → move to `scripts/`
- `.github/prompts/workflows/update-download-links.sh` → move to `scripts/`
- `scripts/relase-post-process.sh` → rename to `release-post-process.sh` (fix typo)

**Documentation**:
- `STATISTICS.md` - Statistics dashboard (exists)
- `docs/templates/README-downloads-template.md` - Download section template (exists)
- `DOWNLOAD_STATS.md` - Detailed stats table (needs creation)

**Rationale**:
- **GitHub Actions requirement**: Workflows MUST be in `.github/workflows/` to execute
- **Consistency**: All scripts should be in `scripts/` directory
- **Free tier optimization**: Workflows designed for GitHub Free limits (2,000 CI minutes/month)
- **GitFlow alignment**: Workflows trigger on correct branches

**Migration Actions**:

| Source | Destination | Notes |
|--------|-------------|-------|
| `.github/prompts/workflows/*.yml` | `.github/workflows/` | Move all 8 workflow files |
| `.github/prompts/workflows/README.md` | `.github/workflows/README.md` | Move documentation |
| `.github/prompts/workflows/test-build-local.sh` | `scripts/test-build-local.sh` | Move script |
| `.github/prompts/workflows/update-download-links.sh` | `scripts/update-download-links.sh` | Move script |
| `scripts/relase-post-process.sh` | `scripts/release-post-process.sh` | Rename (fix typo) |
| `.github/prompts/workflows/realease.yml` | `.github/workflows/release.yml` | Move + rename (fix typo) |

**Workflow Triggers** (GitFlow-aligned):

| Workflow | Triggers |
|----------|----------|
| `ci.yml` | PR to `develop`/`main`, push to `main`/`release/**`/`hotfix/**` |
| `test-build.yml` | Manual, PR to `develop`/`main` |
| `release.yml` | Tag push (`v*.*.*`) |
| `release-with-readme-update.yml` | Release published |
| `download-stats.yml` | Daily cron (00:00 UTC), manual |
| `update-readme-downloads.yml` | Release published, manual |
| `update-file-sizes.yml` | Release published, manual |
| `changelog.yml` | Tag push, manual |

**Alternatives Considered**:
- **Keep workflows in prompts/**: Rejected (GitHub Actions won't execute)
- **Manual release process**: Rejected (automation reduces errors)
- **External CI (CircleCI, Travis)**: Rejected (GitHub Actions is free and integrated)

---

## Summary

All research questions resolved with concrete decisions:

1. **clang-format**: LLVM base + Allman braces + JUCE-specific customizations
2. **CHANGELOG**: Keep a Changelog format with semantic versioning
3. **CONTRIBUTING**: Comprehensive guide covering setup, workflow, style, testing
4. **EditorConfig**: UTF-8, LF, 4-space indent, whitespace trimming
5. **.gitignore**: Add macOS, Windows, Linux, IDE, CMake patterns
6. **PR template**: Structured with testing checklist and constitution compliance
7. **CI/CD workflows**: Move to `.github/workflows/`, consolidate scripts, document automation

**No unresolved clarifications** - ready for Phase 1 (Design & Contracts).
