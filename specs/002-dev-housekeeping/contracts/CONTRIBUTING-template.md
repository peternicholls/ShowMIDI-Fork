# CONTRIBUTING.md Template

## Structure Specification

**Purpose**: Guide contributors through ShowMIDI development workflow  
**Audience**: External contributors, new team members, AI agents

---

## Template

```markdown
# Contributing to ShowMIDI

Thank you for your interest in contributing to ShowMIDI! This guide will help you get started.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Community](#community)

---

## Code of Conduct

ShowMIDI follows the [JUCE Code of Conduct](https://github.com/juce-framework/JUCE/blob/master/CODE_OF_CONDUCT.md). By participating, you agree to uphold this code. Report unacceptable behavior to [INSERT_EMAIL].

---

## Getting Started

### Prerequisites

- **C++ Compiler**: C++17 or later
  - macOS: Xcode 13+
  - Windows: Visual Studio 2022
  - Linux: GCC 9+ or Clang 10+
- **JUCE Framework**: Included as submodule
- **CMake**: 3.15+ (optional, for Linux/macOS builds)
- **Git**: 2.30+

### Clone the Repository

```bash
# Clone with JUCE submodule
git clone --recursive https://github.com/peternicholls/ShowMIDI-Fork.git
cd ShowMIDI-Fork

# If already cloned without --recursive
git submodule update --init --recursive
```

### Build

#### macOS (Xcode)

```bash
cd Builds/MacOSX
xcodebuild -project ShowMIDI.xcodeproj -scheme ShowMIDI clean build
```

#### Windows (Visual Studio 2022)

```batch
cd Builds\VisualStudio2022
MSBuild.exe ShowMIDI.sln /p:Configuration=Release
```

#### Linux (CMake)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## Development Workflow

ShowMIDI follows [Atlassian GitFlow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow). See `.specify/memory/gitflow-workflow.md` for complete details.

### Quick Workflow

1. **Sync with upstream** (if using fork):
   ```bash
   git fetch upstream
   git checkout develop
   git merge upstream/develop
   ```

2. **Create feature branch**:
   ```bash
   # Using helper script (recommended)
   .specify/scripts/bash/create-feature.sh 003 "midi-export"
   
   # Or manually
   git checkout develop
   git checkout -b feature/003-midi-export
   ```

3. **Make changes**:
   ```bash
   # Edit files
   git add Source/MyNewFile.cpp
   git commit -m "feat(midi): add MIDI export functionality"
   ```

4. **Push and create PR**:
   ```bash
   git push origin feature/003-midi-export
   # Create PR on GitHub targeting `develop` branch
   ```

5. **Wait for CI**: All 4 checks must pass before merge

---

## Coding Standards

### C++ Style

- **Standard**: C++17 (JUCE framework requirement)
- **Braces**: Allman style (opening brace on new line)
- **Indentation**: 4 spaces (no tabs)
- **Line length**: 120 characters max
- **Pointers**: `Type* ptr` (left-aligned)

### File Headers

All C++ source files MUST include GPL-3.0 header:

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

### JUCE Patterns

```cpp
// Component declaration
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

### DPI Scaling

Use `sm::scaled()` for all pixel values:

```cpp
button.setBounds(sm::scaled(10), sm::scaled(20), 
                 sm::scaled(100), sm::scaled(30));
```

### Formatting Tools

- **EditorConfig**: `.editorconfig` (automatic on save)
- **clang-format**: `.clang-format` (manual, see below)

```bash
# Format specific file
clang-format -i Source/MyComponent.cpp

# Format all C++ files
find Source -name '*.cpp' -o -name '*.h' | xargs clang-format -i
```

---

## Commit Guidelines

