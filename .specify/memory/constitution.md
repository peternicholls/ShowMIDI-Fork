<!--
SYNC IMPACT REPORT
==================
Version Change: 0.0.0 → 1.0.0
Rationale: Initial constitution establishment for ShowMIDI project

Sections Added:
- Multi-Platform Architecture
- JUCE Framework Standards
- Real-Time Performance
- User-Centric Design
- Maintainability
- Platform Standards & Requirements
- Development Workflow

Templates Status:
- ✅ plan-template.md - reviewed and aligned
- ✅ spec-template.md - reviewed and aligned
- ✅ tasks-template.md - reviewed and aligned

Follow-up Items: None - all placeholders filled
-->

# ShowMIDI Constitution

## Core Principles

### I. Multi-Platform Architecture

ShowMIDI MUST support all declared target platforms with feature parity:
- **Desktop**: macOS (11.7+), Windows (10+), Linux (mainstream distributions)
- **Mobile**: iOS (12+) as plugin (AUv3)
- **Plugin Formats**: VST, VST3, AU, AUv3, LV2, CLAP, Standalone

**DPI-Aware Design**: All UI components MUST scale appropriately across different screen densities and resolutions. Use the `sm::scaled()` utility pattern for all dimensional values to ensure consistent appearance across platforms.

**Build System**: Support both JUCE Projucer (.jucer) and CMake build configurations. CMake MUST be maintained as an alternative build path, especially for CLAP plugin format.

**Rationale**: ShowMIDI's purpose is to provide real-time MIDI visualization wherever musicians work. Platform fragmentation undermines this mission. DPI awareness ensures usability on high-resolution displays and varied form factors.

### II. JUCE Framework Standards

All code MUST adhere to JUCE framework idioms and best practices:
- Use JUCE types (String, Array, OwnedArray, etc.) over STL equivalents for consistency
- Implement Pimpl pattern for complex components to minimize header dependencies
- Declare components with `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` macro
- Use JUCE's platform abstraction layers; avoid direct platform-specific APIs unless necessary
- Organize source with `.h` (interface) and `.cpp` (implementation) separation
- Maintain namespace `showmidi` for all project code

**C++ Standard**: C++17 as defined in project configuration (CMakeLists.txt, Projucer settings)

**Rationale**: Consistency with JUCE patterns ensures maintainability, leverages framework strengths, and prevents memory leaks through leak detection mechanisms.

### III. Real-Time Performance

MIDI message handling and visualization MUST meet real-time performance requirements:
- Message processing latency < 10ms (preferably < 5ms)
- UI updates MUST NOT block MIDI thread
- Channel auto-hiding based on inactivity MUST be smooth and glitch-free
- Support for high-throughput MIDI streams (e.g., MPE controllers, dense CC automation)

**Memory Management**: Avoid allocations on the MIDI thread. Pre-allocate data structures where possible.

**Thread Safety**: Maintain strict separation between audio/MIDI thread and UI thread using JUCE's message thread mechanisms.

**Rationale**: Musicians rely on ShowMIDI for real-time feedback during performance and recording. Latency or dropped messages compromise the tool's core value proposition.

### IV. User-Centric Design

Features MUST prioritize immediate value and ease of use:
- **Glanceable Interface**: Users should understand current MIDI activity at a glance without reading logs
- **Auto-Discovery**: MIDI devices appear automatically; minimal configuration required
- **Visual Clarity**: Use of color, typography (JetBrains Mono font), and spatial organization to communicate information hierarchy
- **Keyboard Shortcuts**: Essential functions accessible via keyboard (spacebar = pause/resume, delete/backspace = reset, V = toggle visualization)
- **Theme Support**: Users can customize appearance via SVG-based themes

**Progressive Disclosure**: Hide complexity until needed (e.g., device sidebar can be collapsed)

**Rationale**: ShowMIDI fills a void by making MIDI activity comprehensible without expertise. Complex or opaque UI defeats this purpose.

### V. Maintainability

Code MUST be maintainable for the long term:
- **File Headers**: All source files MUST include GPL-3.0 license header with copyright notice (© 2023 Uwyn LLC)
- **Component Documentation**: Complex components (e.g., MidiDeviceComponent) MUST include docstring explaining purpose and responsibilities
- **State Management**: Use SettingsManager pattern for persistent preferences; PropertiesSettings and PluginSettings for context-specific state
- **Consistent Naming**: Follow JUCE conventions (camelCase for methods, PascalCase for types)
- **Build Hygiene**: Avoid hardcoded paths; use environment variables (e.g., PATH_TO_JUCE) in CMake configuration

