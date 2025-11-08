# .editorconfig Contract

## Format Specification

**Standard**: [EditorConfig](https://editorconfig.org/)  
**Purpose**: Define coding styles for multiple editors and IDEs

---

## Template

```ini
# EditorConfig is awesome: https://EditorConfig.org

# top-most EditorConfig file
root = true

# All files - base defaults
[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
trim_trailing_whitespace = true

# C++ source files (.cpp, .h, .mm for Objective-C++)
[*.{cpp,h,mm}]
indent_style = space
indent_size = 4

# Markdown and text files
[*.{md,txt}]
indent_style = space
indent_size = 2
trim_trailing_whitespace = false  # Markdown uses trailing spaces for line breaks

# JSON and YAML
[*.{json,yml,yaml}]
indent_style = space
indent_size = 2

# XML files (including .jucer Projucer files)
[*.{xml,jucer}]
indent_style = space
indent_size = 2

# Makefiles - MUST use tabs
[Makefile]
indent_style = tab

# Shell scripts
[*.{sh,bash,zsh}]
indent_style = space
indent_size = 2
end_of_line = lf

# CMake files
[{CMakeLists.txt,*.cmake}]
indent_style = space
indent_size = 2
```

---

## Property Definitions

### Global Properties

| Property | Valid Values | ShowMIDI Choice | Rationale |
|----------|--------------|-----------------|-----------|
| `charset` | `utf-8`, `utf-8-bom`, `latin1`, `utf-16be`, `utf-16le` | `utf-8` | Universal, handles copyright symbols |
| `end_of_line` | `lf`, `cr`, `crlf` | `lf` | Git default, works cross-platform |
| `insert_final_newline` | `true`, `false` | `true` | POSIX standard, required by some compilers |
| `trim_trailing_whitespace` | `true`, `false` | `true` | Prevents noisy diffs |

### Indentation Properties

| Property | Valid Values | C++ Files | Markdown | JSON/YAML |
|----------|--------------|-----------|----------|-----------|
| `indent_style` | `space`, `tab` | `space` | `space` | `space` |
| `indent_size` | number | `4` | `2` | `2` |

---

## Editor Support

| Editor | Support | Notes |
|--------|---------|-------|
| VS Code | ✅ Native | Automatic with EditorConfig extension |
| CLion | ✅ Native | Built-in support since 2018.2 |
| Xcode | ⚠️ Plugin | Requires EditorConfig-Xcode plugin |
| Vim | ✅ Plugin | editorconfig-vim plugin |
| Emacs | ✅ Plugin | editorconfig-emacs package |
| Sublime Text | ✅ Package | EditorConfig package |
| Visual Studio 2022 | ✅ Native | Built-in since VS2017 |

---

## Validation Rules

1. **root = true**: MUST be present in top-most .editorconfig
2. **Section order**: Specific patterns should come after general patterns
3. **Glob patterns**: Use `*`, `**`, `?`, `[...]` for matching
4. **Property names**: Case-insensitive, but use lowercase by convention
5. **Boolean values**: Use lowercase `true` or `false`
6. **Indentation**: Numbers only (no "tab" for indent_size)
7. **Conflicts**: More specific patterns override general patterns

---

## Testing

### Validation Checklist

- [ ] File saved with UTF-8 encoding (check in hex editor if needed)
- [ ] Line endings are LF (not CRLF or CR)
- [ ] C++ files use 4-space indentation
- [ ] No trailing whitespace (except Markdown where intentional)
- [ ] File ends with newline character
- [ ] Makefiles use tabs (not spaces)

### Editor Test

1. Open existing C++ file in supported editor
2. Verify indentation matches EditorConfig (4 spaces)
3. Add trailing whitespace and save
4. Verify whitespace is automatically removed
5. Check git diff shows no formatting changes

---

## Integration with .clang-format

EditorConfig provides **basic** formatting (indentation, line endings).  
clang-format provides **advanced** C++ formatting (braces, spacing, alignment).

**Relationship**: EditorConfig applies first (on file open/save), clang-format applies on explicit format command.

**No conflicts**: EditorConfig's `indent_size=4` matches clang-format's `IndentWidth: 4`.
