# CHANGELOG.md Template

## Format Specification

**Standard**: [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)  
**Versioning**: [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html)

---

## Template

```markdown
# Changelog

All notable changes to ShowMIDI will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- New features committed to `develop` but not yet released

### Changed
- Changes to existing functionality

### Fixed
- Bug fixes

## [X.Y.Z] - YYYY-MM-DD

### Added
- Feature description (#issue)

### Changed
- Change description (#issue)

### Deprecated
- Soon-to-be-removed feature warning

### Removed
- Removed feature notice

### Fixed
- Bug fix description (#issue)

### Security
- Security fix description (#issue)

---

## Change Categories

- **Added**: New features
- **Changed**: Changes in existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Vulnerabilities fixed

## Version Linking

```markdown
[Unreleased]: https://github.com/peternicholls/ShowMIDI-Fork/compare/v1.0.1...HEAD
[1.0.1]: https://github.com/peternicholls/ShowMIDI-Fork/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/peternicholls/ShowMIDI-Fork/releases/tag/v1.0.0
```

## Example Entry

```markdown
## [1.0.1] - 2025-11-01

### Fixed
- Resolved crash when disconnecting MIDI device during playback (#42)
- Corrected DPI scaling on 4K displays at 150% and 200% zoom levels (#45)

### Changed
- Improved theme loading performance by 30% through SVG caching
- Updated JetBrains Mono font to version 2.304

### Security
- Updated JUCE framework to address CVE-2025-XXXX
```

## Validation Rules

1. **Version Format**: MUST be `[X.Y.Z]` (semantic versioning)
2. **Date Format**: MUST be `YYYY-MM-DD` (ISO 8601)
3. **Categories**: MUST be one of the six standard categories
4. **Order**: Versions in reverse chronological order (newest first)
5. **Links**: Each version SHOULD link to GitHub compare or release
6. **Issue References**: Use `(#<number>)` format for auto-linking
7. **Consistency**: Use past tense ("Added" not "Add")
