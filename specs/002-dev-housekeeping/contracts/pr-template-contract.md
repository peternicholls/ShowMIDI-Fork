# Pull Request Template Contract

## Format Specification

**Location**: `.github/pull_request_template.md`  
**Purpose**: Standardize PR descriptions and ensure quality gates  
**Trigger**: Auto-populates when creating PR on GitHub

---

## Template

```markdown
## Description

<!-- Provide a clear, concise summary of the changes -->

**Type**: <!-- feat | fix | docs | style | refactor | perf | test | build | ci | chore -->
**Scope**: <!-- midi | ui | theme | plugin | settings | juce | build | release -->

**Related Issue**: <!-- Closes #XXX or N/A -->

### Changes Made

<!-- List key changes (3-5 bullet points) -->
- 
- 
- 

### Motivation

<!-- Why is this change needed? What problem does it solve? -->

---

## Testing

### Build Verification

- [ ] **macOS**: Xcode build successful (Standalone + VST3 + AU)
- [ ] **Windows**: Visual Studio 2022 build successful (Standalone + VST3)
- [ ] **Linux**: CMake build successful (Standalone + VST3)
- [ ] **iOS**: Xcode build successful (if applicable)

### Functional Testing

- [ ] **Standalone**: Launches without crashes
- [ ] **VST3**: Loads in DAW (specify: _____________)
- [ ] **AU** (macOS): Loads in Logic Pro / GarageBand
- [ ] **MIDI Events**: Displays correctly with test device
- [ ] **Settings**: Persist across app restarts
- [ ] **Theme Switching**: No visual artifacts

### Platform-Specific Checks

- [ ] **macOS Retina**: UI scales correctly at 2x
- [ ] **Windows DPI**: Tested at 150% and/or 200% scaling
- [ ] **Linux**: ALSA or JACK MIDI working

---

## Code Quality

### Style Compliance

- [ ] **GPL-3.0 Headers**: All new `.cpp`/`.h` files include license header
- [ ] **Conventional Commits**: Commit messages follow `<type>(<scope>): <subject>` format
- [ ] **Code Formatting**: Ran `clang-format -i` on modified C++ files (or auto-formatted via EditorConfig)
- [ ] **No Warnings**: Compiles without warnings (`-Werror` compatible)
- [ ] **DPI Scaling**: All pixel values use `sm::scaled()` wrapper

### JUCE Compliance

- [ ] **Leak Detector**: Components use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR`
- [ ] **Namespace**: JUCE types prefixed (`juce::String`, `juce::Component`)
- [ ] **Memory Management**: Uses `std::unique_ptr` or `juce::OwnedArray` (no raw `new`/`delete`)
- [ ] **Real-Time Safe**: No allocations on MIDI thread

---

## Constitution Compliance

Verify changes align with ShowMIDI constitution (`.specify/memory/constitution.md`):

- [ ] **Multi-Platform**: Works on macOS, Windows, Linux (tested or N/A for platform-specific feature)
- [ ] **JUCE Standards**: Uses JUCE idioms, no deprecated APIs
- [ ] **Real-Time Performance**: <10ms latency, no UI blocking
- [ ] **User-Centric**: Improves usability, discoverability, or visual clarity
- [ ] **Maintainability**: Code is documented, no magic numbers, readable

---

## Documentation

- [ ] **README.md**: Updated if build/install steps changed
- [ ] **CHANGELOG.md**: Entry added under `[Unreleased]` (for `feat`/`fix`/`breaking change`)
- [ ] **Code Comments**: Complex logic has explanatory comments
- [ ] **Specification**: Created/updated spec in `specs/` (if new feature, see `.specify/templates/spec.md`)

---

## Screenshots / Videos

<!-- For UI changes, include before/after screenshots or demo videos -->
<!-- Example: Drag and drop images here or use ![alt](url) -->

**Before**:


**After**:


---

## Checklist

- [ ] I have read [CONTRIBUTING.md](../CONTRIBUTING.md)
- [ ] My code follows the ShowMIDI coding standards
- [ ] I have tested my changes on at least one platform
- [ ] All CI checks pass (or failures explained below)
- [ ] This PR targets the `develop` branch (not `main`)

---

## Additional Notes