Follow [Conventional Commits 1.0.0](https://www.conventionalcommits.org/):

### Format

```
<type>(<scope>): <subject>

[optional body]

[optional footer]
```

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only
- `style`: Formatting (no code change)
- `refactor`: Code restructuring
- `perf`: Performance improvement
- `test`: Add/update tests
- `build`: Build system (CMake, Projucer)
- `ci`: CI/CD changes
- `chore`: Maintenance (deps, versions)

### Scopes (ShowMIDI-specific)

- `midi`: MIDI handling
- `ui`: User interface
- `theme`: Theme system
- `plugin`: Plugin formats (VST, AU, etc.)
- `settings`: Settings/preferences
- `juce`: JUCE integration
- `build`: Build configuration
- `release`: Release management

### Examples

```bash
feat(midi): add MPE manager visualization
fix(ui): correct DPI scaling on 4K displays
docs(readme): update Linux build instructions
chore(release): bump version to 1.1.0
```

---

## Pull Request Process

### Before Submitting

1. ✅ All code compiles without warnings
2. ✅ GPL-3.0 headers on new files
3. ✅ Manual testing completed
4. ✅ Conventional commit messages
5. ✅ No merge conflicts with `develop`

### PR Template

See `.github/pull_request_template.md` for required checklist.

### CI Requirements

All 4 GitHub Actions jobs MUST pass:

| Job | Checks |
|-----|--------|
| `code-quality` | GPL headers, warnings as errors |
| `build-and-test-macos` | Xcode builds, smoke tests |
| `build-windows` | Visual Studio 2022 builds |
| `build-linux` | CMake builds |

### Review Process

1. **Automated CI**: 4 jobs run (5-10 minutes)
2. **Maintainer Review**: Code quality, architecture, JUCE compliance
3. **Approval**: At least 1 maintainer approval required
4. **Merge**: Squash and merge to `develop` (preserves linear history)

---

## Testing

### Manual Testing

1. **Build all plugin formats**:
   - Standalone
   - VST3
   - AU (macOS)
   - AAX (if available)

2. **Smoke tests**:
   - Launch plugin in DAW
   - Connect MIDI device
   - Verify MIDI events display correctly
   - Test theme switching
   - Verify settings persistence

3. **Platform-specific**:
   - macOS: Test on Retina and non-Retina displays
   - Windows: Test on 100%, 150%, 200% DPI scaling
   - Linux: Test on ALSA and JACK

### Automated Testing (Future)

Unit tests will be added in future releases (see roadmap).

---

## Community

### Communication Channels

- **GitHub Issues**: Bug reports, feature requests
- **GitHub Discussions**: Q&A, ideas, show-and-tell
- **Email**: [INSERT_EMAIL] (security issues only)

### Getting Help

- **Build issues**: Check README.md platform-specific instructions
- **Git workflow**: See `.specify/memory/gitflow-workflow.md`
- **JUCE questions**: [JUCE Forum](https://forum.juce.com/)

### Recognition

Contributors will be recognized in:
- CHANGELOG.md (with GitHub username)
- About dialog (for significant contributions)

---

## License

By contributing, you agree that your contributions will be licensed under GPL-3.0. See [COPYING.md](COPYING.md).
```

---

## Section Breakdown

| Section | Purpose | Required Content |
|---------|---------|------------------|
| Code of Conduct | Set community standards | Link to existing CoC or define basic rules |
| Getting Started | Onboard new contributors | Prerequisites, clone, build commands |
| Development Workflow | Define contribution process | GitFlow summary, branch naming, helper scripts |
| Coding Standards | Ensure consistency | C++ style, JUCE patterns, file headers, DPI scaling |
| Commit Guidelines | Standardize history | Conventional Commits, types, scopes, examples |
| Pull Request Process | Define quality gates | Pre-submission checklist, CI requirements, review process |
| Testing | Validate changes | Manual smoke tests, platform-specific checks |
| Community | Foster engagement | Communication channels, help resources, recognition |

---

## Validation Rules

1. **Completeness**: ALL 8 sections present
2. **Working Commands**: All bash/batch commands tested
3. **Link Validity**: All links resolve (internal files exist, external URLs valid)
4. **Consistency**: Aligns with `.specify/memory/gitflow-workflow.md`
5. **Accessibility**: Clear for non-expert contributors (explain jargon)
6. **Actionable**: Each section has concrete steps, not just theory

---

## Customization Notes

Replace placeholders before committing:

- `[INSERT_EMAIL]`: Maintainer contact email
- Build commands: Verify paths match actual project structure
- CI job names: Update if `.github/workflows/ci.yml` changes
- Community links: Add Discord/Slack if created

---

## Relationship to Other Documents

| Document | Relationship |
|----------|--------------|
| `.specify/memory/gitflow-workflow.md` | CONTRIBUTING is **summary**, gitflow-workflow is **detailed reference** |
| `.github/copilot-instructions.md` | CONTRIBUTING for **humans**, copilot-instructions for **AI agents** |
| `CHANGELOG.md` | CONTRIBUTING explains **how to write** changelog entries |
| `.editorconfig`, `.clang-format` | CONTRIBUTING references these for **formatting** |
| `README.md` | README for **users**, CONTRIBUTING for **developers** |
