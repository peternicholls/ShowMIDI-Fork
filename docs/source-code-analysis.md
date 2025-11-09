# ShowMIDI Source Code Analysis

**Version:** 1.1.1  
**Analysis Date:** November 9, 2025  
**Analyzed By:** Automated Code Analysis  

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [Architecture Overview](#architecture-overview)
3. [Codebase Statistics](#codebase-statistics)
4. [Component Hierarchy](#component-hierarchy)
5. [MIDI Processing Pipeline](#midi-processing-pipeline)
6. [UI Framework and DPI Scaling](#ui-framework-and-dpi-scaling)
7. [Theme System](#theme-system)
8. [Plugin Architecture](#plugin-architecture)
9. [Key Data Structures](#key-data-structures)
10. [Build System](#build-system)
11. [Code Quality Assessment](#code-quality-assessment)
12. [Architectural Patterns](#architectural-patterns)
13. [Recommendations](#recommendations)

---

## Executive Summary

ShowMIDI is a **multi-platform MIDI visualization application** built on the JUCE framework. The application is designed to provide real-time, glanceable visualization of MIDI activity across multiple devices and channels.

### Key Characteristics

- **Language:** C++17
- **Framework:** JUCE (audio plugin framework)
- **Total Lines of Code:** ~7,277 lines
- **Source Files:** 20 .cpp, 27 .h, 1 .mm (Objective-C++)
- **License:** GPL-3.0
- **Platforms:** macOS, Windows, Linux, iOS
- **Plugin Formats:** VST3, AU, AAX, LV2, CLAP

### Architecture Highlights

- **Pimpl (Pointer to Implementation) pattern** extensively used for encapsulation
- **DPI-aware UI** with comprehensive scaling utilities
- **Real-time MIDI processing** with non-blocking message handling
- **Theme system** with SVG-based theme import/export
- **Dual deployment**: Standalone application and plugin formats

---

## Architecture Overview

ShowMIDI follows a **layered architecture** with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                        │
│  ┌──────────────────────┐   ┌─────────────────────────┐    │
│  │  Standalone App      │   │   Plugin Processor      │    │
│  │  (Main.cpp)          │   │   (PluginProcessor)     │    │
│  └──────────────────────┘   └─────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                            │
┌─────────────────────────────────────────────────────────────┐
│                      UI Layer                               │
│  ┌──────────────────┐  ┌──────────────────┐               │
│  │  MainLayout      │  │  PluginEditor    │               │
│  │  Component       │  │                  │               │
│  └──────────────────┘  └──────────────────┘               │
│                                                             │
│  ┌──────────────────┐  ┌──────────────────┐               │
│  │  Sidebar         │  │  MidiDevice      │               │
│  │  Component       │  │  Component       │               │
│  └──────────────────┘  └──────────────────┘               │
│                                                             │
│  ┌──────────────────┐  ┌──────────────────┐               │
│  │  Settings        │  │  PortList        │               │
│  │  Component       │  │  Component       │               │
│  └──────────────────┘  └──────────────────┘               │
└─────────────────────────────────────────────────────────────┘
                            │
┌─────────────────────────────────────────────────────────────┐
│                   MIDI Processing Layer                     │
│  ┌──────────────────┐  ┌──────────────────┐               │
│  │  DeviceListener  │  │  DeviceManager   │               │
│  └──────────────────┘  └──────────────────┘               │
│                                                             │
│  ┌──────────────────────────────────────────┐              │
│  │         ChannelState (Data Model)        │              │
│  │  - ActiveChannels                        │              │
│  │  - Notes, CC, PitchBend                  │              │
│  │  - Clock, SysEx                          │              │
│  └──────────────────────────────────────────┘              │
└─────────────────────────────────────────────────────────────┘
                            │
┌─────────────────────────────────────────────────────────────┐
│                   Infrastructure Layer                      │
│  ┌──────────────────┐  ┌──────────────────┐               │
│  │  Theme System    │  │  DPI Scaling     │               │
│  │  (Theme.h/cpp)   │  │  (DpiScaling.h)  │               │
│  └──────────────────┘  └──────────────────┘               │
│                                                             │
│  ┌──────────────────┐  ┌──────────────────┐               │
│  │  Settings        │  │  Layout          │               │
│  │  Manager         │  │  Constants       │               │
│  └──────────────────┘  └──────────────────┘               │
└─────────────────────────────────────────────────────────────┘
```

---

## Codebase Statistics

### File Distribution

| Category          | Count | Lines of Code |
|-------------------|-------|---------------|
| C++ Source (.cpp) | 20    | 5,033         |
| C++ Headers (.h)  | 27    | 2,208         |
| Objective-C++ (.mm)| 1    | 36            |
| **Total**         | **48**| **7,277**     |

### Component Breakdown

```
Source/
├── Application Entry Points (3 files)
│   ├── Main.cpp
│   ├── ShowMidiApplication.h/cpp
│   └── StandaloneWindow.h/cpp
│
├── Plugin Components (6 files)
│   ├── PluginProcessor.h/cpp
│   ├── PluginEditor.h/cpp
│   └── PluginSettings.h/cpp
│
├── UI Components (14 files)
│   ├── MainLayoutComponent.h/cpp
│   ├── MidiDeviceComponent.h/cpp
│   ├── SidebarComponent.h/cpp
│   ├── SettingsComponent.h/cpp
│   ├── PortListComponent.h/cpp
│   ├── StandaloneDevicesComponent.h/cpp
│   └── AboutComponent.h/cpp
│
├── MIDI Processing (8 files)
│   ├── DeviceListener.h/cpp
│   ├── MidiDevicesListener.h/cpp
│   ├── DeviceManager.h
│   ├── ChannelState.h
│   └── DetectDevice.h/mm
│
├── Infrastructure (12 files)
│   ├── Theme.h/cpp
│   ├── DpiScaling.h
│   ├── LayoutConstants.h
│   ├── Settings.h
│   ├── SettingsManager.h
│   ├── PropertiesSettings.h/cpp
│   ├── UwynLookAndFeel.h/cpp
│   └── PaintedButton.h/cpp
│
└── Utilities (5 files)
    ├── MidiDeviceInfoComparator.h
    └── PopupColourSelector.h/cpp
```

---

## Component Hierarchy

ShowMIDI uses **JUCE's Component hierarchy** for UI organization:

### Primary Components

1. **ShowMidiApplication** (`ShowMidiApplication.h/cpp`)
   - Entry point for standalone application
   - Implements `JUCEApplication` and `SettingsManager`
   - Manages application lifecycle

2. **StandaloneWindow** (`StandaloneWindow.h/cpp`)
   - Main application window for standalone mode
   - Contains `MainLayoutComponent`

3. **MainLayoutComponent** (`MainLayoutComponent.h/cpp`)
   - Core layout manager for both standalone and plugin
   - Contains sidebar and MIDI device display area
   - Handles file drag-and-drop for theme loading

4. **MidiDeviceComponent** (`MidiDeviceComponent.h/cpp`)
   - **Most complex component** (~1,500 lines)
   - Renders individual MIDI device columns
   - Processes and visualizes MIDI messages
   - Handles DPI scaling and graphical rendering

5. **SidebarComponent** (`SidebarComponent.h/cpp`)
   - Expandable/collapsible sidebar
   - Contains device list, settings, playback controls
   - Toggle visualization mode (line/graph)

6. **SettingsComponent** (`SettingsComponent.h/cpp`)
   - Theme editor and settings UI
   - Color pickers for theme customization
   - Import/export theme functionality

### Plugin-Specific Components

1. **ShowMIDIPluginAudioProcessor** (`PluginProcessor.h/cpp`)
   - JUCE `AudioProcessor` implementation
   - Declares as MIDI effect plugin
   - Handles plugin state save/load

2. **ShowMIDIPluginAudioProcessorEditor** (`PluginEditor.h/cpp`)
   - Plugin UI editor
   - Embeds `MainLayoutComponent`
   - Forwards MIDI messages from processor

---

## MIDI Processing Pipeline

### Message Flow

```
MIDI Hardware/DAW
       ↓
[JUCE MidiInput]
       ↓
DeviceListener::handleIncomingMidiMessage()
       ↓
MidiDeviceComponent::handleIncomingMidiMessage()
       ↓
[Parse Message Type]
       ↓
    ┌──────┴────────┐
    │               │
[Channel Msg]  [System Msg]
    │               │
    ↓               ↓
ActiveChannel   Clock/SysEx
    │
    ├─→ Notes (ON/OFF)
    ├─→ ControlChanges (CC, HRCC)
    ├─→ PitchBend
    ├─→ ProgramChange
    ├─→ ChannelPressure
    ├─→ Parameters (RPN, NRPN)
    └─→ MPE Configuration
    
       ↓
[Render to Screen]
       ↓
MidiDeviceComponent::paint()
```

### Key Data Structures

The MIDI state is stored in `ChannelState.h`:

```cpp
struct ActiveChannels
{
    ActiveChannel channel_[16];  // 16 MIDI channels
    Sysex sysex_;
    Clock clock_;
};

struct ActiveChannel
{
    Notes notes_;                // 128 notes (ON/OFF)
    ControlChanges controlChanges_;  // 128 CCs
    ProgramChange programChange_;
    ChannelPressure channelPressure_;
    PitchBend pitchBend_;
    Parameters hrccs_;  // Hi-Res CC (14-bit)
    Parameters rpns_;   // Registered Parameter Numbers
    Parameters nrpns_;  // Non-Registered Parameter Numbers
    
    // MPE support
    bool mpeManager_;
    MpeMember mpeMember_;  // mpeLower, mpeUpper, mpeNone
};
```

### Message Processing

**Real-time Processing Characteristics:**

1. **Non-blocking:** All MIDI processing happens in `handleIncomingMidiMessage()` without blocking the audio thread
2. **History tracking:** Recent values are stored for graph visualization
3. **Time-based decay:** Old messages auto-hide after inactivity
4. **BPM calculation:** MIDI clock messages are analyzed to compute tempo
5. **MPE support:** Channels 1 and 16 can be configured as MPE managers

---

## UI Framework and DPI Scaling

### DPI Scaling System

ShowMIDI implements a **comprehensive DPI scaling system** in `DpiScaling.h`:

```cpp
namespace sm
{
    // Returns scale factor (1.0, 1.5, 2.0, etc.)
    float dpiScale() noexcept;
    
    // Scale any numeric value
    template <typename Numeric>
    Numeric scaled(Numeric value) noexcept;
    
    // Scale relative to specific component
    template <typename Numeric>
    Numeric scaled(Numeric value, const juce::Component& c) noexcept;
    
    // Scale rectangles
    juce::Rectangle<int> scaled(juce::Rectangle<int> r) noexcept;
}
```

**Usage Pattern:**

```cpp
// ✅ Correct - DPI-aware
button.setBounds(sm::scaled(10), sm::scaled(20), 
                 sm::scaled(100), sm::scaled(30));

// ❌ Wrong - Hard-coded pixels
button.setBounds(10, 20, 100, 30);
```

### Layout Constants

All UI dimensions are centralized in `LayoutConstants.h`:

```cpp
namespace showmidi::layout
{
    // Device dimensions
    static constexpr int STANDARD_WIDTH = 150;
    static constexpr int WIDTH_SEPARATOR = 48;
    
    // Typography
    static constexpr int FONT_SIZE = 9;
    static constexpr int LINE_HEIGHT_NORMAL = 22;
    
    // Sidebar
    static constexpr int SIDEBAR_EXPANDED_WIDTH = 210;
    static constexpr int SIDEBAR_COLLAPSED_WIDTH = 36;
    
    // MIDI display positions
    static constexpr int X_NOTE = 48;
    static constexpr int X_CC = 177;  // X_MID + 6
    static constexpr int Y_NOTE = 7;
}
```

**Benefits:**
- Single source of truth for all dimensions
- Easy to adjust layouts globally
- DPI scaling applied consistently
- iOS portrait mode has separate narrow constants

---

## Theme System

### Architecture

The theme system is defined in `Theme.h/cpp`:

```cpp
struct Theme
{
    Colour colorBackground;
    Colour colorSidebar;
    Colour colorSeperator;
    Colour colorTrack;
    Colour colorLabel;
    Colour colorData;
    Colour colorPositive;    // Blue - pitch bend up, etc.
    Colour colorNegative;    // Red - pitch bend down, etc.
    Colour colorController;  // Orange - CC values
    
    String generateXml();       // Export theme
    void parseXml(const String&);  // Import theme
    void randomize();           // Random colors
};
```

### Built-in Themes

```cpp
static const Theme THEME_DARK = { ... };
static const Theme THEME_LIGHT = { ... };
```

Additional themes in `Themes/` directory:
- bstation.svg
- classic light.svg
- darcula.svg
- disco.svg
- mouha.svg

### SVG Theme Format

Themes are stored as SVG files with custom XML:

```xml
<svg>
  <metadata>
    <showmidi:theme>
      <colorBackground>#29272B</colorBackground>
      <colorSidebar>#201E21</colorSidebar>
      <!-- ... -->
    </showmidi:theme>
  </metadata>
</svg>
```

**Drag-and-drop support:** Users can drag SVG theme files onto the application to load them.

---

## Plugin Architecture

### JUCE Plugin Framework

ShowMIDI implements a **MIDI effect plugin** using JUCE's AudioProcessor API:

```cpp
class ShowMIDIPluginAudioProcessor : public AudioProcessor
{
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }
    
    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;
};
```

### Plugin Formats Supported

- **VST3** (Windows, macOS, Linux)
- **AU** (Audio Units - macOS only)
- **AAX** (Pro Tools - requires AAX SDK)
- **LV2** (Linux)
- **CLAP** (via CMake build)

### State Persistence

Plugin settings are saved via JUCE's `getStateInformation()`:

```cpp
void getStateInformation(MemoryBlock& destData) override
{
    // Save theme, visibility settings, etc.
}

void setStateInformation(const void* data, int sizeInBytes) override
{
    // Restore saved state
}
```

### Dual Architecture

ShowMIDI shares **95% of the code** between standalone and plugin:

```
Standalone:
  Main.cpp → ShowMidiApplication → StandaloneWindow
             → MainLayoutComponent → MidiDeviceComponent

Plugin:
  PluginProcessor → PluginEditor → MainLayoutComponent
                                  → MidiDeviceComponent
```

**Key difference:** 
- Standalone manages its own `DeviceManager` (MIDI input discovery)
- Plugin receives MIDI from the host DAW

---

## Key Data Structures

### ChannelState.h

Defines the **complete MIDI state model**:

#### TimedValue
```cpp
struct TimedValue
{
    Time time_;
    int value_;
};
```

#### ChannelMessage
```cpp
struct ChannelMessage
{
    TimedValue current_;
    std::vector<TimedValue> history_;  // For graph visualization
};
```

#### Notes
```cpp
struct Notes
{
    NoteOn noteOn_[128];
    NoteOff noteOff_[128];
    Time time_;
};
```

#### ControlChanges
```cpp
struct ControlChanges
{
    ControlChange controlChange_[128];
    Time time_;
};
```

#### Parameters (RPN/NRPN/HRCC)
```cpp
struct Parameters
{
    std::map<int, Parameter> param_;  // Dynamic parameter map
    Time time_;
};
```

#### Clock
```cpp
struct Clock
{
    Time timeBpm_;
    Time timeStart_;
    Time timeContinue_;
    Time timeStop_;
    double bpm_;
};
```

#### SysEx
```cpp
struct Sysex
{
    static constexpr int MAX_SYSEX_DATA = 20;
    uint8 data_[MAX_SYSEX_DATA];
    int length_;
    Time time_;
};
```

### MPE (MIDI Polyphonic Expression) Support

```cpp
enum MpeMember
{
    mpeNone,
    mpeLower,  // Channels 2-15 (lower zone)
    mpeUpper   // Channels 1-14 (upper zone)
};

struct ActiveChannel
{
    bool mpeManager_;       // Channel 1 or 16
    MpeMember mpeMember_;
};
```

**MPE Configuration:**
- Channel 1: Lower zone manager
- Channel 16: Upper zone manager
- RPN 6 (MSB=0) enables/disables MPE zones
- Dynamic member channel assignment

---

## Build System

### Multi-Platform Build Support

ShowMIDI uses **three build systems**:

#### 1. Projucer (JUCE native)

```
showmidi.jucer → Platform-specific projects
    ├── Builds/MacOSX/*.xcodeproj (Xcode)
    ├── Builds/VisualStudio2022/*.sln
    ├── Builds/LinuxMakefile/Makefile
    └── Builds/iOS/*.xcodeproj
```

**Workflow:**
1. Edit `showmidi.jucer` in Projucer
2. Click "Save Project" to regenerate build files
3. Build in native IDE

#### 2. CMake (Cross-platform)

```cmake
cmake_minimum_required(VERSION 3.15)
project(ShowMIDI VERSION 1.1.1)
set(CMAKE_CXX_STANDARD 17)

# Platform detection
if (APPLE)
    set(JUCER_GENERATOR "Xcode")
elseif (WIN32)
    set(JUCER_GENERATOR "VisualStudio2022")
else ()
    set(JUCER_GENERATOR "LinuxMakefile")
endif ()
```

**CLAP plugin support:** Uses `clap-juce-extensions` submodule

#### 3. Direct Makefile (Linux)

```bash
cd Builds/LinuxMakefile
make CONFIG=Release
```

### Dependencies

```
ShowMIDI/
├── JUCE/                    # Git submodule
├── libs/
│   ├── clap-juce-extensions/  # CLAP plugin support
│   └── vst2/                  # VST2 SDK (not included - user-provided)
```

**Required packages (Linux):**
```bash
sudo apt install build-essential pkg-config \
  libasound2-dev libfreetype6-dev \
  libx11-dev libxrandr-dev \
  libxinerama-dev libxcursor-dev
```

---

## Code Quality Assessment

### Strengths

1. ✅ **Consistent GPL-3.0 licensing headers** on all source files
2. ✅ **Pimpl pattern** extensively used for encapsulation
3. ✅ **DPI-aware design** with comprehensive scaling utilities
4. ✅ **JUCE leak detector** used on all components
5. ✅ **Smart pointers** (`std::unique_ptr`) for memory management
6. ✅ **Namespace organization** (`showmidi`, `showmidi::layout`, `sm`)
7. ✅ **Centralized constants** in `LayoutConstants.h`
8. ✅ **Comprehensive MIDI support** (MPE, RPN, NRPN, 14-bit CC)

### Areas for Improvement

1. ⚠️ **TODO comments present** (2 instances found):
   - `MidiDeviceComponent.cpp:26` - Refactor header underline drawing
   - `SettingsComponent.cpp:367` - Use DPI scaling for container width

2. ⚠️ **Documentation coverage:**
   - Header files have brief comments
   - Implementation files have minimal inline documentation
   - Missing Doxygen-style API documentation

3. ⚠️ **Test coverage:**
   - No unit tests found
   - No integration tests
   - Manual testing only

4. ⚠️ **Magic numbers:**
   - Some hard-coded values in MIDI processing (e.g., BPM calculation thresholds)

### Code Metrics

| Metric                  | Value    | Assessment |
|-------------------------|----------|------------|
| Average file length     | ~151 LOC | Good ✅     |
| Longest file            | ~1,500   | Acceptable |
| Component count         | 8        | Manageable |
| Namespace usage         | Consistent | Good ✅   |
| Memory management       | Smart pointers | Excellent ✅ |

---

## Architectural Patterns

### 1. Pimpl (Pointer to Implementation)

**Used extensively** to hide implementation details:

```cpp
// Header
class MidiDeviceComponent : public Component
{
    struct Pimpl;
private:
    std::unique_ptr<Pimpl> pimpl_;
};

// Implementation
struct MidiDeviceComponent::Pimpl : public MidiInputCallback
{
    // All implementation details here
};
```

**Benefits:**
- Faster compile times (changes don't ripple to dependents)
- ABI stability
- Clean public API

### 2. Settings Manager Pattern

```cpp
class SettingsManager
{
public:
    virtual bool isPlugin() = 0;
    virtual Component* getTopLevelComponent() = 0;
    virtual Settings& getSettings() = 0;
    virtual void applySettings() = 0;
    virtual void storeSettings() = 0;
    virtual MidiDevicesListeners& getMidiDevicesListeners() = 0;
};
```

**Implementations:**
- `ShowMidiApplication` (standalone)
- `PluginSettings` (plugin)

### 3. Observer Pattern (Listeners)

```cpp
class DeviceListener : public MidiInputCallback
{
    void handleIncomingMidiMessage(MidiInput*, const MidiMessage&) override;
};

class MidiDevicesListeners
{
    void addListener(DeviceListener*);
    void removeListener(DeviceListener*);
};
```

### 4. Component Tree (Composite Pattern)

JUCE's Component hierarchy forms a composite tree:

```
StandaloneWindow
  └── MainLayoutComponent
      ├── SidebarComponent
      │   ├── PortListComponent
      │   └── PaintedButton (multiple)
      └── MidiDeviceComponent (multiple)
```

---

## Recommendations

### Short-term Improvements

1. **Address TODO comments:**
   - Refactor header underline drawing into helper function
   - Fix DPI scaling in settings component container width

2. **Add inline documentation:**
   - Document complex MIDI processing logic
   - Add Doxygen comments for public APIs

3. **Extract magic numbers:**
   - BPM calculation thresholds → constants
   - MIDI clock queue size → constant
   - Timestamp filtering tolerance (1%) → constant

4. **Code cleanup:**
   - Remove commented-out code (if any)
   - Consistent formatting (verify clang-format applied)

### Medium-term Enhancements

1. **Unit tests:**
   - Test MIDI message parsing
   - Test MPE zone configuration
   - Test theme XML parsing

2. **Integration tests:**
   - Automated UI tests with virtual MIDI
   - Cross-platform rendering tests
   - Plugin host compatibility tests

3. **Performance profiling:**
   - Measure rendering performance at high MIDI rates
   - Optimize graph visualization (if needed)
   - Profile memory usage

4. **Documentation:**
   - Generate Doxygen API reference
   - Add architecture diagrams (UML)
   - Document MIDI message flow in detail

### Long-term Considerations

1. **Plugin format expansion:**
   - Consider AUv3 (iOS)
   - Consider VST2 deprecation timeline

2. **Feature additions:**
   - MIDI export/recording
   - Custom MIDI mappings
   - Preset management
   - Multi-instance support

3. **Accessibility:**
   - Screen reader support
   - Keyboard navigation
   - High contrast themes

4. **Internationalization:**
   - Translatable UI strings
   - Locale-specific formatting

---

## Conclusion

ShowMIDI demonstrates **excellent architectural design** and **professional code quality**. The use of modern C++17, JUCE framework best practices, comprehensive DPI scaling, and the Pimpl pattern results in a maintainable, cross-platform codebase.

**Key Strengths:**
- Clean separation of concerns (UI, MIDI processing, infrastructure)
- Comprehensive MIDI support (including MPE)
- Robust theme system
- Dual standalone/plugin architecture with code sharing

**Opportunities:**
- Add automated testing
- Improve inline documentation
- Extract remaining magic numbers
- Performance profiling at high message rates

The codebase is **well-positioned for future enhancements** while maintaining stability and cross-platform compatibility.

---

**Generated by:** Automated Source Code Analysis  
**Repository:** https://github.com/peternicholls/ShowMIDI-Fork  
**License:** GPL-3.0  
**JUCE Version:** Submodule at commit 4f43011
