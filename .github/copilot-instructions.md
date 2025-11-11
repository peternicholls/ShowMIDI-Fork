# ShowMIDI.git Development Guidelines

Auto-generated from all feature plans. Last updated: 2025-11-08

## Active Technologies
- Markdown, YAML, EditorConfig, clang-format configuration (no programming language - documentation only) (002-dev-housekeeping)
- Text files in repository root and .github/ directory (version controlled via git) (002-dev-housekeeping)
- YAML (GitHub Actions workflow syntax), CMake 3.15+, Shell scripting (Bash/PowerShell) + GitHub Actions runners (ubuntu-latest, macos-latest, windows-latest), JUCE 7.0.5, CMake, Xcode (macOS), Visual Studio 2022 (Windows), system libraries (ALSA, X11, Freetype on Linux) (003-ci-build-fix)
- N/A (CI/CD configuration only) (003-ci-build-fix)

- C++17 (JUCE framework requirements)
- JUCE framework (audio plugin framework)
- CMake (build system)
- Projucer (.jucer project files)

## Project Structure

```text
Source/           # C++ source files (.h, .cpp)
JuceLibraryCode/  # JUCE-generated code
Builds/           # Platform-specific build files
  MacOSX/         # Xcode projects
  VisualStudio2022/  # Visual Studio solutions
  LinuxMakefile/  # Linux makefiles
  iOS/            # iOS Xcode projects
CMakeLists.txt    # CMake configuration
showmidi.jucer    # Projucer project file
.specify/         # Feature specifications and planning
  memory/         # Constitution and workflow docs
  templates/      # Spec, plan, task templates
  scripts/        # Automation scripts
```

## GitFlow Workflow

ShowMIDI follows **Atlassian GitFlow** branching strategy. See `.specify/memory/gitflow-workflow.md` for complete workflow.

### Branch Structure

- **`main`**: Production releases only (tagged: v1.0.0, v1.0.1, etc.)
- **`develop`**: Integration branch for features
- **`feature/*`**: Feature branches (e.g., `feature/001-dpi-scaling`, `feature/002-midi-export`)
- **`release/*`**: Release stabilization (e.g., `release/1.1.0`)
- **`hotfix/*`**: Critical production fixes (e.g., `hotfix/1.0.2`)

### Branching Rules

✅ **DO**:
- Create feature branches from `develop`
- Merge features to `develop` via Pull Request
- Create release/hotfix branches when preparing releases
- Tag all releases on `main` with semantic versions
- Merge hotfixes to both `main` AND `develop`

❌ **DON'T**:
- Commit directly to `main` or `develop`
- Merge features directly to `main`
- Create feature branches from `main`
- Push without pulling latest `develop` first

### Commit Convention

Use [Conventional Commits](https://www.conventionalcommits.org/):

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

**Scopes** (ShowMIDI-specific):
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

## Code Style

### C++17 JUCE Patterns

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

**Namespace**:
- Use `showmidi` namespace for project code
- Prefix JUCE types: `juce::String`, `juce::Array`, etc.

**File Headers**:
All source files MUST include GPL-3.0 header:
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

**DPI Scaling**:
Use `sm::scaled()` utility for all dimensional values:
```cpp
button.setBounds(sm::scaled(10), sm::scaled(20), 
                 sm::scaled(100), sm::scaled(30));
```

### Memory Management

- Use `std::unique_ptr` or `juce::OwnedArray` for ownership
- Avoid raw `new`/`delete`
- Use JUCE leak detector macro on all components
- No allocations on MIDI thread

## CI/CD Integration

GitHub Actions pipeline (`.github/workflows/ci.yml`) validates code automatically.

### CI Triggers

| Event | Branches | Jobs |
|-------|----------|------|
| Pull Request | `develop`, `main` | All 4 jobs |
| Push | `main`, `release/**`, `hotfix/**` | All 4 jobs |
| Push | `develop`, `feature/**` | None (validated at PR) |

### Pipeline Jobs

1. **code-quality**: GPL headers, warnings as errors
2. **build-and-test-macos**: Xcode builds, smoke tests
3. **build-windows**: Visual Studio 2022 builds
4. **build-linux**: CMake builds

**PR Requirements**:
- ✅ All 4 CI jobs pass (green checks)
- ✅ No merge conflicts
- ✅ Conventional commit messages
- ✅ GPL-3.0 headers on new files

## Version Management

Versions MUST be synchronized in TWO locations:

1. **CMakeLists.txt** (line ~14):
   ```cmake
   project(ShowMIDI VERSION 1.0.1)
   ```

2. **showmidi.jucer** (Projucer settings):
   ```xml
   <VERSION value="1.0.1"/>
   ```

**Semantic Versioning 2.0.0** ([semver.org](https://semver.org/spec/v2.0.0.html)):
- `MAJOR.MINOR.PATCH`
- MAJOR: Breaking changes
- MINOR: New features
- PATCH: Bug fixes only

## Constitution Compliance

See `.specify/memory/constitution.md` for core principles:

1. **Multi-Platform Architecture**: Support macOS, Windows, Linux, iOS
2. **JUCE Framework Standards**: Use JUCE idioms consistently
3. **Real-Time Performance**: <10ms latency, no UI blocking
4. **User-Centric Design**: Glanceable, auto-discovery, visual clarity
5. **Maintainability**: GPL-3.0 headers, clean code, docs

## Commands

### Local Development

**macOS Build**:
```bash
cd Builds/MacOSX
xcodebuild -project ShowMIDI.xcodeproj -scheme ShowMIDI clean build
```

**Linux Build**:
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

**Create Feature Branch**:
```bash
.specify/scripts/bash/create-feature.sh 002 "midi-export"
```

**Create Release**:
```bash
git checkout develop
git pull origin develop
git checkout -b release/1.1.0
# Update versions in CMakeLists.txt and .jucer
git commit -m "chore(release): bump version to 1.1.0"
git push origin release/1.1.0
```

## Recent Changes
- 003-ci-build-fix: Added YAML (GitHub Actions workflow syntax), CMake 3.15+, Shell scripting (Bash/PowerShell) + GitHub Actions runners (ubuntu-latest, macos-latest, windows-latest), JUCE 7.0.5, CMake, Xcode (macOS), Visual Studio 2022 (Windows), system libraries (ALSA, X11, Freetype on Linux)
- 002-dev-housekeeping: Completed development housekeeping infrastructure
  - Added configuration files (.editorconfig, .clang-format, updated .gitignore)
  - Added documentation (CHANGELOG.md, CONTRIBUTING.md, DOWNLOAD_STATS.md, PR template)
  - Migrated CI/CD workflows from .github/prompts/workflows/ to .github/workflows/
  - Migrated scripts to scripts/ directory with typo corrections
  - Technologies: Markdown, YAML, EditorConfig, clang-format, Bash scripts

- 001-dpi-scaling: Added C++17 (JUCE framework requirements)

<!-- MANUAL ADDITIONS START -->
<!-- MANUAL ADDITIONS END -->
