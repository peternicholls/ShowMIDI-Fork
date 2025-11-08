# File Migration & Organization Contract

**Feature**: 002-dev-housekeeping  
**Date**: 2025-11-08  
**Purpose**: Define exact file movements and organization for CI/CD integration

---

## Migration Actions

### 1. Workflow Files

**Source**: `.github/prompts/workflows/`  
**Destination**: `.github/workflows/`  
**Action**: Move all files

| Current Name | Final Name | Notes |
|--------------|------------|-------|
| `ci.yml` | `ci.yml` | Already documented in constitution |
| `test-build.yml` | `test-build.yml` | No changes |
| `realease.yml` | `release.yml` | **FIX TYPO** |
| `update-readme-downloads.yml` | `update-readme-downloads.yml` | No changes |
| `download-stats.yml` | `download-stats.yml` | No changes |
| `changelog.yml` | `changelog.yml` | No changes |
| `update-file-sizes.yml` | `update-file-sizes.yml` | No changes |
| `release-with-readme-update.yml` | `release-with-readme-update.yml` | No changes |
| `README.md` | `README.md` | No changes |

### 2. Script Files

**Source**: `.github/prompts/workflows/` and `scripts/`  
**Destination**: `scripts/`  
**Action**: Move and consolidate

| Current Location | Current Name | Final Location | Final Name | Notes |
|-----------------|--------------|----------------|------------|-------|
| `.github/prompts/workflows/` | `test-build-local.sh` | `scripts/` | `test-build-local.sh` | Move |
| `.github/prompts/workflows/` | `update-download-links.sh` | `scripts/` | `update-download-links.sh` | Move |
| `scripts/` | `relase-post-process.sh` | `scripts/` | `release-post-process.sh` | **FIX TYPO** |
| `scripts/` | `build-macos.sh` | `scripts/` | `build-macos.sh` | No changes |
| `scripts/` | `build-windows.ps1` | `scripts/` | `build-windows.ps1` | No changes |
| `scripts/` | `build-linux.sh` | `scripts/` | `build-linux.sh` | No changes |
| `scripts/` | `auv3_aum_size.sh` | `scripts/` | `auv3_aum_size.sh` | No changes |

### 3. New Files to Create

| File | Location | Purpose |
|------|----------|---------|
| `DOWNLOAD_STATS.md` | Repository root | Detailed download statistics table (referenced by STATISTICS.md) |
| `.editorconfig` | Repository root | Cross-editor formatting rules |
| `.clang-format` | Repository root | C++ code formatting rules |
| `CHANGELOG.md` | Repository root | Version history |
| `CONTRIBUTING.md` | Repository root | Contributor guide |
| `.github/pull_request_template.md` | `.github/` | PR submission template |

### 4. Files to Update

| File | Location | Changes |
|------|----------|---------|
| `.gitignore` | Repository root | Add macOS, Windows, Linux, IDE, CMake patterns |
| `README.md` | Repository root | Link to CONTRIBUTING.md, STATISTICS.md, CHANGELOG.md |
| `.github/copilot-instructions.md` | `.github/` | Add CI/CD technologies and workflow patterns |

---

## Cleanup Actions

### Remove Empty Directories

After migration, these directories should be checked and removed if empty:

- `.github/prompts/workflows/` (after moving all files to `.github/workflows/`)

---

## Verification Checklist

After migration:

- [ ] All workflows execute from `.github/workflows/`
- [ ] All scripts are in `scripts/` directory
- [ ] Typos fixed (`realease.yml` → `release.yml`, `relase-post-process.sh` → `release-post-process.sh`)
- [ ] Scripts maintain executable permissions (`chmod +x`)
- [ ] Workflow references to scripts use correct paths
- [ ] `.github/prompts/workflows/` directory empty or removed
- [ ] All new files created with proper content
- [ ] Updated files have correct additions
- [ ] Git status shows expected changes only

---

## Implementation Order

1. **Phase 1**: Create new configuration files (.editorconfig, .clang-format, .gitignore updates)
2. **Phase 2**: Create new documentation files (CHANGELOG.md, CONTRIBUTING.md, PR template)
3. **Phase 3**: Move workflows from prompts/workflows to workflows/
4. **Phase 4**: Move scripts from prompts/workflows to scripts/
5. **Phase 5**: Rename files with typos
6. **Phase 6**: Create DOWNLOAD_STATS.md
7. **Phase 7**: Update references in existing files (README.md, copilot-instructions.md)
8. **Phase 8**: Verify and test

---

## Success Criteria

- ✅ `git status` shows no untracked workflow or script files in old locations
- ✅ GitHub Actions recognizes workflows (visible in Actions tab)
- ✅ All scripts execute successfully from new locations
- ✅ Documentation files pass markdown linting
- ✅ Configuration files validated (editorconfig, clang-format)
- ✅ Constitution updated with new technologies
