# Dev Housekeeping Implementation Quick Start

**Feature**: 002-dev-housekeeping  
**Goal**: Add essential development documentation to ShowMIDI repository

---

## 🎯 Quick Overview

Add 6 files to improve developer experience:

1. `CHANGELOG.md` - Track version history
2. `CONTRIBUTING.md` - Contributor guide
3. `.editorconfig` - Cross-editor formatting
4. `.clang-format` - C++ code style
5. `.gitignore` (update) - Ignore build/OS artifacts
6. `.github/pull_request_template.md` - PR quality checklist

---

## 📋 Prerequisites

- ✅ On `002-dev-housekeeping` branch
- ✅ Read `spec.md` (full requirements)
- ✅ Read `plan.md` (implementation strategy)

---

## 🚀 Implementation Steps

### Phase 1: Documentation Files (P1)

**Estimated time**: 30 minutes

#### Step 1.1: Create CHANGELOG.md

```bash
# Copy template from contracts/
cp specs/002-dev-housekeeping/contracts/CHANGELOG-template.md CHANGELOG.md

# Customize for ShowMIDI
# - Replace [X.Y.Z] with current version from CMakeLists.txt (1.0.1)
# - Add initial entry for v1.0.1 release
# - Update version links to peternicholls/ShowMIDI-Fork repo
```

**Validation**:
```bash
# Check format
grep -E '^\[Unreleased\]' CHANGELOG.md  # Should exist
grep -E '^\[1\.0\.1\]' CHANGELOG.md     # Should exist
grep 'https://github.com/peternicholls' CHANGELOG.md  # Should use Fork repo
```

#### Step 1.2: Create CONTRIBUTING.md

```bash
# Copy template
cp specs/002-dev-housekeeping/contracts/CONTRIBUTING-template.md CONTRIBUTING.md

# Customize
# - Replace [INSERT_EMAIL] with maintainer email
# - Verify all build commands work
# - Test all internal links (e.g., COPYING.md exists)
```

**Validation**:
```bash
# Check links
markdown-link-check CONTRIBUTING.md  # Or manually verify

# Test build commands (spot check one platform)
cd Builds/MacOSX
xcodebuild -project ShowMIDI.xcodeproj -scheme ShowMIDI clean build
cd ../..
```

---

### Phase 2: Formatting Configuration (P2)

**Estimated time**: 45 minutes

#### Step 2.1: Create .editorconfig

```bash
# Copy from contract
cat > .editorconfig << 'EOF'
# EditorConfig is awesome: https://EditorConfig.org

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

[*.{xml,jucer}]
indent_style = space
indent_size = 2

[Makefile]
indent_style = tab

[*.{sh,bash,zsh}]
indent_style = space
indent_size = 2
end_of_line = lf

[{CMakeLists.txt,*.cmake}]
indent_style = space
indent_size = 2
EOF
```

**Validation**:
```bash
# Install EditorConfig plugin in your editor
# Open Source/PluginEditor.cpp
# Verify: Indent is 4 spaces, no tabs, file ends with newline
```

#### Step 2.2: Create .clang-format

```bash
# Copy YAML config from contract
cat > .clang-format << 'EOF'
BasedOnStyle: LLVM
Language: Cpp
BreakBeforeBraces: Allman
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 120
PointerAlignment: Left
ReferenceAlignment: Left
# ... (see contracts/clang-format-contract.md for full config)
EOF
```

**Test on existing code**:
```bash
# Format one file to check compatibility
cp Source/PluginEditor.cpp Source/PluginEditor.cpp.backup
clang-format -i Source/PluginEditor.cpp

# Check diff percentage
git diff --stat Source/PluginEditor.cpp
# Should change < 5% of lines (per SC-004)

# Restore backup if > 5%
mv Source/PluginEditor.cpp.backup Source/PluginEditor.cpp
```

**If > 5% changes**: Adjust `.clang-format` settings (see contract for tuning guide)

---

### Phase 3: Repository Hygiene (P3)

**Estimated time**: 20 minutes

#### Step 3.1: Update .gitignore

