# Quickstart Guide: CI/CD Build Infrastructure Fix

**Feature**: 003-ci-build-fix  
**Date**: 2025-11-08  
**Audience**: ShowMIDI contributors and maintainers

## Overview

This guide provides quick reference for understanding and working with ShowMIDI's CI/CD build infrastructure after the fixes implemented in feature 003.

---

## 🎯 What Changed?

### Fixed Issues
1. ✅ **macOS builds** - Adaptive Xcode version selection (no more hardcoded 15.2)
2. ✅ **Linux CMake** - Robust dependency detection with graceful degradation
3. ✅ **Workflow efficiency** - Path filtering skips docs-only builds
4. ✅ **CLAP support** - Optional plugin format builds when dependencies present
5. ✅ **Documentation** - Comprehensive CI/CD guide in CONTRIBUTING.md

### Key Improvements
- **<5% failure rate** - Infrastructure resilience with auto-retry on timeout
- **<2 min** - CMake configuration on Linux
- **<15 min** - macOS builds (all formats)
- **<30 sec** - Doc-only PRs (builds skipped)

---

## 🚀 Quick Start for Contributors

### Before Opening a PR

1. **Build locally on your platform**:
   ```bash
   # macOS
   cd Builds/MacOSX
   xcodebuild -scheme ShowMIDI -configuration Release clean build
   
   # Linux
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   
   # Windows
   cmake -B build -G "Visual Studio 17 2022"
   cmake --build build --config Release
   ```

2. **Verify no warnings** (CI treats warnings as errors on Linux):
   ```bash
   # Check for warnings in build output
   grep -i warning build.log
   ```

3. **Check GPL-3.0 headers** on new files:
   ```cpp
   /*
     ==============================================================================
   
      ShowMIDI
      Copyright (C) 2023 Uwyn LLC.  https://www.uwyn.com
   
      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.
      ...
   */
   ```

### Understanding CI Results

When you open a PR, these workflows run automatically:

| Workflow | Runs When | What It Does |
|----------|-----------|--------------|
| **CI Build** | PR to develop/main | Builds on Linux/macOS/Windows, uploads artifacts |
| **Code Quality** | Every PR | Validates GPL headers, checks warnings |
| **Build Matrix** | Push to develop | Quick platform verification |

**Timeline**:
- Linux: ~10 minutes
- macOS: ~15 minutes
- Windows: ~20 minutes
- **Total**: ~20 minutes (jobs run in parallel)

### Interpreting Failures

#### ❌ "JUCE not found"
**Cause**: Submodules not initialized  
**Fix**: Workflow should auto-initialize. If persistent, check `.github/workflows/ci.yml` has:
```yaml
- uses: actions/checkout@v4
  with:
    submodules: recursive
```

#### ❌ "invalid developer directory" (macOS)
**Cause**: Xcode version mismatch (should be auto-fixed)  
**Fix**: Workflow adaptively selects Xcode. If still failing, check runner image Xcode availability:
```yaml
- name: List Xcode versions
  run: ls /Applications/ | grep Xcode
```

#### ❌ "warning: unused variable" (Linux)
**Cause**: Compiler warning (Linux treats warnings as errors)  
**Fix**: Fix the warning in source code:
```bash
# Local reproduction
cmake -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror"
cmake --build build
```

#### ⚠️ "CLAP skipped" (Warning, not error)
**Cause**: clap-juce-extensions submodule not initialized  
**Fix**: Optional - only needed for CLAP builds:
```bash
git submodule update --init --recursive
```

---

## 🏗️ Building Optional Formats

### CLAP Plugin

**Prerequisites**: clap-juce-extensions library

```bash
# Initialize submodule
git submodule update --init libs/clap-juce-extensions

# Build
cmake -B build_clap -DCMAKE_BUILD_TYPE=Release
cmake --build build_clap

# Output
build_clap/ShowMIDI_artefacts/Release/CLAP/ShowMIDI.clap
```

### LV2 Plugin (Linux only)

**Prerequisites**: LV2 development headers

```bash
# Ubuntu/Debian
sudo apt-get install lv2-dev

# Fedora
sudo dnf install lv2-devel

# Build
cmake -B build_lv2 -DCMAKE_BUILD_TYPE=Release
cmake --build build_lv2

# Output
build_lv2/ShowMIDI_artefacts/Release/LV2/ShowMIDI.lv2/
```

---

## 🔧 Workflow Customization

### Skip Builds for Documentation Changes

Workflows automatically skip heavy builds when **only** these files change:
- `**.md` (all Markdown files)
- `docs/**` (documentation directory)
- `*.txt` (text files in root)
- `LICENSE`, `COPYING.md`

