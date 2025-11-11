# CI/CD Workflows Overview 🚀

**Repository:** ShowMIDI-Fork  
**Last Updated:** 2025-11-08 19:36:33 UTC  
**Maintained by:** @peternicholls

This document provides a comprehensive guide to all automated workflows in this repository.

---

## 📋 Table of Contents

1. [Workflows Overview](#workflows-overview)
2. [Quick Start](#quick-start)
3. [Workflow Details](#workflow-details)
4. [Triggering Workflows](#triggering-workflows)
5. [Configuration & Secrets](#configuration--secrets)
6. [Troubleshooting](#troubleshooting)
7. [Best Practices](#best-practices)
8. [Roadmap](#roadmap)

---

## 🛠️ Workflows Overview

| Workflow | File | Purpose | Trigger |
|----------|------|---------|---------|
| **Test Build** | `test-build.yml` | Multi-platform build testing | Manual, PR |
| **Release** | `release.yml` | Build and package releases | Git tags (v*.*.*) |
| **Update Downloads** | `update-readme-downloads.yml` | Auto-update download links | Release published |
| **Download Stats** | `download-stats.yml` | Track download metrics | Daily schedule |
| **Changelog** | `changelog.yml` | Auto-generate changelog | Git tags |
| **File Sizes** | `update-file-sizes.yml` | Update asset sizes in README | Release published |
| **Release + README** | `release-with-readme-update.yml` | Orchestrate release updates | Git tags |

---

## 🚀 Quick Start

### For Contributors: Testing Your Changes

```bash
# Test locally before pushing
./scripts/test-build-local.sh Release

# Or create a PR to trigger automated tests
git checkout -b my-feature
git push origin my-feature
# Open PR on GitHub
```

### For Maintainers: Creating a Release

```bash
# 1. Tag a new version
git tag -a v1.2.3 -m "Release version 1.2.3"
git push origin v1.2.3

# 2. Workflows automatically:
#    - Build all platforms
#    - Create GitHub Release
#    - Update download links
#    - Generate changelog
#    - Update file sizes
#    - Collect statistics

# 3. Review and approve README update PR
```

---

## 📖 Workflow Details

### 1️⃣ Test Build Workflow (`test-build.yml`)

**Purpose:** Validate builds across all supported platforms before release.

**Platforms Tested:**
- 🍎 **macOS** (Intel + Apple Silicon universal binary)
- 🪟 **Windows** (64-bit, MSVC)
- 🐧 **Linux** (Ubuntu with common dependencies)

**Triggers:**
- Manual dispatch via Actions tab
- Pull requests to `main` or `develop` branches
- Changes to source files (`.cpp`, `.h`, `.cmake`, etc.)

**Manual Trigger Options:**
- **Platform:** `all`, `macos`, `windows`, or `linux`
- **Build Type:** `Debug` or `Release`
- **Verbose:** Enable detailed logging

**Outputs:**
- Build artifacts (retained 3 days)
- Build summary report
- Platform-specific binaries

**How to Run:**
1. Go to **Actions** → **Test Build Workflow**
2. Click **Run workflow**
3. Select options and click green **Run workflow** button

---

### 2️⃣ Release Workflow (`release.yml`)

**Purpose:** Build production-ready binaries and create GitHub Release.

**Triggers:**
- Git tags matching `v*.*.*` (e.g., `v1.0.0`)
- Manual dispatch with version input

**Build Process:**

**macOS:**
- Universal binary (Intel + Apple Silicon)
- DMG installer with drag-to-Applications layout
- Code signing ready (requires certificates)

**Windows:**
- MSVC 64-bit build
- Portable ZIP package
- Inno Setup installer ready (future)

**Linux:**
- AppImage (universal)
- DEB package ready (future)
- Tarball distribution

**Outputs:**
- GitHub Release with all platform binaries
- Release notes
- Download statistics baseline

---

### 3️⃣ Update Downloads Workflow (`update-readme-downloads.yml`)

**Purpose:** Keep README download links current after each release.

**Triggers:**
- Automatically when release is published
- Manual dispatch for specific release tag

**Process:**
1. Fetches latest release information
2. Generates download section with current links
3. Updates README.md
4. Creates Pull Request for review
5. Optional: Auto-merge (if configured)

**Updates:**
- Download links for all platforms
- Release version and date
- File sizes (in conjunction with `update-file-sizes.yml`)

---

### 4️⃣ Download Statistics (`download-stats.yml`)

**Purpose:** Track download metrics and generate reports.

**Triggers:**
- Daily at 00:00 UTC (scheduled)
- Manual dispatch for immediate update

**Collects:**
- Total downloads across all releases
- Per-release download counts
- Platform distribution (macOS/Windows/Linux)
- Per-asset download counts

**Outputs:**
- `DOWNLOAD_STATS.md` - Detailed statistics table
- Historical data in `docs/data/` (JSON)
- Download trend charts (future)
- Monthly report issues (on 1st of month)

**Data Stored:**
- Daily snapshots in JSON format
- Cumulative statistics
- Platform preferences
- Release performance metrics

---

### 5️⃣ Changelog Generator (`changelog.yml`)

**Purpose:** Automatically generate release notes from commit history.

**Triggers:**
- Git tags matching `v*.*.*`
- Manual dispatch

**Features:**
- Categorized changes (Features, Fixes, Docs, etc.)
- Conventional commit support
- Contributor statistics
- File change summary
- Previous release comparison

**Output Format:**
```markdown
## ✨ Features
- feat(ui): Add dark mode support (abc123)

## 🐛 Bug Fixes
- fix(audio): Resolve MIDI timing issue (def456)
```

**Updates:**
- `CHANGELOG.md` in repository
- Release notes on GitHub Release page

---

### 6️⃣ File Size Updates (`update-file-sizes.yml`)

**Purpose:** Keep README file sizes accurate and current.

**Triggers:**
- Automatically after release is published
- Manual dispatch

**Process:**
1. Fetches release asset information from GitHub API
2. Calculates human-readable sizes (MB/GB)
3. Updates README.md download table
4. Commits changes

**Updates:**
- Individual file sizes for each platform
- Total release size badge
- Formatted as user-friendly (e.g., "15.2 MB")

---

### 7️⃣ Release + README Update (`release-with-readme-update.yml`)

**Purpose:** Orchestrate post-release automation.

**Triggers:**
- Git tags matching `v*.*.*`

**Coordinates:**
1. Release build completion
2. README download link updates
3. File size detection
4. Changelog generation
5. Statistics collection

---

## 🔧 Configuration & Secrets

### Required Secrets

Currently uses only:
- `GITHUB_TOKEN` - Automatically provided by GitHub Actions

### Optional Secrets (for future enhancements)

**macOS Code Signing:**
```
MACOS_CERTIFICATE          # Base64-encoded .p12 certificate
MACOS_CERTIFICATE_PWD      # Certificate password
MACOS_IDENTITY             # Developer ID Application identity
APPLE_ID                   # Apple ID for notarization
APPLE_ID_PASSWORD          # App-specific password
APPLE_TEAM_ID              # Apple Developer Team ID
```

**Windows Code Signing:**
```
WINDOWS_CERTIFICATE        # Base64-encoded .pfx certificate
WINDOWS_CERTIFICATE_PWD    # Certificate password
```

### Adding Secrets

1. Go to **Settings** → **Secrets and variables** → **Actions**
2. Click **New repository secret**
3. Enter name and value
4. Click **Add secret**

---

## 🔍 Troubleshooting

### Common Issues

#### "Run workflow" button not visible (iPad/mobile)
**Solution:**
- Request desktop site in Safari
- Or create a PR to trigger automated tests
- Or use GitHub mobile app

#### Build fails on specific platform
**Solution:**
1. Check workflow logs for detailed error
2. Test locally with `./scripts/test-build-local.sh`
3. Verify dependencies in workflow file
4. Check CMakeLists.txt configuration

#### Download links not updating
**Solution:**
1. Check if `update-readme-downloads.yml` ran successfully
2. Verify PR was created and merged
3. Manually run workflow with specific tag
4. Check for merge conflicts in README.md

#### Statistics not collecting
**Solution:**
1. Verify `download-stats.yml` has proper permissions
2. Check GitHub API rate limits
3. Ensure releases have published assets
4. Review workflow logs for API errors

#### Changelog not generating
**Solution:**
1. Ensure commits follow conventional commit format
2. Verify git history is accessible (fetch-depth: 0)
3. Check if auto-changelog is installed correctly
4. Review commit messages for proper formatting

---

## ✅ Best Practices

### For Contributors

1. **Test locally first:** Run `./scripts/test-build-local.sh` before pushing
2. **Follow commit conventions:** Use `feat:`, `fix:`, `docs:`, etc.
3. **Create PRs:** Let automated tests run before merging
4. **Check build logs:** Review any warnings or errors

### For Maintainers

1. **Semantic versioning:** Follow `vMAJOR.MINOR.PATCH` format
2. **Test releases:** Use manual dispatch to test before tagging
3. **Review PRs:** Check automated README updates before merging
4. **Monitor statistics:** Review download trends monthly
5. **Update workflows:** Keep actions versions current

### Commit Message Format

```bash
# Feature
git commit -m "feat(midi): Add velocity curve adjustment"

# Bug fix
git commit -m "fix(ui): Resolve window resize crash"

# Documentation
git commit -m "docs(readme): Update installation instructions"

# Performance
git commit -m "perf(audio): Optimize MIDI message processing"

# Refactor
git commit -m "refactor(core): Simplify event handling"
```

---

## 🗓️ Workflow Execution Order

For a typical release (`v1.2.3`):

```
1. Git tag pushed (v1.2.3)
   ↓
2. release.yml triggers
   ├─ Build macOS
   ├─ Build Windows  
   └─ Build Linux
   ↓
3. GitHub Release created
   ↓
4. Parallel execution:
   ├─ changelog.yml (update CHANGELOG.md)
   ├─ update-file-sizes.yml (detect asset sizes)
   └─ update-readme-downloads.yml (update links)
   ↓
5. PR created for README updates
   ↓
6. Review & merge PR
   ↓
7. download-stats.yml (next scheduled run)
```

---

## 📊 Monitoring Workflows

### View Workflow Status

**Repository badges:**
```markdown
![Build Status](https://github.com/peternicholls/ShowMIDI-Fork/actions/workflows/test-build.yml/badge.svg)
```

**Actions Dashboard:**
- Go to **Actions** tab
- View all workflow runs
- Filter by workflow, status, branch
- Download logs and artifacts

### Workflow Run Times (Average)

| Workflow | Duration |
|----------|----------|
| Test Build (all platforms) | 8-12 min |
| Release (all platforms) | 15-20 min |
| Update Downloads | 1-2 min |
| Download Stats | 2-3 min |
| Changelog | 1-2 min |
| File Sizes | 1-2 min |

---

## 🗺️ Roadmap

### High Priority
- [ ] Add unit test execution to test-build workflow
- [ ] Implement Apple notarization for macOS releases
- [ ] Add Windows code signing
- [ ] Create Inno Setup installer for Windows
- [ ] Generate proper AppImage for Linux

### Medium Priority
- [ ] Add RPM package for Linux
- [ ] Implement Snap/Flatpak packaging
- [ ] Create download trend visualizations
- [ ] Add performance benchmarking
- [ ] Automated security scanning

### Low Priority
- [ ] Homebrew formula automation
- [ ] Windows Store (MSIX) packaging
- [ ] Docker build containers
- [ ] Nightly builds from main branch

---

## 📚 Additional Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [JUCE CMake Documentation](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md)
- [Semantic Versioning](https://semver.org/)
- [Conventional Commits](https://www.conventionalcommits.org/)

---

## 🆘 Getting Help

**Issues with workflows?**
1. Check this documentation
2. Review workflow logs in Actions tab
3. Search existing GitHub Issues
4. Create new issue with `workflow` label
5. Tag @peternicholls for maintainer attention

**Want to contribute?**
1. Fork the repository
2. Create feature branch
3. Make changes
4. Test locally
5. Submit Pull Request
6. Automated tests will run

---

*Last updated: 2025-11-08 19:36:33 UTC*