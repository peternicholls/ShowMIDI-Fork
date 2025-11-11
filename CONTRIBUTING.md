# Contributing to ShowMIDI

Thank you for your interest in contributing to ShowMIDI! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Code Style](#code-style)
- [Testing](#testing)
- [CI/CD Pipeline](#cicd-pipeline)
- [Pull Request Process](#pull-request-process)
- [Community](#community)

---

## Getting Started

### Prerequisites

To build ShowMIDI, you'll need:

- **C++17 compatible compiler**:
  - macOS: Xcode 10.0 or later
  - Windows: Visual Studio 2022 or later
  - Linux: GCC 7.0+ or Clang 5.0+
- **CMake 3.15+** (for CMake builds) or **Projucer** (for platform-specific builds)
- **JUCE framework** (included as git submodule)
- **Git** for version control

### Platform-Specific Setup

#### macOS

1. Install Xcode from the App Store
2. Install Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```
3. (Optional) Install CMake via Homebrew:
   ```bash
   brew install cmake
   ```

#### Windows

1. Install Visual Studio 2022 (Community Edition is free)
   - Select "Desktop development with C++" workload
   - Include CMake tools component
2. Install Git for Windows

#### Linux

Install required dependencies:

**Ubuntu/Debian**:
```bash
sudo apt install build-essential pkg-config cmake \
  libasound2-dev libfreetype6-dev libx11-dev \
  libxrandr-dev libxinerama-dev libxcursor-dev
```

**Fedora/RHEL**:
```bash
sudo dnf install gcc-c++ cmake alsa-lib-devel \
  freetype-devel libX11-devel libXrandr-devel \
  libXinerama-devel libXcursor-devel
```

### Cloning the Repository

ShowMIDI uses JUCE as a git submodule, so you must initialize submodules after cloning:

```bash
git clone https://github.com/peternicholls/ShowMIDI-Fork.git
cd ShowMIDI-Fork
git submodule update --init --recursive
```

### Building from Source

#### Using CMake (Recommended for all platforms)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

#### Using Projucer

1. Open `showmidi.jucer` in Projucer
2. Click "Save Project" to generate platform-specific build files
3. Open the generated project in your IDE:
   - macOS: `Builds/MacOSX/ShowMIDI.xcodeproj`
   - Windows: `Builds/VisualStudio2022/ShowMIDI.sln`
   - Linux: `Builds/LinuxMakefile/Makefile`
4. Build the project in your IDE

---

## Development Workflow

ShowMIDI follows the **Atlassian GitFlow** branching strategy. For complete details, see [`.specify/memory/gitflow-workflow.md`](.specify/memory/gitflow-workflow.md).

### Branch Structure

- **`main`**: Production releases only (tagged: `v1.0.0`, `v1.0.1`, etc.)
- **`develop`**: Integration branch for features
- **`feature/*`**: Feature branches (e.g., `feature/001-dpi-scaling`)
- **`release/*`**: Release stabilization (e.g., `release/1.1.0`)
- **`hotfix/*`**: Critical production fixes (e.g., `hotfix/1.0.2`)

### Creating a Feature Branch

Use the helper script to create a feature branch:

```bash
.specify/scripts/bash/create-feature.sh 003 "midi-export"
```

Or manually:

```bash
git checkout develop
git pull origin develop
git checkout -b feature/003-midi-export
```

### Conventional Commits

All commits MUST follow [Conventional Commits](https://www.conventionalcommits.org/) format:

**Format**: `<type>(<scope>): <subject>`

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only
- `style`: Formatting, whitespace
- `refactor`: Code restructuring
- `perf`: Performance improvement
- `test`: Add/update tests
- `build`: Build system (CMake, Projucer)
- `ci`: CI/CD changes
- `chore`: Maintenance (deps, versions)

**ShowMIDI Scopes**:
- `midi`: MIDI handling
- `ui`: User interface
- `theme`: Theme system
- `plugin`: Plugin formats (VST, AU, etc.)
- `settings`: Settings/preferences
- `juce`: JUCE integration
- `build`: Build configuration
- `release`: Release management

**Examples**:
```
feat(midi): add MPE manager visualization
fix(ui): correct DPI scaling on 4K displays
docs(readme): update Linux build instructions
chore(release): bump version to 1.1.0
```

### Submitting Changes

1. Ensure your feature branch is up to date:
   ```bash
   git checkout develop
   git pull origin develop
   git checkout feature/your-feature
   git merge develop
   ```

2. Push your feature branch:
   ```bash
   git push origin feature/your-feature
   ```

3. Open a Pull Request on GitHub targeting `develop` branch

---

## Code Style

### GPL-3.0 License Headers

All new source files (`.cpp`, `.h`, `.mm`) MUST include the GPL-3.0 header:

```cpp
/*
  ==============================================================================

   ShowMIDI
   Copyright (C) 2023 Uwyn LLC.  https://www.uwyn.com

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/
```

### JUCE Framework Patterns

**Component Declaration**:
```cpp
class MyComponent : public juce::Component
{
public:
    MyComponent();
    ~MyComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyComponent)
};
```

**Namespace Usage**:
- Use `showmidi` namespace for project code
- Prefix JUCE types: `juce::String`, `juce::Array`, etc.
- Do NOT use `using namespace juce;`

**Memory Management**:
- Use `std::unique_ptr` or `juce::OwnedArray` for ownership
- Avoid raw `new`/`delete`
- Use JUCE leak detector macro on all components
- No allocations on MIDI thread

### DPI Scaling

All dimensional values in UI code MUST use the `sm::scaled()` utility:

```cpp
// ✅ Correct
button.setBounds(sm::scaled(10), sm::scaled(20), 
                 sm::scaled(100), sm::scaled(30));

// ❌ Wrong
button.setBounds(10, 20, 100, 30);
```

### EditorConfig

ShowMIDI uses [EditorConfig](https://editorconfig.org/) for consistent formatting across editors. The `.editorconfig` file defines:

- UTF-8 encoding
- LF line endings
- 4-space indentation for C++
- Trailing whitespace trimming
- Final newline insertion

Most modern editors (VS Code, CLion, Vim, Emacs) support EditorConfig automatically. If formatting changes occur when you open files, ensure your editor has EditorConfig support enabled.

### clang-format

For C++ code formatting, ShowMIDI uses `clang-format` with JUCE-compatible settings:

**Installation**:
- **macOS**: `brew install clang-format`
- **Linux**: `sudo apt install clang-format` or `sudo dnf install clang`
- **Windows**: Download from [LLVM releases](https://releases.llvm.org/)

**Usage**:
```bash
# Format a single file in-place
clang-format -i Source/PluginProcessor.cpp

# Format all source files
find Source/ -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Check formatting without modifying (dry-run)
clang-format --dry-run -Werror Source/PluginProcessor.cpp
```

**IDE Integration**:
- **VS Code**: Install "C/C++" extension, enable "Format On Save"
- **CLion**: Settings → Editor → Code Style → Enable ClangFormat
- **Xcode**: Install ClangFormat-Xcode plugin

The `.clang-format` file in the repository root defines the JUCE style (Allman braces, 4-space indent, 120 column limit).

---

## Testing

### Manual Testing Requirements

All changes MUST be manually tested before submitting a PR:

1. **Build on Primary Platform**: At minimum, build and test on macOS
2. **Cross-Platform Verification**: For UI changes, test on macOS, Windows, and Linux if possible
3. **Plugin Format Testing**: If changes affect plugin code, test at least one plugin format (VST3 recommended)
4. **Theme Compatibility**: Verify changes work with both light and dark themes

### No JUCE Leak Warnings Policy

ShowMIDI enforces a **zero-tolerance policy** for JUCE leak detector warnings:

- All PRs MUST pass with no leak warnings in Debug builds
- Use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` on all component classes
- Use smart pointers (`std::unique_ptr`, `juce::OwnedArray`) for ownership
- Review leak warnings carefully - they often indicate real memory issues

### Running Local Tests

Before pushing, verify your changes:

```bash
# Build in Debug mode to enable leak detection
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build

# Run the standalone application
./build/ShowMIDI_artefacts/Debug/ShowMIDI

# Test MIDI functionality
# 1. Connect a MIDI device or use a virtual MIDI driver
# 2. Play notes, send CC messages, etc.
# 3. Verify visualization appears correctly
# 4. Check for any console warnings or errors
```

---

## CI/CD Pipeline

ShowMIDI uses GitHub Actions for continuous integration and delivery. Understanding how workflows behave helps you troubleshoot build failures and optimize your development workflow.

### Workflow Triggers

GitHub Actions workflows run automatically based on branch activity and file changes:

| Workflow | Triggers | Purpose | Skips on |
|----------|----------|---------|----------|
| **CI Build** (`ci.yml`) | PR → `develop`/`main`<br>Push → `main`/`release/*`/`hotfix/*` | Full platform validation (Linux, macOS, Windows) | Documentation-only changes (`**.md`, `docs/**`, `*.txt`) |
| **Test Build** (`test-build.yml`) | Push → `develop` | Quick build matrix test | N/A |
| **Changelog** (`changelog.yml`) | Tags `v*.*.*` only | Generate release notes | All non-tag pushes |

**Path Filtering Optimization**: The CI Build workflow skips heavy builds when only documentation files change, completing in under 30 seconds instead of 15+ minutes. This saves CI minutes and reduces wait time for doc-only PRs.

**Concurrency Control**: When you push multiple commits rapidly to the same branch, GitHub Actions automatically cancels in-progress workflow runs to avoid wasting resources on outdated code.

### Local Testing Before Pushing

**Always build locally before opening a PR** to catch errors early and reduce CI failures:

#### macOS (Xcode)

```bash
# Build Standalone
cd Builds/MacOSX
xcodebuild -project ShowMIDI.xcodeproj \
  -scheme ShowMIDI \
  -configuration Release \
  clean build

# Build VST3
xcodebuild -project ShowMIDI.xcodeproj \
  -scheme "ShowMIDI - VST3" \
  -configuration Release \
  clean build

# Build AU (Audio Unit)
xcodebuild -project ShowMIDI.xcodeproj \
  -scheme "ShowMIDI - AU" \
  -configuration Release \
  clean build
```

#### Linux (CMake)

```bash
# Configure CMake (first time or after CMakeLists.txt changes)
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build all targets
cmake --build . --config Release -j$(nproc)

# Verify artifacts created
ls -lh ShowMIDI_artefacts/Release/
```

**Common CMake Issues**:
- **JUCE not found**: Run `git submodule update --init --recursive`
- **Missing system libraries**: Install dependencies (see [Platform-Specific Setup](#platform-specific-setup))
- **Slow builds**: Use `-j$(nproc)` to parallelize compilation

#### Windows (Visual Studio 2022)

```powershell
# Using MSBuild from Developer Command Prompt
cd Builds\VisualStudio2022
msbuild ShowMIDI.sln /p:Configuration=Release /p:Platform=x64

# Or open ShowMIDI.sln in Visual Studio and build via IDE
```

### Common CI Failure Scenarios

Understanding frequent failures helps you resolve issues faster:

#### "JUCE not found" (Linux/macOS/Windows)

**Cause**: Git submodules not initialized or `PATH_TO_JUCE` environment variable pointing to invalid location.

**Error Message**:
```
CMake Error: JUCE not found. Please either:
  1. Set PATH_TO_JUCE environment variable to point to your JUCE installation, OR
  2. Initialize the JUCE submodule: git submodule update --init --recursive
```

**Solution**:
1. **Local fix**: `git submodule update --init --recursive`
2. **CI fix**: Workflow already runs `checkout` with `submodules: recursive` - if this fails, check if `.gitmodules` is corrupted
3. **Verification**: `ls -la JUCE/CMakeLists.txt` should show JUCE's CMake file

#### "invalid developer directory" (macOS)

**Cause**: Hardcoded Xcode version doesn't exist on GitHub Actions runner (runner images update regularly).

**Error Message**:
```
xcrun: error: unable to find utility "xcodebuild", not a developer tool or in PATH
xcode-select: error: tool 'xcodebuild' requires Xcode, but active developer directory
'/Applications/Xcode_15.2.app/Contents/Developer' does not exist
```

**Solution**:
- **CI handles this automatically**: Workflow now uses adaptive Xcode selection (detects latest available version)
- **Local verification**: `xcode-select --print-path` shows active Xcode
- **Switch Xcode**: `sudo xcode-select -s /Applications/Xcode_15.4.app/Contents/Developer`

#### Compiler Warnings Treated as Errors

**Cause**: ShowMIDI builds with `-Werror` (warnings as errors) to maintain code quality.

**Error Example**:
```
error: unused variable 'midiMessage' [-Werror,-Wunused-variable]
```

**Solution**:
1. Fix the warning in your code (remove unused variables, add casts, etc.)
2. **Do NOT disable** `-Werror` - it's a project requirement
3. Use `clang-format` to catch style issues before CI

#### Missing System Dependencies (Linux)

**Cause**: Required libraries (ALSA, X11, Freetype) not installed on runner.

**Error Message**:
```
Could NOT find ALSA (missing: ALSA_LIBRARY ALSA_INCLUDE_DIR)
```

**Solution**:
- **CI fix**: Workflow installs dependencies via `apt-get install`
- **Local fix**: Follow [Linux setup instructions](#linux) to install build dependencies

#### CLAP Plugin Build Warnings

**Cause**: `clap-juce-extensions` submodule not initialized (CLAP is optional).

**Expected Behavior**: Build continues successfully, emits warning:
```
CMake Warning: CLAP extensions not found at libs/clap-juce-extensions.
CLAP plugin format will be skipped.
  To enable CLAP: git submodule update --init --recursive
```

**Solution**:
- This is **not a failure** - CLAP is optional
- To enable CLAP: `git submodule update --init libs/clap-juce-extensions`

### Interpreting CI Results

When you open a PR, GitHub Actions runs 4 jobs in parallel:

#### Job Timeline and Expected Duration

| Job | Platform | Duration | Purpose |
|-----|----------|----------|---------|
| **code-quality** | Ubuntu | 2-5 min | Check GPL headers, build with warnings-as-errors |
| **build-and-test-macos** | macOS 13+ | 10-15 min | Build Standalone, VST3, AU, run smoke tests |
| **build-windows** | Windows Server 2022 | 15-20 min | Build Standalone, VST3 with MSVC 2022 |
| **build-linux** | Ubuntu 22.04 | 5-10 min | Build all formats via CMake, fast parallel build |

**Total PR validation time**: ~15-20 minutes (jobs run in parallel)

#### Reading the GitHub Actions UI

**Green checkmark (✓)**: Job passed - no action needed

**Red X (✗)**: Job failed - click to see error logs:
1. Click the red X next to the job name
2. Expand the failing step (e.g., "Build with Xcode")
3. Look for `error:` or `Error:` lines
4. Match error to [Common Failure Scenarios](#common-ci-failure-scenarios) above

**Yellow dot (●)**: Job in progress - wait for completion

**Gray circle (○)**: Job queued - GitHub Actions runners are busy, will start soon

#### Artifact Downloads

After successful builds, CI uploads artifacts you can download for testing:

- **ShowMIDI-macOS**: `.app` bundle, `.vst3`, `.component` (AU)
- **ShowMIDI-Windows**: `.exe`, `.vst3`
- **ShowMIDI-Linux**: Standalone binary, `.vst3`, `.so` files

**How to Download**:
1. Open your PR on GitHub
2. Click "Checks" tab
3. Select a successful workflow run
4. Scroll to "Artifacts" section at bottom
5. Click artifact name to download ZIP

**Retention**: Artifacts are kept for 90 days, then automatically deleted.

### Troubleshooting CI Failures

**Step 1: Identify the failing job**
- Look for red X in PR "Checks" tab
- Note which platform failed (code-quality, macOS, Windows, Linux)

**Step 2: Read the error message**
- Click the failing job name
- Expand the failing step
- Copy the error message (starting with `error:` or `Error:`)

**Step 3: Match to common scenarios**
- Compare error to [Common Failure Scenarios](#common-ci-failure-scenarios)
- Follow the solution steps for that scenario

**Step 4: Reproduce locally**
- Use [Local Testing](#local-testing-before-pushing) commands for your platform
- Fix the issue until local build succeeds
- Push updated code to trigger new CI run

**Step 5: Ask for help if stuck**
- If error doesn't match common scenarios, post in [GitHub Discussions](https://github.com/gbevin/ShowMIDI/discussions)
- Include: full error message, platform, PR link
- Maintainers typically respond within 24 hours

---

## Pull Request Process

### Before Submitting

Ensure your PR meets these requirements:

- [ ] Code builds successfully on at least one platform
- [ ] No JUCE leak detector warnings in Debug build
- [ ] All new source files include GPL-3.0 headers
- [ ] Commit messages follow Conventional Commits format
- [ ] Code follows JUCE patterns and uses `sm::scaled()` for UI dimensions
- [ ] Changes are documented in commit messages

### PR Template

When you open a PR, GitHub will auto-populate a template. Please fill out all sections:

- **Description**: What does this PR do and why?
- **Type of Change**: Bug fix, feature, breaking change, docs
- **Testing Checklist**: Which platforms you've tested on
- **Code Quality**: Confirm headers, commits, patterns, DPI scaling
- **Related Issues**: Link to issues this PR addresses
- **Constitution Compliance**: For architecture changes, link to relevant constitution section

### CI Validation

All PRs targeting `develop` or `main` trigger CI validation:

1. **Code Quality**: GPL headers, warnings as errors
2. **Build macOS**: Xcode builds, smoke tests
3. **Build Windows**: Visual Studio 2022 builds
4. **Build Linux**: CMake builds

PRs can only be merged when all CI checks pass (green checkmarks).

### Constitution Compliance

For changes that affect architecture, performance, or multi-platform support, review the [Constitution](.specify/memory/constitution.md) and confirm compliance:

- **Multi-Platform Architecture**: Changes work on macOS, Windows, Linux, iOS
- **Real-Time Performance**: No blocking operations on MIDI thread, <10ms latency
- **User-Centric Design**: Glanceable, auto-discovery, visual clarity
- **Maintainability**: Clean code, GPL-3.0 headers, documentation

---

## Community

### Getting Help

- **Forum**: https://forum.uwyn.com
- **Discord**: https://discord.gg/TgsCnwqWTf
- **GitHub Issues**: https://github.com/gbevin/ShowMIDI/issues

### Reporting Issues

When reporting bugs, please include:

1. **Platform**: macOS/Windows/Linux version
2. **ShowMIDI Version**: From "About" dialog or release tag
3. **MIDI Device**: Make/model of MIDI controller or interface
4. **Steps to Reproduce**: Detailed steps to trigger the issue
5. **Expected vs Actual Behavior**: What should happen vs what does happen
6. **Logs**: Any console output or error messages

### Code of Conduct

ShowMIDI follows the [Contributor Covenant Code of Conduct](https://www.contributor-covenant.org/version/2/1/code_of_conduct/). Please be respectful and inclusive in all interactions.

### Recognition

Contributors are recognized in:

- Git commit history
- Release notes (for significant contributions)
- Project website acknowledgments

---

## Additional Resources

- [GitFlow Workflow](.specify/memory/gitflow-workflow.md) - Detailed branching strategy
- [Constitution](.specify/memory/constitution.md) - Core project principles
- [CHANGELOG.md](CHANGELOG.md) - Version history
- [GitHub Actions Workflows](.github/workflows/README.md) - CI/CD documentation
- [JUCE Documentation](https://juce.com/learn/documentation) - JUCE framework reference

---

Thank you for contributing to ShowMIDI! Your efforts help make MIDI visualization better for everyone. 🎹✨