**Example**:
```bash
# This PR will skip CI builds (docs only)
git checkout -b fix-readme
echo "Updated" >> README.md
git commit -am "docs: update README"
git push

# This PR will run CI builds (code change)
git checkout -b fix-bug
echo "// Fix" >> Source/Main.cpp
git commit -am "fix: null pointer check"
git push
```

### Trigger Full Build Manually

Even if builds were skipped, you can manually trigger:

1. Go to **Actions** tab on GitHub
2. Select **CI Build** workflow
3. Click **Run workflow**
4. Choose your branch
5. Click **Run workflow** button

---

## 📊 Monitoring CI Health

### Success Metrics

Per constitution and feature spec:

- ✅ **100%** success rate for clean code PRs
- ✅ **<5%** spurious failure rate (infrastructure issues)
- ✅ **90%** of failures have actionable error messages

### Checking Infrastructure Failures

If CI fails but your code is correct:

1. **Check runner status**: https://www.githubstatus.com/
2. **Re-run the job**: Click "Re-run failed jobs" button
3. **Auto-retry**: Timeout failures automatically retry once
4. **Report persistent issues**: Open issue with workflow run link

### Build Time Trends

Expected job durations:

| Job | Target | Acceptable | Investigate If |
|-----|--------|------------|----------------|
| Linux CMake config | <2 min | <5 min | >5 min |
| macOS build | <15 min | <20 min | >25 min |
| Windows build | <20 min | <25 min | >30 min |
| Concurrency cancel | <10 sec | <30 sec | >60 sec |

---

## 🎓 Advanced: CMake Configuration

### Environment Variables

| Variable | Purpose | Default |
|----------|---------|---------|
| `PATH_TO_JUCE` | Override JUCE location | `./JUCE` (submodule) |
| `CMAKE_BUILD_TYPE` | Debug or Release | `Release` |
| `BUILD_CLAP` | Enable CLAP format | Auto-detected |

**Example**: Use custom JUCE version
```bash
export PATH_TO_JUCE=/Users/me/custom-juce
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### Dependency Priority

**JUCE framework**:
1. `PATH_TO_JUCE` environment variable (highest priority)
2. `./JUCE` git submodule
3. Fail with error if neither found

**Optional dependencies**:
1. Check if directory exists
2. If found: enable format, log STATUS message
3. If missing: disable format, log WARNING, continue

### Platform-Specific Behavior

**Linux**:
- Requires: ALSA, X11, Freetype2
- Optional: LV2 headers
- Warnings treated as errors: `-Wall -Wextra -Werror`

**macOS**:
- Xcode auto-selected (latest available)
- Builds: Standalone, VST3, AU (always), CLAP (optional)
- Code signing disabled in CI

**Windows**:
- Visual Studio 2022 required
- Builds: Standalone, VST3 (always), CLAP (optional)
- MSVC environment auto-initialized

---

## 🐛 Troubleshooting

### Local Build Matches CI

To reproduce CI environment locally:

**Linux (Ubuntu 22.04)**:
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y \
  cmake build-essential \
  libasound2-dev libx11-dev libxrandr-dev \
  libxinerama-dev libxcursor-dev libfreetype6-dev

# Build
git submodule update --init --recursive
cmake -B build -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror"
cmake --build build
```

**macOS**:
```bash
# Verify Xcode
xcode-select --print-path
xcodebuild -version

# Build
cd Builds/MacOSX
xcodebuild -scheme ShowMIDI -configuration Release clean build
```

### Common Issues

**"Could not find Git"**
- CI runners have git pre-installed
- Local: Install git via package manager

**"Submodule JUCE is empty"**
- Run: `git submodule update --init --recursive`
- Verify: `ls JUCE/` should show files

**"No Xcode version found"**
- macOS only issue
- Ensure Xcode installed: `xcode-select --install`
- Set path: `sudo xcode-select --switch /Applications/Xcode.app`

---

## 📚 Further Reading

- **Full CI/CD Guide**: See CONTRIBUTING.md § CI/CD Pipeline
- **Workflow Files**: `.github/workflows/ci.yml` (inline comments)
- **Constitution**: `.specify/memory/constitution.md` § Development Workflow
- **GitFlow**: `.specify/memory/gitflow-workflow.md`

---

## 🆘 Getting Help

If CI issues persist:

1. **Check this quickstart** (you are here)
2. **Read CONTRIBUTING.md** CI/CD section
3. **Review workflow logs** on GitHub Actions tab
4. **Ask on Discord**: https://discord.gg/TgsCnwqWTf
5. **Post on forum**: https://forum.uwyn.com

When reporting CI failures, include:
- Branch name
- Workflow run URL
- Error message from logs
- Local build result (if applicable)

---

**Last Updated**: 2025-11-08  
**Feature**: 003-ci-build-fix  
**Status**: ✅ Ready for implementation
