# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

### Changed

### Fixed

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