<!-- Any context reviewers should know? Known issues? Future work? -->
```

---

## Section Breakdown

| Section | Purpose | Required Fields |
|---------|---------|-----------------|
| **Description** | Summarize PR intent | Type, scope, related issue, changes, motivation |
| **Testing** | Prove functionality | Build verification (3 platforms), functional tests (6 checks), platform-specific (3 checks) |
| **Code Quality** | Enforce standards | Style compliance (5 checks), JUCE compliance (4 checks) |
| **Constitution Compliance** | Align with principles | 5 constitutional checks (multi-platform, JUCE, performance, UX, maintainability) |
| **Documentation** | Keep docs current | README, CHANGELOG, code comments, specs |
| **Screenshots/Videos** | Visual validation | Before/after images for UI changes |
| **Checklist** | Final verification | 5 pre-merge confirmations |
| **Additional Notes** | Context for reviewers | Optional free-form text |

---

## Validation Rules

1. **Checkbox Completeness**: ALL applicable checkboxes checked (or marked N/A)
2. **Description Required**: Type, scope, and changes list MUST be filled
3. **Testing Evidence**: At least 1 platform build verified
4. **Constitution**: All 5 checks addressed (even if "N/A - no changes to X")
5. **CHANGELOG**: Required for `feat`, `fix`, breaking changes
6. **Branch Target**: Must target `develop` (not `main`)

---

## Automation Hooks (Future Enhancement)

### CI Integration

Add to `.github/workflows/ci.yml`:

```yaml
- name: Validate PR Template Completeness
  run: |
    # Check for unchecked boxes in required sections
    if grep -q "- \[ \]" .github/PULL_REQUEST_DESCRIPTION.md; then
      echo "Error: Incomplete PR checklist"
      exit 1
    fi
```

### Bot Comments

Use [PR Reminder Bot](https://github.com/apps/pr-reminder):

```yaml
# .github/pr-reminder.yml
messages:
  - when: checklist_incomplete
    say: "Please complete all applicable checklist items before requesting review."
  - when: missing_changelog
    say: "This PR contains a `feat` or `fix` but has no CHANGELOG.md entry."
```

**Status**: Future enhancement (not in scope for feature 002)

---

## Usage Example

### Contributor Workflow

1. **Create PR on GitHub** → Template auto-populates
2. **Fill Description**:
   ```markdown
   **Type**: feat
   **Scope**: midi
   **Related Issue**: Closes #23
   
   ### Changes Made
   - Added MIDI export to .mid file format
   - Implemented quantization options (1/4, 1/8, 1/16 note)
   - Added export dialog with tempo/time signature settings
   ```

3. **Complete Testing Checklist**:
   ```markdown
   - [x] **macOS**: Xcode build successful (Standalone + VST3 + AU)
   - [x] **Windows**: Visual Studio 2022 build successful (Standalone + VST3)
   - [ ] **Linux**: CMake build successful (not tested - no Linux machine)
   ```

4. **Verify Code Quality**:
   ```markdown
   - [x] **GPL-3.0 Headers**: All new .cpp/.h files include license header
   - [x] **Conventional Commits**: feat(midi): add MIDI export functionality
   - [x] **DPI Scaling**: Export dialog uses sm::scaled() for dimensions
   ```

5. **Constitution Check**:
   ```markdown
   - [x] **Multi-Platform**: Export logic is cross-platform (uses JUCE File API)
   - [x] **JUCE Standards**: Uses juce::FileChooser and juce::MidiFile
   - [x] **Real-Time Performance**: Export runs on background thread (doesn't block UI)
   - [x] **User-Centric**: Added keyboard shortcut (Cmd/Ctrl+E) for discoverability
   - [x] **Maintainability**: Documented quantization algorithm in comments
   ```

6. **Update CHANGELOG**:
   ```markdown
   - [x] **CHANGELOG.md**: Entry added under `[Unreleased]`
   ```
   ```markdown
   ## [Unreleased]
   
   ### Added
   - MIDI export to .mid file format with quantization options (#23)
   ```

7. **Submit** → CI runs → Reviewers see complete context

---

## Relationship to CI Pipeline

| CI Job | Template Section | Verification |
|--------|------------------|--------------|
| `code-quality` | Code Quality → GPL Headers | Automated check |
| `code-quality` | Code Quality → No Warnings | Compiled with `-Werror` |
| `build-and-test-macos` | Testing → macOS | Xcode build + smoke tests |
| `build-windows` | Testing → Windows | VS2022 build |
| `build-linux` | Testing → Linux | CMake build |

**Redundancy Intent**: Template is **self-reported**, CI is **automated verification**. Both must pass.

---

## Customization Notes

Before committing, verify:

- DAW names in "Functional Testing" match team test environment (e.g., Ableton Live, Reaper, Logic Pro)
- Platform-specific checks align with target platforms (remove iOS if not building for iOS)
- Constitution principles match `.specify/memory/constitution.md` (update if constitution changes)

---

## Testing the Template

1. **Create test PR**:
   ```bash
   git checkout -b test-pr-template
   echo "test" > test.txt
   git add test.txt
   git commit -m "test: verify PR template"
   git push origin test-pr-template
   ```

2. **Open PR on GitHub** → Verify template populates

3. **Check markdown rendering**: All checkboxes, links, code blocks render correctly

4. **Close PR** without merging
