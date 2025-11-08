# Pull Request

## Description

<!-- Provide a clear and concise description of your changes -->

Fixes #

## Type of Change

<!-- Mark the relevant option(s) with an [x] -->

- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update
- [ ] Refactoring (no functional changes)
- [ ] Performance improvement
- [ ] Build/CI change

## Testing Checklist

<!-- Mark completed items with an [x] -->

### Build Verification

- [ ] macOS (Xcode)
- [ ] Windows (Visual Studio 2022)
- [ ] Linux (CMake)
- [ ] iOS (Xcode)

### Manual Testing

- [ ] Tested as standalone application
- [ ] Tested as VST3 plugin
- [ ] Tested as AU plugin (macOS only)
- [ ] Tested with real MIDI device
- [ ] Tested with light theme
- [ ] Tested with dark theme

### Quality Checks

- [ ] No JUCE leak detector warnings in Debug build
- [ ] No console errors or warnings
- [ ] Tested on HiDPI/Retina displays (if UI changes)
- [ ] Verified cross-platform behavior (if multi-platform changes)

## Code Quality

<!-- Mark completed items with an [x] -->

- [ ] All new source files include GPL-3.0 headers
- [ ] Commit messages follow Conventional Commits format
- [ ] Code follows JUCE patterns (namespaced types, leak detector macros)
- [ ] UI dimensions use `sm::scaled()` for DPI scaling
- [ ] No blocking operations on MIDI thread
- [ ] Code is documented (comments for complex logic)

## Related Issues

<!-- Link related issues or discussions -->

- Related to #
- Closes #
- Depends on #

## Constitution Compliance

<!-- For changes affecting architecture, performance, or multi-platform support -->
<!-- Review: .specify/memory/constitution.md -->

<!-- Mark completed items with an [x] if applicable -->

- [ ] Multi-Platform Architecture: Changes tested on macOS, Windows, Linux, iOS (where applicable)
- [ ] JUCE Framework Standards: Code uses JUCE idioms and patterns consistently
- [ ] Real-Time Performance: No blocking operations, <10ms latency maintained
- [ ] User-Centric Design: Changes maintain glanceable, clear, auto-discovery UX
- [ ] Maintainability: Code is clean, documented, follows project conventions

**Architecture Impact** (if applicable):
<!-- Describe any changes to core architecture, data flow, or performance characteristics -->

## Additional Notes

<!-- Any additional context, screenshots, or information that would help reviewers -->

---

**Reviewer Checklist** (for maintainers):

- [ ] CI checks pass (code-quality, build-macos, build-windows, build-linux)
- [ ] No merge conflicts
- [ ] Conventional commit messages validated
- [ ] GPL headers verified on new files
- [ ] CHANGELOG.md updated (for features/fixes)
- [ ] Version bumped if needed (CMakeLists.txt + showmidi.jucer)