```bash
# .gitignore already exists - append new patterns
cat >> .gitignore << 'EOF'

### macOS ###
.DS_Store
.AppleDouble
.LSOverride

### Windows ###
Thumbs.db
Desktop.ini

### Linux ###
*~
.directory

### IDEs ###
.vscode/
.idea/
*.iml
.vs/

### CMake ###
build/
cmake-build-*/
CMakeCache.txt
EOF
```

**Validation**:
```bash
# Check patterns work
touch .DS_Store
git status  # Should NOT show .DS_Store

mkdir build
git status  # Should NOT show build/

rm .DS_Store
rmdir build
```

#### Step 3.2: Create PR Template

```bash
# Create .github directory if missing
mkdir -p .github

# Copy template
cp specs/002-dev-housekeeping/contracts/pr-template-contract.md \
   .github/pull_request_template.md
```

**Validation**:
```bash
# Push and create test PR to verify template populates
git checkout -b test-pr-template-002
echo "test" > test-pr.txt
git add test-pr.txt
git commit -m "test: verify PR template"
git push origin test-pr-template-002

# Open PR on GitHub → verify template appears
# Close PR without merging
git checkout 002-dev-housekeeping
git branch -D test-pr-template-002
```

---

## ✅ Validation Checklist

### File Existence

- [ ] `CHANGELOG.md` exists at repository root
- [ ] `CONTRIBUTING.md` exists at repository root
- [ ] `.editorconfig` exists at repository root
- [ ] `.clang-format` exists at repository root
- [ ] `.gitignore` updated (contains macOS, Windows, Linux patterns)
- [ ] `.github/pull_request_template.md` exists

### Content Verification

- [ ] CHANGELOG has `[Unreleased]` and `[1.0.1]` sections
- [ ] CONTRIBUTING has working build commands
- [ ] CONTRIBUTING links resolve (COPYING.md, gitflow-workflow.md exist)
- [ ] .editorconfig has C++ indent_size = 4
- [ ] .clang-format has BreakBeforeBraces: Allman
- [ ] .gitignore includes `.DS_Store`, `build/`, `.vscode/`
- [ ] PR template has all 8 sections (Description, Testing, Code Quality, etc.)

### Testing

- [ ] EditorConfig: Open C++ file, verify 4-space indent auto-applies
- [ ] clang-format: Format sample file, check < 5% lines changed
- [ ] gitignore: Create `.DS_Store`, verify `git status` ignores it
- [ ] PR template: Create test PR, verify template populates

### Constitution Compliance

- [ ] ✅ Multi-Platform: All files work on macOS, Windows, Linux
- [ ] ✅ JUCE Standards: clang-format uses Allman braces (JUCE style)
- [ ] ✅ Real-Time Performance: N/A (documentation-only)
- [ ] ✅ User-Centric: CONTRIBUTING improves contributor experience
- [ ] ✅ Maintainability: All files reduce friction, improve consistency

---

## 🔧 Common Issues

### Issue 1: clang-format changes > 5% of lines

**Symptom**: `git diff --stat` shows many lines changed after formatting

**Fix**:
1. Open `.clang-format`
2. Increase `ColumnLimit: 140` (if lots of line breaks)
3. Change `AllowShortFunctionsOnASingleLine: Inline` (if getters/setters break)
4. Re-test on sample file

### Issue 2: EditorConfig not applying in IDE

**Symptom**: File opens with wrong indentation

**Fix**:
1. Install EditorConfig plugin (VS Code: `EditorConfig.EditorConfig`)
2. Reload window (Cmd+R / Ctrl+R)
3. Verify `.editorconfig` in workspace root (not subdirectory)

### Issue 3: CONTRIBUTING links broken

**Symptom**: Clicking links in CONTRIBUTING.md shows 404

**Fix**:
1. Check file exists: `ls -l COPYING.md .specify/memory/gitflow-workflow.md`
2. Update relative paths if files moved
3. Use `markdown-link-check CONTRIBUTING.md` to validate all links

---

## 🛠️ EditorConfig Usage

### What is EditorConfig?

EditorConfig helps maintain consistent coding styles across different editors and IDEs. The `.editorconfig` file automatically configures your editor to use ShowMIDI's formatting rules.

### Supported Editors

