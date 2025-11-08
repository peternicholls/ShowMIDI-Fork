# .clang-format Contract

## Format Specification

**Tool**: [clang-format](https://clang.llvm.org/docs/ClangFormat.html)  
**Version**: 14+ recommended (supports all options used)  
**Purpose**: Enforce consistent C++ code style aligned with JUCE framework

---

## Template

```yaml
# ShowMIDI clang-format configuration
# Based on LLVM style with JUCE framework adaptations

BasedOnStyle: LLVM
Language: Cpp

# Brace wrapping - Allman style (JUCE standard)
BreakBeforeBraces: Allman

# Indentation
IndentWidth: 4
TabWidth: 4
UseTab: Never
NamespaceIndentation: None
IndentCaseLabels: false

# Line length
ColumnLimit: 120

# Pointers and references - left-aligned (Type* ptr)
PointerAlignment: Left
ReferenceAlignment: Left

# Spacing
SpaceAfterCStyleCast: false
SpaceBeforeParens: ControlStatements
SpacesInParentheses: false
SpacesInSquareBrackets: false
SpaceBeforeAssignmentOperators: true

# Alignment
AlignConsecutiveAssignments: false
AlignConsecutiveMacros: true
AlignConsecutiveDeclarations: false
AlignTrailingComments: true

# Breaking
AllowShortFunctionsOnASingleLine: None
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false
AllowShortBlocksOnASingleLine: Never
BreakBeforeBinaryOperators: None
BreakBeforeTernaryOperators: true

# Comments
ReflowComments: true
CommentPragmas: '^ IWYU pragma:'

# Includes
SortIncludes: CaseSensitive
IncludeBlocks: Preserve

# Macros
MacroBlockBegin: ''
MacroBlockEnd: ''

# Penalties (fine-tuning line breaking decisions)
PenaltyBreakBeforeFirstCallParameter: 19
PenaltyBreakComment: 300
PenaltyBreakString: 1000
PenaltyExcessCharacter: 1000000
PenaltyReturnTypeOnItsOwnLine: 60

# Other
AccessModifierOffset: -4
AlwaysBreakAfterReturnType: None
AlwaysBreakTemplateDeclarations: Yes
BinPackArguments: true
BinPackParameters: true
CompactNamespaces: false
ConstructorInitializerIndentWidth: 4
ContinuationIndentWidth: 4
FixNamespaceComments: true
```

---

## Style Rationale

### Brace Style: Allman

**Why**: JUCE framework uses Allman style (braces on new line)

```cpp
// ✅ Correct (Allman - JUCE style)
void myFunction()
{
    if (condition)
    {
        doSomething();
    }
}

// ❌ Incorrect (Attached braces - not JUCE)
void myFunction() {
    if (condition) {
        doSomething();
    }
}
```

### Indentation: 4 Spaces

**Why**: JUCE codebase standard, matches EditorConfig

```cpp
// ✅ Correct (4-space indent)
class MyComponent : public juce::Component
{
public:
    MyComponent()
    {
        setSize(sm::scaled(800), sm::scaled(600));
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyComponent)
};
```

### Pointer Alignment: Left

**Why**: `Type* ptr` aligns with JUCE convention

```cpp
// ✅ Correct (pointer with type)
juce::String* textPtr = nullptr;
void processArray(int* data, size_t length);

// ❌ Incorrect (pointer with variable)
juce::String *textPtr = nullptr;
```

### Column Limit: 120

**Why**: Balance readability vs horizontal space on modern displays

```cpp
// Acceptable with 120-character limit
auto result = calculateComplexValue(parameter1, parameter2, parameter3, parameter4);

// Too long - would break to multiple lines
auto result = calculateComplexValueWithVeryLongName(veryLongParameter1, veryLongParameter2, veryLongParameter3, veryLongParameter4, veryLongParameter5);
```

---

## Validation Rules

1. **JUCE Compatibility**: Output MUST compile without warnings
2. **Minimal Diff**: Running on existing codebase SHOULD change < 5% of lines
3. **Namespace Preservation**: SHOULD NOT add/remove `using namespace`
4. **Comment Preservation**: MUST NOT break documentation comments
5. **Macro Handling**: MUST preserve JUCE macros (`JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR`)

---

## Usage Workflow

### Command Line

```bash
# Format single file
clang-format -i Source/MyComponent.cpp

# Format all C++ files in Source/
find Source -name '*.cpp' -o -name '*.h' | xargs clang-format -i

# Check formatting without modifying (CI validation)
clang-format --dry-run --Werror Source/**/*.{cpp,h}
```

### IDE Integration

| IDE | Integration Method |
|-----|-------------------|
| VS Code | C/C++ extension (auto-format on save) |
| CLion | Built-in (Settings → Editor → Code Style) |
| Xcode | clang-format-Xcode plugin |
| Visual Studio | Built-in (Tools → Options → Text Editor → C/C++ → Formatting) |

### Git Hook (Optional - Future Enhancement)

```bash
# .git/hooks/pre-commit
#!/bin/bash
# Format staged C++ files before commit
git diff --cached --name-only --diff-filter=ACM | \
  grep -E '\.(cpp|h|mm)$' | \
  xargs clang-format -i
```

---

## Testing Against Existing Code

### Validation Process

1. **Backup**: Create branch for testing
   ```bash
   git checkout -b test-clang-format
   ```

2. **Format Source Directory**:
   ```bash
   find Source -name '*.cpp' -o -name '*.h' | xargs clang-format -i
   ```

3. **Review Diff**:
   ```bash
   git diff --stat Source/
   ```

4. **Success Criteria**: < 5% of lines changed (per SC-004 in spec)

5. **Adjustments**: If > 5%, tweak settings:
   - `ColumnLimit`: Increase to 140 if many line breaks
   - `AllowShortFunctionsOnASingleLine`: Change to `Inline` if getter/setters break
   - `AlignConsecutiveMacros`: Disable if JUCE macros misalign

6. **Rollback**: If incompatible, document exceptions and make JUCE-specific adjustments

---

## Exception Handling

### Disable Formatting for Specific Code

```cpp
// clang-format off
const int specialTable[] = {
    1,    2,    4,    8,
    16,   32,   64,   128,
    256,  512,  1024, 2048
};
// clang-format on
```

### File-Level Disable (Not Recommended)

```cpp
// At top of file (use sparingly)
// clang-format off
```

---

## Integration with CI (Future)

Add to `.github/workflows/ci.yml`:

```yaml
- name: Check C++ formatting
  run: |
    find Source -name '*.cpp' -o -name '*.h' | \
      xargs clang-format --dry-run --Werror
```

**Status**: Not in current CI - add as future enhancement per Out of Scope section in spec.
