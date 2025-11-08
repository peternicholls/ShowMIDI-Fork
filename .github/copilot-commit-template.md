# Commit Message Template

Follow [Conventional Commits](https://www.conventionalcommits.org/) for all commit messages in this repository.

## Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

## Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style (formatting, semicolons, etc.)
- `refactor`: Code restructuring without behavior change
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `chore`: Maintenance tasks (deps, config, etc.)
- `ci`: CI/CD changes

## Scope

The scope should indicate the area of the codebase affected:

- `ui`: User interface components
- `midi`: MIDI processing and device handling
- `theme`: Theme and color management
- `settings`: Settings and configuration
- `layout`: Layout and DPI scaling
- `build`: Build system and configuration
- `deps`: Dependencies

## Subject

- Use imperative mood ("add" not "added" or "adds")
- Don't capitalize first letter
- No period at the end
- Keep under 50 characters

## Body

- Explain the what and why (not how)
- Wrap at 72 characters
- Separate from subject with blank line
- Use bullet points for multiple items

## Footer

- Reference issues: `Fixes #123`, `Closes #456`
- Breaking changes: `BREAKING CHANGE: description`

## Examples

### Feature
```
feat(layout): add auto-sized button layout with text-driven widths

Implement Option 1 button sizing strategy that automatically
calculates button width based on text content. This provides:
- Consistent spacing across all DPI settings
- Font-aware button sizing
- Proper gap control between buttons

Fixes #42
```

### Bug Fix
```
fix(settings): correct width calculation for collapsed sidebar

Settings and About boxes now properly size to MIDI viewport
width regardless of sidebar state.
```

### Refactor
```
refactor(constants): consolidate layout constants into LayoutConstants.h

Move all scattered layout values into centralized header with
organized sections for different UI areas.
```

### Documentation
```
docs(readme): update DPI scaling implementation status

Add progress tracking for DPI scaling tasks T001-T069.
```

### Style
```
style(code): apply consistent namespace usage for layout constants

Replace Theme:: references with sm::layout:: throughout codebase.
```

### Performance
```
perf(rendering): optimize button touch area calculations

Cache DPI scale factor to avoid repeated calculations in
setBoundsForTouch.
```

### Test
```
test(layout): add button spacing validation tests

Verify auto-sized buttons maintain correct gaps at different
DPI scales.
```

### Chore
```
chore(deps): update JUCE to version 7.0.9

Update submodule to latest stable release for bug fixes.
```

### CI/CD
```
ci(workflow): add macOS build validation

Add automated build checks for Debug and Release configurations.
```

## Multi-line Example

```
feat(keyboard): add Cmd-W shortcut to close window

Implement standard macOS window close behavior with Command-W
keyboard shortcut. This enhancement:

- Calls systemRequestedQuit() to properly clean up
- Uses isCommandDown() modifier check for macOS
- Integrates with existing keyPressed handler
- Provides familiar user experience on macOS

Closes #89
```

## Breaking Changes

```
feat(api): redesign settings storage format

BREAKING CHANGE: Settings file format changed from XML to JSON.
Users will need to reconfigure their settings on first launch
after update. Old settings will be backed up to settings.xml.bak
```
