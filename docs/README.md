# ShowMIDI Code Analysis Summary

**Analysis Date:** November 9, 2025  
**Repository:** [peternicholls/ShowMIDI-Fork](https://github.com/peternicholls/ShowMIDI-Fork)  
**Version:** 1.1.1

---

## Quick Reference

This directory contains comprehensive documentation of the ShowMIDI codebase:

1. **[source-code-analysis.md](source-code-analysis.md)** - Complete codebase analysis
   - Architecture overview
   - Statistics and metrics
   - Code quality assessment
   - Recommendations

2. **[architecture-diagrams.md](architecture-diagrams.md)** - Visual architecture diagrams
   - System architecture
   - Component interaction maps
   - MIDI message flow
   - State management

3. **[component-reference.md](component-reference.md)** - Component API reference
   - All 38 major components documented
   - Data structures
   - Key methods and responsibilities

---

## Executive Summary

### What is ShowMIDI?

ShowMIDI is a **cross-platform MIDI visualization application** that provides real-time, glanceable views of MIDI activity. It runs as both a **standalone application** and as **audio plugin** (VST3, AU, AAX, LV2, CLAP).

### Key Statistics

- **Total Code:** 7,277 lines
- **Language:** C++17
- **Framework:** JUCE
- **Files:** 48 (20 .cpp, 27 .h, 1 .mm)
- **Components:** 38 major components
- **Platforms:** macOS, Windows, Linux, iOS

### Architecture Highlights

```
┌─────────────────────────────────────────┐
│   95% Shared Code (Core UI)            │
│   ├── MainLayoutComponent              │
│   ├── MidiDeviceComponent              │
│   ├── Theme System                     │
│   └── DPI Scaling                      │
└─────────────────────────────────────────┘
         │                    │
    ┌────┴────┐         ┌─────┴─────┐
    │ 3%      │         │ 2%        │
    │ Stand-  │         │ Plugin    │
    │ alone   │         │ Code      │
    └─────────┘         └───────────┘
```

---

## Key Features

### MIDI Visualization

- **16 MIDI channels** per device
- **128 notes** (on/off with velocity)
- **128 control changes** (CC)
- **14-bit Hi-Res CC** (HRCC)
- **Pitch bend** with graph visualization
- **Program change**
- **Channel pressure**
- **Polyphonic pressure** (aftertouch)
- **RPN/NRPN** parameters
- **MPE support** (lower/upper zones)
- **MIDI clock** with BPM calculation
- **SysEx** data (first 20 bytes)

### Display Modes

- **Line mode:** Horizontal bars with current values
- **Graph mode:** Scrolling time-based graphs

### Theme System

- **9 customizable colors**
- **SVG import/export**
- **Built-in themes:** Dark, Light, BStation, Darcula, Disco, Mouha
- **Drag-and-drop** theme loading

### DPI Scaling

- **Automatic scaling** for 4K/Retina displays
- **sm::scaled()** utility for all dimensions
- **Per-monitor DPI** support

---

## Architecture Patterns

### 1. Pimpl Pattern

**Used extensively** for implementation hiding:

```cpp
class MidiDeviceComponent
{
    struct Pimpl;
private:
    std::unique_ptr<Pimpl> pimpl_;
};
```

**Benefits:**
- Faster compile times
- ABI stability
- Clean public headers

### 2. Observer Pattern

```cpp
MidiInput
    └─> DeviceListener (MidiInputCallback)
        └─> MidiDeviceComponent
            └─> ChannelState (data model)
```

### 3. Settings Manager Interface

```cpp
SettingsManager (interface)
    ├─> ShowMidiApplication (standalone)
    │       └─> PropertiesSettings (XML file)
    │
    └─> PluginSettings (plugin)
            └─> MemoryBlock (DAW state)
```

### 4. Component Tree (Composite)

```cpp
MainLayoutComponent
    ├─> SidebarComponent
    │   └─> PortListComponent
    └─> MidiDeviceComponent (x N)
```

---

## Code Quality

### Strengths ✅

- Consistent GPL-3.0 headers
- Pimpl pattern for encapsulation
- Smart pointers (std::unique_ptr)
- JUCE leak detector on all components
- Namespace organization
- Centralized constants
- DPI-aware design

### Areas for Improvement ⚠️

- Only 2 TODO comments (very good!)
- No automated tests
- Limited inline documentation
- Some magic numbers in BPM calculation

### Metrics

| Metric              | Value     | Assessment |
|---------------------|-----------|------------|
| Avg file length     | 151 LOC   | Good ✅     |
| Longest file        | 1,500 LOC | Acceptable |
| Component count     | 38        | Manageable |
| Leak detector usage | 100%      | Excellent ✅|

---

## MIDI Processing Pipeline

```
Hardware MIDI Device
    ↓
[JUCE MidiInput]
    ↓
DeviceListener::handleIncomingMidiMessage()
    ↓
MidiDeviceComponent::handleIncomingMidiMessage()
    ↓
Parse & Update State
    ├─> Notes (ON/OFF)
    ├─> CC (including 14-bit)
    ├─> Pitch Bend
    ├─> Program Change
    ├─> RPN/NRPN/HRCC
    ├─> Clock (BPM calculation)
    └─> SysEx
    ↓
[Render at 60 FPS]
    ↓
MidiDeviceComponent::paint()
    ↓
Screen Display
```

---

## Build System

### Three Build Methods

1. **Projucer** (JUCE native)
   - Edit `showmidi.jucer`
   - Generates Xcode, VS2022, Makefile projects

2. **CMake** (recommended)
   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ```

3. **Direct Makefile** (Linux)
   ```bash
   cd Builds/LinuxMakefile
   make CONFIG=Release
   ```

### Dependencies

- **JUCE** (git submodule)
- **clap-juce-extensions** (for CLAP plugin)
- **VST2 SDK** (optional, user-provided)

---

## Platform Support

| Platform | Standalone | VST3 | AU | AAX | LV2 | CLAP |
|----------|------------|------|----|----|-----|------|
| macOS    | ✅          | ✅    | ✅  | ✅  | ❌   | ✅    |
| Windows  | ✅          | ✅    | ❌  | ✅  | ❌   | ✅    |
| Linux    | ✅          | ✅    | ❌  | ❌  | ✅   | ✅    |
| iOS      | ✅          | ❌    | ❌  | ❌  | ❌   | ❌    |

---

## Component Hierarchy

### By Size (Lines of Code)

1. **MidiDeviceComponent** - 1,500 LOC
   - Most complex component
   - MIDI processing + rendering

2. **ChannelState.h** - 514 LOC
   - Complete MIDI state model
   - 15+ data structures

3. **MainLayoutComponent** - 400 LOC
   - Core layout manager
   - Shared by standalone/plugin

4. **SidebarComponent** - 300 LOC
   - Expandable sidebar
   - Device list + controls

5. **SettingsComponent** - 300 LOC
   - Theme editor
   - Color pickers

---

## Recommendations

### Short-term (1-2 weeks)

1. ✅ Address 2 TODO comments
2. 📝 Add inline documentation to complex methods
3. 🔢 Extract magic numbers to constants
4. 🧹 Run clang-format on all files

### Medium-term (1-3 months)

1. 🧪 Add unit tests for MIDI parsing
2. 🧪 Add integration tests with virtual MIDI
3. 📊 Performance profiling at high message rates
4. 📚 Generate Doxygen API documentation

### Long-term (6-12 months)

1. 🎨 AUv3 plugin (iOS)
2. 💾 MIDI recording/export feature
3. ♿ Accessibility improvements
4. 🌍 Internationalization (i18n)

---

## Key Files to Understand

### For New Contributors

Start with these files in order:

1. **README.md** - Project overview
2. **CONTRIBUTING.md** - Development workflow
3. **Source/Main.cpp** - Application entry point (27 lines!)
4. **Source/ShowMidiApplication.h** - Main app class
5. **Source/MainLayoutComponent.h** - Core UI
6. **Source/MidiDeviceComponent.h** - MIDI visualization
7. **Source/ChannelState.h** - MIDI data model
8. **Source/Theme.h** - Theme system
9. **Source/DpiScaling.h** - DPI utilities
10. **Source/LayoutConstants.h** - UI constants

### For MIDI Logic

1. **Source/ChannelState.h** - Data structures
2. **Source/MidiDeviceComponent.cpp** - handleIncomingMidiMessage()
3. **Source/DeviceListener.cpp** - MIDI input callback

### For UI Customization

1. **Source/Theme.h/cpp** - Theme system
2. **Source/LayoutConstants.h** - All dimensions
3. **Source/DpiScaling.h** - Scaling utilities
4. **Source/UwynLookAndFeel.h/cpp** - Custom L&F

---

## License & Credits

**License:** GPL-3.0  
**Copyright:** © 2023 Uwyn LLC  
**Author:** Geert Bevin  
**UI Design:** Stephen Petoniak  
**Website:** https://www.uwyn.com

---

## Resources

- **Forum:** https://forum.uwyn.com
- **Discord:** https://discord.gg/TgsCnwqWTf
- **Issues:** https://github.com/gbevin/ShowMIDI/issues
- **Releases:** https://github.com/gbevin/ShowMIDI/releases

---

## Analysis Documentation

This analysis generated three comprehensive documents:

1. **[source-code-analysis.md](source-code-analysis.md)** (23 KB)
   - Complete architectural analysis
   - Statistics and metrics
   - Quality assessment
   - Detailed recommendations

2. **[architecture-diagrams.md](architecture-diagrams.md)** (19 KB)
   - System architecture diagrams
   - Component interaction maps
   - MIDI message flow charts
   - State management diagrams

3. **[component-reference.md](component-reference.md)** (19 KB)
   - All 38 components documented
   - Data structure reference
   - API method signatures
   - Usage examples

**Total Documentation:** ~60 KB covering every aspect of the codebase

---

## Conclusion

ShowMIDI demonstrates **excellent software engineering**:

- ✅ Clean architecture with clear separation of concerns
- ✅ Modern C++17 with JUCE best practices
- ✅ Cross-platform design with 95% code sharing
- ✅ Comprehensive MIDI support including MPE
- ✅ Professional code quality and consistency
- ✅ Robust theme system
- ✅ DPI-aware UI for all platforms

The codebase is **well-positioned for future enhancements** while maintaining **stability** and **maintainability**.

---

**Analysis Generated By:** Automated Code Analysis Tool  
**Analysis Date:** November 9, 2025  
**Repository:** https://github.com/peternicholls/ShowMIDI-Fork  
**Branch:** copilot/analyze-source-code