**Code Review**: Changes affecting core MIDI handling or multi-platform compatibility require careful review.

**Rationale**: Open-source GPL-3.0 project depends on community maintainability. Clear code and licensing reduce barriers to contribution.

## Platform Standards & Requirements

### Supported Platforms & Deployment Targets

**macOS**:
- Deployment Target: 11.7 (as specified in CMakeLists.txt)
- Architectures: x86_64, Arm64
- Plugin Formats: VST, VST3, AU, AUv3, CLAP, Standalone
- Sandboxing: Enabled for App Sandbox
- Hardened Runtime: Enabled

**Windows**:
- Deployment Target: Windows 10+
- Architectures: x86_64 (primary), x86, Arm64 (target)
- Plugin Formats: VST, VST3, Standalone, CLAP
- Runtime Library: MultiThreaded (static runtime as per CMakeLists.txt)

**Linux**:
- Distributions: Mainstream (Ubuntu, Fedora, Debian, Arch, etc.)
- Architectures: x86_64 (primary), Arm64/aarch64
- Plugin Formats: VST, VST3, LV2, CLAP, Standalone
- Dependencies: ALSA, FreeType2, X11, Xrandr, Xinerama, Xcursor

**iOS**:
- Deployment Target: 12.0+
- Architectures: Arm64, x86_64 (simulator)
- Plugin Format: AUv3 (Audio Unit v3)
- Note: iOS support is plugin-only (no standalone app)

### Build Configuration

**Projucer Settings**:
- Project Type: Audio Plugin
- Version Format: MAJOR.MINOR.PATCH (currently 1.0.1)
- Bundle ID: com.uwyn.showmidi
- Manufacturer: Uwyn (code: Uwyn)
- Plugin Characteristics: MIDI Effect, Produces MIDI Out, Wants MIDI In

**CMake Requirements**:
- Minimum Version: 3.15
- Custom Options: CLAP support requires manual build process (see CMakeLists.txt)
- JUCE Path: Configurable via PATH_TO_JUCE environment variable

**VST2 SDK**: Required for VST2 builds but not distributed (user must provide in `libs/vst2` due to licensing)

## Development Workflow

### Feature Development Process

1. **Specification**: Define user scenarios and acceptance criteria using `.specify/templates/spec-template.md`
2. **Planning**: Document technical approach and architecture using `.specify/templates/plan-template.md`
3. **Implementation**: Follow task breakdown from `.specify/templates/tasks-template.md`
4. **Testing**: Manual testing across platforms (automated tests currently limited to dependency libraries)
5. **Documentation**: Update README.md for user-facing features; inline docs for code

### Branching Strategy

- `main`: Stable release branch (currently upstream: gbevin/ShowMIDI)
- `feature/*`: Feature development branches (e.g., `feature/DPI-Scaling`)
- Version tags: Match release versions (e.g., v1.0.1)

### Quality Gates

**Pre-Commit**:
- Code compiles on macOS and Linux (minimum)
- No JUCE leak detector warnings
- License headers present on new files

**Pre-Release**:
- Test on all supported platforms and plugin formats
- Verify theme loading and customization
- Confirm keyboard shortcuts function correctly
- Check memory usage under high MIDI throughput

### Issue Management

- Community Forum: https://forum.uwyn.com
- Discord: https://discord.gg/TgsCnwqWTf
- GitHub Issues: For bug reports and feature requests

## Governance

This constitution defines the architectural and quality standards for ShowMIDI development. All contributions, whether from maintainers or community members, MUST adhere to these principles.

**Amendment Process**:
- Amendments require documentation of rationale and impact
- Version number MUST be incremented according to semantic versioning:
  - **MAJOR**: Incompatible changes to architecture principles or removal of supported platforms
  - **MINOR**: Addition of new principles, platform targets, or major guidance expansion
  - **PATCH**: Clarifications, corrections, or non-substantive improvements
- All templates and related documentation MUST be updated to reflect amendments

**Compliance Verification**:
- Feature specifications MUST include Constitution Check section referencing relevant principles
- Pull requests affecting multi-platform support, real-time performance, or JUCE patterns require explicit constitution compliance review
- Complexity that appears to violate principles (e.g., platform-specific workarounds) MUST be justified with "Why Needed" and "Alternatives Rejected" documentation

**Conflict Resolution**:
- When principles conflict (e.g., real-time performance vs. user-centric feature), preference order is: Real-Time Performance → Multi-Platform Architecture → User-Centric Design → Maintainability
- Justification for trade-offs MUST be documented in implementation plan

**Version**: 1.0.0 | **Ratified**: 2025-11-07 | **Last Amended**: 2025-11-07