**VS Code**:
- Install extension: `EditorConfig.EditorConfig`
- Reload window (Cmd+Shift+P → "Reload Window")
- Open any C++ file → should auto-apply 4-space indent, LF endings

**CLion/IntelliJ IDEA**:
- Built-in support (no plugin needed)
- Settings → Editor → Code Style → Enable EditorConfig support

**Vim/Neovim**:
- Install plugin: `editorconfig/editorconfig-vim`
- Add to `.vimrc`: `Plugin 'editorconfig/editorconfig-vim'`

**Sublime Text**:
- Install via Package Control: `EditorConfig`

**Emacs**:
- Install package: `editorconfig-emacs`

### Verifying EditorConfig Works

1. Open `Source/PluginProcessor.cpp` in your editor
2. Press Tab → should insert 4 spaces (not a tab character)
3. Save file → should add final newline and trim trailing whitespace
4. Check `git diff` → should show no formatting changes

If formatting still changes, verify:
- EditorConfig plugin is installed and enabled
- `.editorconfig` file is in repository root (not subdirectory)
- Your editor supports EditorConfig (check https://editorconfig.org)

---

## 🎨 clang-format Usage

### What is clang-format?

clang-format automatically formats C++ code to match ShowMIDI's JUCE style guidelines (Allman braces, 4-space indent, 120 column limit).

### Installation

**macOS**:
```bash
brew install clang-format
```

**Linux (Ubuntu/Debian)**:
```bash
sudo apt install clang-format
```

**Linux (Fedora/RHEL)**:
```bash
sudo dnf install clang
```

**Windows**:
1. Download from [LLVM releases](https://releases.llvm.org/)
2. Install and add to PATH

### Basic Usage

**Format a single file** (modifies in-place):
```bash
clang-format -i Source/PluginProcessor.cpp
```

**Format all C++ files** in Source/:
```bash
find Source/ -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

**Check formatting without modifying** (dry-run):
```bash
clang-format --dry-run -Werror Source/PluginProcessor.cpp
```

**View formatting changes** (without modifying):
```bash
clang-format Source/PluginProcessor.cpp | diff -u Source/PluginProcessor.cpp -
```

### IDE Integration

**VS Code**:
1. Install "C/C++" extension (ms-vscode.cpptools)
2. Settings → Search "format on save"
3. Enable "Editor: Format On Save"
4. C++ files will auto-format when you save

**CLion**:
1. Settings → Editor → Code Style → C/C++
2. Click "Set from..." → Predefined Style → ClangFormat
3. Enable "Enable ClangFormat support"
4. Use Cmd+Option+L (macOS) or Ctrl+Alt+L (Windows/Linux) to format

**Xcode**:
1. Install ClangFormat-Xcode plugin: https://github.com/travisjeffery/ClangFormat-Xcode
2. Restart Xcode
3. Edit → Clang Format → Format File

### When to Use clang-format

**Before submitting a PR**:
```bash
# Format all files you modified
git diff --name-only develop | grep -E '\.(cpp|h)$' | xargs clang-format -i
```

**When cleaning up legacy code**:
```bash
# Format entire directory (use with caution on large codebases)
find Source/ -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

**Validation (CI/CD)**:
```bash
# Check if any files need formatting (exit code 1 if changes needed)
find Source/ -name "*.cpp" -o -name "*.h" -exec clang-format --dry-run -Werror {} \;
```

### clang-format Configuration

ShowMIDI's `.clang-format` file uses these key settings:

- **BasedOnStyle**: LLVM (clean, readable baseline)
- **BreakBeforeBraces**: Allman (JUCE standard - braces on new line)
- **IndentWidth**: 4 spaces
- **ColumnLimit**: 120 characters
- **PointerAlignment**: Left (`Type* ptr` not `Type *ptr`)

If formatting produces unexpected results, review `.clang-format` and adjust settings. See [clang-format documentation](https://clang.llvm.org/docs/ClangFormatStyleOptions.html) for all options.

---

## 📝 Commit Strategy

### Option A: Single Commit (Recommended for Small Team)

```bash
git add CHANGELOG.md CONTRIBUTING.md .editorconfig .clang-format .gitignore .github/
git commit -m "docs(release): add dev housekeeping documentation

- Add CHANGELOG.md (Keep a Changelog format)
- Add CONTRIBUTING.md (GitFlow workflow guide)
- Add .editorconfig (cross-editor formatting)
- Add .clang-format (C++ style enforcement)
- Update .gitignore (macOS/Windows/Linux/IDE patterns)
- Add PR template (quality checklist)

Closes #002"
```

### Option B: Atomic Commits (Recommended for Large Team)

```bash
# Commit 1: CHANGELOG
git add CHANGELOG.md
git commit -m "docs(release): add CHANGELOG.md

Implements Keep a Changelog format with Semantic Versioning 2.0.0.
Includes initial entry for v1.0.1.

Part of #002"

# Commit 2: CONTRIBUTING
git add CONTRIBUTING.md
git commit -m "docs(workflow): add CONTRIBUTING.md

Comprehensive guide covering GitFlow, build instructions,
coding standards, and PR process.

Part of #002"

# Commit 3: Formatting configs
git add .editorconfig .clang-format
git commit -m "style(build): add .editorconfig and .clang-format

- .editorconfig: UTF-8, LF, 4-space indent for C++
- .clang-format: LLVM base with Allman braces (JUCE style)

Part of #002"

# Commit 4: Repository hygiene
git add .gitignore .github/pull_request_template.md
git commit -m "chore(build): update .gitignore and add PR template

- .gitignore: Add macOS, Windows, Linux, IDE patterns
- PR template: Add quality checklist with constitution compliance

Closes #002"
```

---

## 🚢 Merge to Develop

### Pre-Merge Checklist

- [ ] All 6 files created/updated
- [ ] All validation checks passed (see Validation Checklist above)
- [ ] Manual testing complete (EditorConfig, clang-format, gitignore)
- [ ] No merge conflicts with `develop`
- [ ] Conventional commit messages
- [ ] Pushed to origin

### Create PR

```bash
# Push feature branch
git push origin 002-dev-housekeeping

# Create PR on GitHub
# - Title: "feat(docs): add dev housekeeping documentation"
# - Body: Use PR template (this will be first test of template!)
# - Target: develop
# - Reference: Closes spec in specs/002-dev-housekeeping/spec.md
```

### CI Validation

All 4 CI jobs will run:

1. ✅ `code-quality`: GPL headers (N/A for .md/.yaml/.ini files)
2. ✅ `build-and-test-macos`: Xcode builds (no code changes)
3. ✅ `build-windows`: Visual Studio 2022 builds (no code changes)
4. ✅ `build-linux`: CMake builds (no code changes)

**Expected**: All pass (no C++ code modified)

### Post-Merge

```bash
# After PR merged to develop
git checkout develop
git pull origin develop

# Clean up feature branch
git branch -d 002-dev-housekeeping
```

---

## 📚 References

- **Specification**: `specs/002-dev-housekeeping/spec.md`
- **Implementation Plan**: `specs/002-dev-housekeeping/plan.md`
- **Research Findings**: `specs/002-dev-housekeeping/research.md`
- **Data Model**: `specs/002-dev-housekeeping/data-model.md`
- **Contracts**: `specs/002-dev-housekeeping/contracts/`

---

## 🕒 Time Estimates

| Phase | Tasks | Estimated Time |
|-------|-------|----------------|
| Phase 1 | CHANGELOG + CONTRIBUTING | 30 min |
| Phase 2 | .editorconfig + .clang-format | 45 min |
| Phase 3 | .gitignore + PR template | 20 min |
| **Validation** | All checks | 15 min |
| **Commit & PR** | Git operations | 10 min |
| **Total** | | **2 hours** |

*Actual time may vary based on clang-format tuning and testing thoroughness.*

---

## 💡 Tips

- **Start with Phase 1**: Get documentation in place first (highest priority)
- **Test formatting on branches**: Don't format entire codebase until config validated
- **Use templates as reference**: Contracts show example content, customize for ShowMIDI
- **Validate incrementally**: Check each file after creation, don't wait until end
- **Ask for help**: If stuck, refer to research.md for decision rationale
