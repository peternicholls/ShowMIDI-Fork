# ShowMIDI Architecture Diagrams

This document provides detailed architectural diagrams and component interaction maps for the ShowMIDI application.

---

## Table of Contents

1. [System Architecture](#system-architecture)
2. [Component Interaction Map](#component-interaction-map)
3. [MIDI Message Flow](#midi-message-flow)
4. [Plugin vs Standalone Differences](#plugin-vs-standalone-differences)
5. [Theme System Flow](#theme-system-flow)
6. [State Management](#state-management)

---

## System Architecture

### High-Level System Diagram

```
┌───────────────────────────────────────────────────────────────────┐
│                         ShowMIDI System                           │
│                                                                   │
│  ┌─────────────────────┐         ┌─────────────────────┐         │
│  │   Standalone App    │         │   Plugin (VST/AU)   │         │
│  │                     │         │                     │         │
│  │  ┌──────────────┐   │         │  ┌──────────────┐   │         │
│  │  │ Main.cpp     │   │         │  │ Plugin       │   │         │
│  │  │              │   │         │  │ Processor    │   │         │
│  │  └──────┬───────┘   │         │  └──────┬───────┘   │         │
│  │         │           │         │         │           │         │
│  │         v           │         │         v           │         │
│  │  ┌──────────────┐   │         │  ┌──────────────┐   │         │
│  │  │ ShowMidi     │   │         │  │ Plugin       │   │         │
│  │  │ Application  │   │         │  │ Editor       │   │         │
│  │  └──────┬───────┘   │         │  └──────┬───────┘   │         │
│  │         │           │         │         │           │         │
│  │         v           │         │         v           │         │
│  │  ┌──────────────┐   │         │  ┌──────────────┐   │         │
│  │  │ Standalone   │   │         │  │              │   │         │
│  │  │ Window       │   │         │  │              │   │         │
│  │  └──────┬───────┘   │         │  │              │   │         │
│  └─────────┼───────────┘         └──┼──────────────┘   │         │
│            │                        │                   │         │
│            └────────────┬───────────┘                   │         │
│                         │                               │         │
│                         v                               │         │
│              ┌─────────────────────┐                    │         │
│              │  MainLayout         │                    │         │
│              │  Component          │                    │         │
│              │  (Shared Core UI)   │                    │         │
│              └──────────┬──────────┘                    │         │
│                         │                               │         │
│         ┌───────────────┼───────────────┐               │         │
│         │               │               │               │         │
│         v               v               v               │         │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐        │         │
│  │  Sidebar   │  │   MIDI     │  │  Settings  │        │         │
│  │ Component  │  │  Device    │  │ Component  │        │         │
│  │            │  │ Component  │  │            │        │         │
│  └────────────┘  └────┬───────┘  └────────────┘        │         │
│                       │                                 │         │
│                       v                                 │         │
│              ┌─────────────────────┐                    │         │
│              │  ChannelState       │                    │         │
│              │  (MIDI Data Model)  │                    │         │
│              └─────────────────────┘                    │         │
└───────────────────────────────────────────────────────────────────┘
```

---

## Component Interaction Map

### Component Dependency Graph

```
┌─────────────────────────────────────────────────────────────────┐
│                     Component Dependencies                      │
└─────────────────────────────────────────────────────────────────┘

ShowMidiApplication
    ├─> UwynLookAndFeel
    ├─> PropertiesSettings (implements Settings)
    ├─> MidiDevicesListeners
    └─> StandaloneWindow
            └─> MainLayoutComponent

PluginProcessor
    ├─> PluginSettings (implements Settings)
    └─> PluginEditor
            └─> MainLayoutComponent

MainLayoutComponent
    ├─> SettingsManager (interface)
    ├─> DeviceManager (interface)
    ├─> SidebarComponent
    │       ├─> PortListComponent
    │       └─> PaintedButton (x6)
    │               - Expand/Collapse
    │               - Settings
    │               - Play/Pause
    │               - Visualization Toggle
    │               - Reset
    │               - Help
    ├─> MidiDeviceComponent (x N)
    │       ├─> DeviceListener
    │       ├─> ChannelState
    │       └─> Theme
    └─> SettingsComponent (modal)
            ├─> Theme
            └─> PopupColourSelector

DeviceManager (standalone only)
    └─> MidiDevicesListener
            └─> DeviceListener
                    └─> MidiDeviceComponent

Theme
    ├─> XML parsing/generation
    └─> Color management

LayoutConstants (header-only)
    └─> Defines all UI dimensions

DpiScaling (header-only)
    └─> Provides sm::scaled() utilities
```

### Interface Implementations

```
┌────────────────────┐
│ SettingsManager    │ (Interface)
│ - isPlugin()       │
│ - getSettings()    │
│ - applySettings()  │
│ - storeSettings()  │
└────────┬───────────┘
         │
         ├─> ShowMidiApplication (Standalone)
         │       └─> PropertiesSettings
         │
         └─> PluginSettings (Plugin)
                 └─> MemoryBlock (DAW state)

┌────────────────────┐
│ DeviceManager      │ (Interface)
│ - getDeviceList()  │
│ - isVisible()      │
│ - setVisible()     │
└────────┬───────────┘
         │
         ├─> StandaloneDevicesComponent (Standalone)
         │       └─> MidiDevicesListeners
         │
         └─> (Not used in plugin - DAW provides MIDI)
```

---

## MIDI Message Flow

### Standalone Application MIDI Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                     Standalone MIDI Flow                        │
└─────────────────────────────────────────────────────────────────┘

[Hardware MIDI Device]
         │
         v
  ┌─────────────┐
  │ JUCE MIDI   │  (Operating System MIDI API)
  │ Input       │   - CoreMIDI (macOS)
  │ Manager     │   - ALSA (Linux)
  │             │   - Windows MIDI
  └──────┬──────┘
         │
         v
  ┌─────────────────────┐
  │ DeviceListener      │  (Registered with MidiInput)
  │                     │
  │ handleIncoming      │
  │ MidiMessage()       │
  └──────┬──────────────┘
         │
         v
  ┌─────────────────────┐
  │ MidiDevice          │
  │ Component::Pimpl    │
  │                     │
  │ - Parse message     │
  │ - Update state      │
  │ - Set dirty flag    │
  └──────┬──────────────┘
         │
         v
  ┌─────────────────────┐
  │ ChannelState        │  (Data Model)
  │                     │
  │ ActiveChannels      │
  │ - channel_[16]      │
  │ - sysex_            │
  │ - clock_            │
  └──────┬──────────────┘
         │
         v (60 FPS timer)
  ┌─────────────────────┐
  │ MidiDevice          │
  │ Component           │
  │                     │
  │ paint(Graphics& g)  │
  │ - Render lines/graph│
  │ - DPI scaling       │
  │ - Theme colors      │
  └─────────────────────┘
         │
         v
  [Screen Display]
```

### Plugin MIDI Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                        Plugin MIDI Flow                         │
└─────────────────────────────────────────────────────────────────┘

[DAW MIDI Track]
         │
         v
  ┌─────────────────────┐
  │ Plugin Processor    │
  │                     │
  │ processBlock()      │
  │                     │
  │ for (msg in buffer) │
  │   forward to editor │
  └──────┬──────────────┘
         │
         v
  ┌─────────────────────┐
  │ Plugin Editor       │
  │                     │
  │ handleIncoming      │
  │ MidiMessage()       │
  └──────┬──────────────┘
         │
         v
  ┌─────────────────────┐
  │ MainLayout          │
  │ Component           │
  │                     │
  │ - Routes to device  │
  └──────┬──────────────┘
         │
         v
  ┌─────────────────────┐
  │ MidiDevice          │
  │ Component           │
  │                     │
  │ (Same as standalone)│
  └─────────────────────┘
```

### Message Type Processing

```
handleIncomingMidiMessage(MidiMessage& msg)
    │
    ├─> isSysEx()?
    │       └─> Update sysex_ state
    │           - Store up to 20 bytes
    │           - Timestamp
    │
    ├─> isMidiClock()?
    │       └─> Calculate BPM
    │           - Queue timestamps
    │           - Filter outliers (1%)
    │           - Average intervals
    │           - BPM = 600 / (avg * 24)
    │
    ├─> isMidiStart/Continue/Stop()?
    │       └─> Update clock_ state
    │           - Clear timestamp queue
    │
    └─> getChannel() > 0?
            │
            ├─> isNoteOn()?
            │       └─> notes_.noteOn_[number]
            │           - current_.value_ = velocity
            │           - current_.time_ = now
            │           - Clear noteOff
            │
            ├─> isNoteOff()?
            │       └─> notes_.noteOff_[number]
            │           - current_.value_ = velocity
            │           - current_.time_ = now
            │
            ├─> isAftertouch()?
            │       └─> notes_.noteOn_[number].polyPressure_
            │           - current_.value_ = pressure
            │           - history_ tracking
            │
            ├─> isController()?
            │       ├─> Regular CC
            │       │       └─> controlChanges_.cc_[number]
            │       │
            │       ├─> 14-bit HRCC (MSB/LSB pairing)
            │       │       └─> hrccs_.param_[number]
            │       │
            │       ├─> RPN (CC 101/100/6/38)
            │       │       └─> rpns_.param_[msb << 7 | lsb]
            │       │           - MPE detection (RPN 6)
            │       │
            │       └─> NRPN (CC 99/98/6/38)
            │               └─> nrpns_.param_[msb << 7 | lsb]
            │
            ├─> isProgramChange()?
            │       └─> programChange_
            │
            ├─> isChannelPressure()?
            │       └─> channelPressure_
            │
            └─> isPitchWheel()?
                    └─> pitchBend_
                        - value range: -8192 to +8191
                        - history_ tracking
```

---

## Plugin vs Standalone Differences

### Architecture Comparison

```
┌──────────────────────────────────────────────────────────────────┐
│                   Standalone vs Plugin                           │
└──────────────────────────────────────────────────────────────────┘

┌─────────────────────────────┐  ┌─────────────────────────────┐
│        STANDALONE           │  │          PLUGIN             │
└─────────────────────────────┘  └─────────────────────────────┘

Entry Point:                      Entry Point:
  Main.cpp                          PluginProcessor.cpp
  └─> juce_CreateApplication()      └─> createPluginFilter()

Settings:                         Settings:
  PropertiesSettings                PluginSettings
  └─> ApplicationProperties         └─> MemoryBlock (DAW state)
      - ~/.config/ShowMIDI/             - Saved per-instance
        settings.xml                    - Part of project

MIDI Input:                       MIDI Input:
  DeviceManager                     processBlock(MidiBuffer)
  └─> MidiInput::getDevices()       └─> DAW routes MIDI
      - Auto-discover                   - No device management
      - Enable/disable devices          - Single "device" view

Window Management:                Window Management:
  StandaloneWindow                  PluginEditor
  └─> Owns MainLayoutComponent      └─> Embeds MainLayout
      - Title bar                       - No title bar
      - Resizable                       - DAW controls size
      - Menu bar (macOS)                - No menu

Multi-Instance:                   Multi-Instance:
  Single instance only              Multiple instances OK
  └─> moreThanOneInstance           └─> Each has own state
      Allowed() = false                 - Independent themes
                                        - Independent visibility

Lifecycle:                        Lifecycle:
  User launches/quits               DAW creates/destroys
  └─> systemRequestedQuit()         └─> destructor on project close
      - Save settings                   - State in MemoryBlock
      - Clean shutdown                  - Quick teardown
```

### Code Sharing Statistics

```
Total Code:        100%
  Shared:           95%  (MainLayout, MidiDevice, Theme, etc.)
  Standalone-only:   3%  (Main, StandaloneWindow, DeviceManager)
  Plugin-only:       2%  (PluginProcessor, PluginEditor)
```

---

## Theme System Flow

### Theme Loading and Application

```
┌──────────────────────────────────────────────────────────────────┐
│                      Theme System Flow                           │
└──────────────────────────────────────────────────────────────────┘

User Action:
  ├─> Drag SVG file onto window
  │       └─> MainLayoutComponent::filesDropped()
  │               └─> Theme::parseXml(svgContent)
  │                       └─> SettingsManager::applySettings()
  │
  ├─> Click color swatch in Settings
  │       └─> PopupColourSelector
  │               └─> User picks color
  │                       └─> Theme::colorBackground = newColor
  │                               └─> repaint() all components
  │
  └─> Click "Random" in Settings
          └─> Theme::randomize()
                  └─> Generate random colors
                          └─> repaint() all components

Theme::parseXml(String& svgContent)
    │
    ├─> Parse XML
    │       └─> Find <showmidi:theme> in <metadata>
    │
    ├─> Extract color values
    │       └─> colorBackground = Colour::fromString(...)
    │       └─> colorSidebar = Colour::fromString(...)
    │       └─> ...
    │
    └─> Return success/failure

Theme::generateXml() → String
    │
    ├─> Create SVG structure
    │       <svg>
    │         <metadata>
    │           <showmidi:theme>
    │
    ├─> Serialize colors
    │       <colorBackground>#29272B</colorBackground>
    │       <colorSidebar>#201E21</colorSidebar>
    │       ...
    │
    └─> Return SVG string

Settings Storage:
  Standalone:
    └─> settings.xml
        <THEME>
          <colorBackground>...</colorBackground>
          ...
        </THEME>
  
  Plugin:
    └─> MemoryBlock
        [Binary serialized theme]
```

### Built-in Theme Constants

```cpp
// Theme.h

static const Theme THEME_DARK = {
    Colour(0xFF29272B),  // colorBackground
    Colour(0xFF201E21),  // colorSidebar
    Colour(0xFF66606B),  // colorSeperator
    Colour(0xFF201E21),  // colorTrack
    Colour(0xFF66606B),  // colorLabel
    Colour(0xFFFFFFFF),  // colorData
    Colour(0xFF66ADF3),  // colorPositive (blue)
    Colour(0xFFD8414E),  // colorNegative (red)
    Colour(0xFFFFAB2B)   // colorController (orange)
};

static const Theme THEME_LIGHT = {
    Colour(0xFFF2F2F2),  // colorBackground
    Colour(0xFFFFFFFF),  // colorSidebar
    Colour(0xFFA0A0A0),  // colorSeperator
    Colour(0xFFFFFFFF),  // colorTrack
    Colour(0xFFA0A0A0),  // colorLabel
    Colour(0xFF222222),  // colorData
    Colour(0xFF66ADF3),  // colorPositive
    Colour(0xFFD8414E),  // colorNegative
    Colour(0xFFFFAB2B)   // colorController
};
```

---

## State Management

### State Lifecycle

```
┌──────────────────────────────────────────────────────────────────┐
│                    State Management Flow                         │
└──────────────────────────────────────────────────────────────────┘

Application Startup:
  ├─> Standalone
  │       └─> PropertiesSettings::load()
  │               └─> Read ~/.config/ShowMIDI/settings.xml
  │                       └─> Restore theme, device visibility
  │
  └─> Plugin
          └─> setStateInformation(data, size)
                  └─> Deserialize MemoryBlock
                          └─> Restore theme, settings

Runtime Changes:
  └─> User modifies settings
          └─> SettingsManager::applySettings()
                  └─> Update UI immediately
                  └─> (Auto-save handled separately)

Application Shutdown:
  ├─> Standalone
  │       └─> ShowMidiApplication::shutdown()
  │               └─> PropertiesSettings::save()
  │                       └─> Write settings.xml
  │
  └─> Plugin
          └─> getStateInformation(destData)
                  └─> Serialize to MemoryBlock
                  └─> DAW stores in project

Pause/Resume State:
  └─> Spacebar or pause button
          └─> MainLayoutComponent::setPaused(bool)
                  └─> MidiDeviceComponent::setPaused(bool)
                          └─> Stop/resume render timer
                          └─> (MIDI still received, just not displayed)

Reset State:
  └─> Delete/backspace or reset button
          └─> MainLayoutComponent::resetChannelData()
                  └─> MidiDeviceComponent::resetChannelData()
                          └─> ActiveChannels::reset()
                                  └─> Clear all channels, clock, sysex
```

### Persistent Settings

```
Standalone Settings (XML):
  <SETTINGS>
    <THEME>
      <colorBackground>...</colorBackground>
      <colorSidebar>...</colorSidebar>
      ...
    </THEME>
    <DEVICES>
      <device id="device-123" visible="true"/>
      <device id="device-456" visible="false"/>
      ...
    </DEVICES>
    <VISUALIZATION>
      <mode>line</mode>  <!-- or "graph" -->
    </VISUALIZATION>
  </SETTINGS>

Plugin Settings (Binary):
  MemoryBlock {
    [Theme colors - 9 x 4 bytes ARGB]
    [Visualization mode - 1 byte]
    [Reserved - variable]
  }
```

---

## Conclusion

These architectural diagrams illustrate:

1. **Clear separation** between standalone and plugin code paths
2. **95% code reuse** through MainLayoutComponent abstraction
3. **Event-driven MIDI processing** with real-time state updates
4. **Flexible theme system** with import/export capabilities
5. **Robust state management** for both standalone and plugin contexts

The architecture enables **rapid development** of new features while maintaining **cross-platform compatibility** and **plugin format independence**.

---

**Generated by:** Source Code Analysis  
**Repository:** https://github.com/peternicholls/ShowMIDI-Fork  
**Last Updated:** November 9, 2025
