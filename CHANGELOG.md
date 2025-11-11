# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

### Changed

### Fixed

## [1.1.3] - 2025-11-11

### Added
- **CI/CD Monitoring**: Automated Phase 5 production monitoring workflow
  - Weekly automated metrics collection (runs every Monday at 00:00 UTC)
  - Success rate tracking (target: 100% for clean code)
  - Infrastructure failure rate tracking (target: <5%)
  - Auto-generated weekly reports (PHASE5_WEEK{1-4}_REPORT.md)
  - GitHub issue creation for metrics below targets
  - Automatic task tracking updates

### Changed
- **CI/CD Performance**: Implemented fail-fast pattern in both ci.yml and test-build.yml
  - Code quality checks now gate all build jobs (prevents wasted runner minutes)
  - Faster feedback: Quality failures detected in ~2min (vs ~6min waiting for builds)
  - No failure emails when code quality fails (builds don't run)
- **Workflow Governance**: Created workflow alignment strategy and validation tools
  - Added `.github/WORKFLOW_ALIGNMENT_STRATEGY.md` (comprehensive governance document)
  - Added `.github/scripts/validate-workflow-alignment.sh` (automated validation)
  - Synchronized timeout-minutes across workflows (macOS: 30, Windows: 25, Linux: 20)

### Fixed
- **Testing Infrastructure**: Completed comprehensive Phase 4 edge case testing
  - Validated missing JUCE error handling (FATAL_ERROR with clear instructions)
  - Validated missing CLAP graceful degradation (WARNING, build continues)
  - Validated mixed doc+code changes trigger full CI
  - Validated concurrent PRs handled independently
  - Validated timeout protection configured with healthy margins (74-85%)

## [1.1.2] - 2025-11-11

### Fixed
- **CI/CD Infrastructure**: Complete rebuild of GitHub Actions workflows
  - Fixed macOS builds by migrating from Xcode projects to CMake (eliminates hardcoded absolute paths)
  - Pinned macOS builds to macos-14 runner (JUCE 7.0.5 incompatible with macOS 15 API deprecations)
  - Fixed Windows builds (VST3 + Standalone) - excluded VST2 (proprietary SDK requirement)
  - Fixed Linux builds with proper system library detection (ALSA, X11, Freetype)
  - Applied `-Werror` (warnings as errors) only to ShowMIDI sources, not JUCE modules
  - Fixed signed/unsigned comparison warnings in MidiDeviceComponent.cpp
- **Build System**: Enhanced CMakeLists.txt with complete JUCE plugin target definitions
  - Added all source files, binary data (fonts, SVG icons, themes)
  - Configured conditional -Werror via SHOWMIDI_WERROR environment variable
  - Universal binary support for macOS (arm64 + x86_64)
- **Workflow Optimization**:
  - Added paths-ignore filter to skip CI on documentation-only changes
  - Implemented concurrency control (cancel-in-progress) to save CI resources
  - Configured appropriate timeout limits per platform (macOS: 30min, Windows: 25min, Linux: 20min)
  - Upgraded artifact upload to actions/upload-artifact@v4 with 90-day retention
  - Aligned test-build.yml workflow with production CI patterns

### Changed
- **Build Times**: Dramatically improved CI build performance
  - macOS: ~2.5 minutes (previously broken)
  - Windows: ~3.5 minutes (previously broken)
  - Linux: ~3.8 minutes (previously broken)
  - Total workflow time: ~6 minutes
- **CI/CD Architecture**: Migrated from project files to CMake for consistent cross-platform builds
- **Development Workflow**: Added comprehensive Phase 1 baseline testing (8/8 tests passing)
  - Validated all success criteria (SC-001 through SC-011)
  - Full constitution compliance verified

### Added
- **Documentation**: Comprehensive CI/CD testing and validation documentation
  - REMOTE_TESTING_TASKS.md with complete test results
  - CODE_REVIEW.md with workflow analysis
  - Updated CONTRIBUTING.md with CI/CD integration guide
- **Quality Assurance**: Automated GPL-3.0 header validation in CI
- **Artifact Management**: Build artifacts for all platforms (macOS, Windows, Linux) with 90-day retention

## [1.0.1] - 2024-07-10

### Fixed
- Fixed HRCC LSB history handling
- Improved NRPN/RPN detection with support for MSB-only data entry values
- NRPN and RPN values are now handled with similar logic as HRCC values
- Refactored HRCC value handling for better consistency
- Handle standard RPN numbers and provide meaningful output
- Cleanup and fix for RPN 6 MPE range display

### Added
- Added call to action about MIDI Innovation Awards

## [1.0.0] - 2024-07-07

### Added
- Real-time MIDI visualization with automatic device discovery
- Support for multiple MIDI devices displayed in columns
- Comprehensive MIDI message display including:
  - Note On/Off with velocity
  - Control Change (CC) and Hi-Res 14-bit CC (HRCC)
  - Pitch Bend, Program Change, Channel Pressure, Polyphonic Key Pressure
  - RPN (Registered Parameter Number) and NRPN (Non-Registered Parameter Number)
  - MPE Manager visualization and configuration (MPE MGR, MPE LZ, MPE UZ)
  - Timing Clock as BPM with Start/Continue/Stop
  - System Exclusive (SYSEX) messages
- Multi-platform plugin support:
  - Standalone application (macOS, Windows, Linux, iOS)
  - VST3 plugin format
  - Audio Unit (AU) plugin format
  - AUv3 plugin format (iOS)
  - AAX plugin format
  - LV2 plugin format (Linux)
- Theme system with customizable color schemes
  - Included themes: Dark, Light, Classic Light, Darcula, Disco, BStation, Mouha
  - Theme editor for creating and saving custom themes
- Visualization modes:
  - Horizontal lines view
  - Scrolling graphs view
  - Toggle between modes in real-time (keyboard: `v` key)
- User interface features:
  - Expandable left panel for device visibility controls
  - Pause/unpause MIDI stream (keyboard: `spacebar`)
  - Clear displayed data (keyboard: `delete`/`backspace`)
  - Automatic channel ordering and auto-hide on inactivity
  - DPI scaling support for high-resolution displays
- Cross-platform build system with CMake and Projucer
- GPL-3.0 licensed open-source project

[Unreleased]: https://github.com/gbevin/ShowMIDI/compare/1.0.1...HEAD
[1.0.1]: https://github.com/gbevin/ShowMIDI/compare/1.0.0...1.0.1
[1.0.0]: https://github.com/gbevin/ShowMIDI/releases/tag/1.0.0
