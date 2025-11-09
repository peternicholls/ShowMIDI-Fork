# ShowMIDI Component Reference

A detailed reference guide to all major components in the ShowMIDI codebase.

---

## Table of Contents

1. [Application Components](#application-components)
2. [UI Components](#ui-components)
3. [MIDI Processing Components](#midi-processing-components)
4. [Infrastructure Components](#infrastructure-components)
5. [Data Structures](#data-structures)
6. [Utilities](#utilities)

---

## Application Components

### ShowMidiApplication

**File:** `ShowMidiApplication.h/cpp`  
**Inherits:** `JUCEApplication`, `SettingsManager`  
**Purpose:** Main application entry point for standalone mode

**Key Responsibilities:**
- Application lifecycle management (initialise, shutdown)
- Settings persistence via `PropertiesSettings`
- Look and feel management (`UwynLookAndFeel`)
- Window management (`StandaloneWindow`)
- MIDI device discovery coordination

**Key Methods:**
```cpp
void initialise(const String& commandLine) override;
void shutdown() override;
Settings& getSettings() override;
void applySettings() override;
void storeSettings() override;
Component* getTopLevelComponent() override;
```

**Singleton Access:**
```cpp
#define SMApp ShowMidiApplication::getInstance()

// Usage:
SMApp.getSettings().getTheme();
```

---

### StandaloneWindow

**File:** `StandaloneWindow.h/cpp`  
**Inherits:** `DocumentWindow`  
**Purpose:** Main application window for standalone mode

**Key Responsibilities:**
- Window chrome (title bar, close button)
- Window positioning and size persistence
- Hosting `MainLayoutComponent`
- Menu bar (macOS)

**Key Methods:**
```cpp
void closeButtonPressed() override;
void resized() override;
```

---

### ShowMIDIPluginAudioProcessor

**File:** `PluginProcessor.h/cpp`  
**Inherits:** `AudioProcessor`  
**Purpose:** JUCE plugin audio processor

**Key Responsibilities:**
- MIDI effect plugin implementation
- MIDI message forwarding to editor
- Plugin state save/load
- DAW integration

**Key Methods:**
```cpp
void processBlock(AudioBuffer<float>&, MidiBuffer&) override;
AudioProcessorEditor* createEditor() override;
void getStateInformation(MemoryBlock& destData) override;
void setStateInformation(const void* data, int sizeInBytes) override;

bool acceptsMidi() const override { return true; }
bool producesMidi() const override { return true; }
bool isMidiEffect() const override { return true; }
```

**Plugin Metadata:**
- Name: "ShowMIDI"
- Type: MIDI Effect
- Formats: VST3, AU, AAX, LV2, CLAP

---

### ShowMIDIPluginAudioProcessorEditor

**File:** `PluginEditor.h/cpp`  
**Inherits:** `AudioProcessorEditor`  
**Purpose:** Plugin UI editor

**Key Responsibilities:**
- Embed `MainLayoutComponent`
- Forward MIDI messages from processor
- Plugin window sizing
- Editor lifecycle management

**Key Methods:**
```cpp
void handleIncomingMidiMessage(const MidiMessage&);
void paint(Graphics&) override;
void resized() override;
```

---

## UI Components

### MainLayoutComponent

**File:** `MainLayoutComponent.h/cpp`  
**Inherits:** `Component`, `FileDragAndDropTarget`  
**Purpose:** Core layout manager (shared by standalone and plugin)

**Key Responsibilities:**
- Layout sidebar and MIDI device columns
- Coordinate device visibility
- Handle theme drag-and-drop
- Manage settings modal
- Respond to window resize

**Pimpl Contents:**
```cpp
struct Pimpl
{
    SettingsManager* settingsManager_;
    DeviceManager* deviceManager_;
    MainLayoutType layoutType_;  // standalone or plugin
    
    std::unique_ptr<SidebarComponent> sidebar_;
    std::vector<std::unique_ptr<MidiDeviceComponent>> devices_;
    std::unique_ptr<SettingsComponent> settings_;
};
```

**Key Methods:**
```cpp
void resized() override;
bool isInterestedInFileDrag(const StringArray&) override;
void filesDropped(const StringArray&, int, int) override;
int getSidebarWidth();
```

---

### MidiDeviceComponent

**File:** `MidiDeviceComponent.h/cpp`  
**Inherits:** `Component`, `FileDragAndDropTarget`, `MidiInputCallback`  
**Purpose:** Visualize MIDI activity for a single device

**Lines of Code:** ~1,500 (largest component)

**Key Responsibilities:**
- Render MIDI channels, notes, CC, pitch bend, etc.
- Process incoming MIDI messages
- Maintain MIDI state (`ActiveChannels`)
- Graph/line visualization modes
- DPI-aware rendering
- Auto-hide inactive channels

**Pimpl Contents:**
```cpp
struct Pimpl : public MidiInputCallback
{
    MidiDeviceComponent* owner_;
    SettingsManager* settingsManager_;
    Theme& theme_;
    MidiDeviceInfo deviceInfo_;
    
    ActiveChannels channels_;  // Full MIDI state
    std::unique_ptr<MidiInput> midiIn_;
    
    bool paused_ = false;
    bool dirty_ = true;
    bool graphMode_ = false;
    
    void handleIncomingMidiMessage(MidiInput*, const MidiMessage&) override;
    void render();  // Main rendering logic
};
```

**Rendering Pipeline:**
```cpp
paint(Graphics& g)
    └─> render()
        ├─> renderPortName()
        ├─> renderClock()
        ├─> renderSysEx()
        └─> for each channel:
            ├─> renderChannelHeader()
            ├─> renderNotes()
            ├─> renderCC()
            ├─> renderPitchBend()
            ├─> renderProgramChange()
            └─> renderParameters(RPN/NRPN/HRCC)
```

**MIDI Message Processing:**
```cpp
handleIncomingMidiMessage(const MidiMessage& msg)
    ├─> SysEx → channels_.sysex_
    ├─> Clock → calculate BPM → channels_.clock_
    ├─> Note On/Off → channels_.channel_[n].notes_
    ├─> CC → channels_.channel_[n].controlChanges_
    │         or hrccs_ (14-bit)
    │         or rpns_ (RPN handling)
    │         or nrpns_ (NRPN handling)
    ├─> Pitch Bend → channels_.channel_[n].pitchBend_
    ├─> Program Change → channels_.channel_[n].programChange_
    └─> Channel Pressure → channels_.channel_[n].channelPressure_
```

**Auto-Hide Logic:**
- Channels fade after 5 seconds of inactivity
- `Time::getCurrentTime() - channel.time_ > 5.0`

---

### SidebarComponent

**File:** `SidebarComponent.h/cpp`  
**Inherits:** `Component`  
**Purpose:** Expandable/collapsible sidebar with controls

**Key Responsibilities:**
- Device port list (standalone only)
- Expand/collapse button
- Settings button
- Play/pause button
- Visualization mode toggle (line/graph)
- Reset button
- Help button

**Layout States:**
- **Collapsed:** 36px wide
- **Expanded:** 210px wide (122px on iOS portrait)

**Button Positions:** Defined in `LayoutConstants.h`

**Key Methods:**
```cpp
void setExpanded(bool);
bool isExpanded();
void paint(Graphics&) override;
void resized() override;
```

---

### SettingsComponent

**File:** `SettingsComponent.h/cpp`  
**Inherits:** `Component`  
**Purpose:** Theme editor and settings modal

**Key Responsibilities:**
- Color pickers for 9 theme colors
- Random theme generation
- Reset to default theme
- Import/export theme buttons
- Close button

**Theme Colors:**
1. Background
2. Sidebar
3. Separator
4. Track
5. Label
6. Data
7. Positive (blue)
8. Negative (red)
9. Controller (orange)

**Key Methods:**
```cpp
void paint(Graphics&) override;
void resized() override;
void randomizeTheme();
void resetTheme();
void importTheme();
void exportTheme();
```

---

### PortListComponent

**File:** `PortListComponent.h/cpp`  
**Inherits:** `Component`  
**Purpose:** List of MIDI ports with visibility toggles (standalone only)

**Key Responsibilities:**
- Display available MIDI devices
- Toggle device visibility (checkbox)
- Auto-update when devices connect/disconnect
- Alt+click to show/hide all

**Key Methods:**
```cpp
void refresh();  // Update device list
void paint(Graphics&) override;
void mouseDown(const MouseEvent&) override;
```

---

### AboutComponent

**File:** `AboutComponent.h/cpp`  
**Inherits:** `Component`  
**Purpose:** About dialog with app info

**Key Responsibilities:**
- Display version, copyright, license
- Credits to authors
- Links to website

---

### PaintedButton

**File:** `PaintedButton.h/cpp`  
**Inherits:** `Component`  
**Purpose:** Custom-drawn button with touch outsets

**Key Responsibilities:**
- Render button icon
- Handle mouse/touch interaction
- Provide visual feedback (hover, pressed)
- Touch target expansion (12px outset)

**Key Methods:**
```cpp
void setImage(const Image&);
void setTouchOutset(int);
std::function<void()> onClick;
```

---

### PopupColourSelector

**File:** `PopupColourSelector.h/cpp`  
**Inherits:** `Component`  
**Purpose:** Color picker popup for theme editor

**Key Responsibilities:**
- HSB color selection
- Alpha channel support
- Hex color input
- Live preview

---

### UwynLookAndFeel

**File:** `UwynLookAndFeel.h/cpp`  
**Inherits:** `LookAndFeel_V4`  
**Purpose:** Custom JUCE look and feel

**Key Responsibilities:**
- Custom scrollbar appearance
- Custom button rendering
- Custom slider appearance
- Font management

---

## MIDI Processing Components

### DeviceListener

**File:** `DeviceListener.h/cpp`  
**Inherits:** `MidiInputCallback`  
**Purpose:** Listen to a specific MIDI device

**Key Responsibilities:**
- Register with JUCE `MidiInput`
- Forward MIDI messages to `MidiDeviceComponent`
- Handle device disconnect

**Key Methods:**
```cpp
void handleIncomingMidiMessage(MidiInput*, const MidiMessage&) override;
void setTarget(MidiDeviceComponent*);
```

---

### MidiDevicesListener

**File:** `MidiDevicesListener.h/cpp`  
**Purpose:** Manage multiple device listeners

**Key Responsibilities:**
- Track active MIDI devices
- Create/destroy `DeviceListener` instances
- Notify when devices connect/disconnect

**Key Methods:**
```cpp
void addListener(DeviceListener*);
void removeListener(DeviceListener*);
void refreshDevices();
```

---

### DeviceManager

**File:** `DeviceManager.h`  
**Purpose:** Interface for device management

**Key Methods:**
```cpp
virtual Array<MidiDeviceInfo> getDeviceList() = 0;
virtual bool isDeviceVisible(const String& id) = 0;
virtual void setDeviceVisible(const String& id, bool visible) = 0;
```

**Implementations:**
- `StandaloneDevicesComponent` (standalone)
- (Not used in plugin - DAW provides MIDI)

---

## Infrastructure Components

### Theme

**File:** `Theme.h/cpp`  
**Purpose:** Theme color management

**Data Structure:**
```cpp
struct Theme
{
    Colour colorBackground;
    Colour colorSidebar;
    Colour colorSeperator;
    Colour colorTrack;
    Colour colorLabel;
    Colour colorData;
    Colour colorPositive;    // Blue
    Colour colorNegative;    // Red
    Colour colorController;  // Orange
};
```

**Key Methods:**
```cpp
static Theme getDefault();
String generateXml();  // Export to SVG
void parseXml(const String&);  // Import from SVG
void randomize();
void reset();

Font fontLabel();
Font fontData();
int lineHeight();
int linePosition(float);
```

**Built-in Themes:**
- `THEME_DARK` (default)
- `THEME_LIGHT`

---

### DpiScaling

**File:** `DpiScaling.h` (header-only)  
**Namespace:** `sm`  
**Purpose:** DPI-aware scaling utilities

**Key Functions:**
```cpp
float dpiScale() noexcept;
float dpiScale(const Component& c) noexcept;

template <typename Numeric>
Numeric scaled(Numeric value) noexcept;

template <typename Numeric>
Numeric scaled(Numeric value, const Component& c) noexcept;

Rectangle<int> scaled(Rectangle<int> r) noexcept;
Rectangle<int> scaled(Rectangle<int> r, const Component& c) noexcept;

int getStandardWidth() noexcept;
int getStandardWidth(const Component& c) noexcept;
```

**Usage Example:**
```cpp
// Bad: Hard-coded pixels
g.setFont(14.0f);
button.setBounds(10, 20, 100, 30);

// Good: DPI-aware
g.setFont(sm::scaled(14.0f));
button.setBounds(sm::scaled(10), sm::scaled(20),
                 sm::scaled(100), sm::scaled(30));
```

---

### LayoutConstants

**File:** `LayoutConstants.h` (header-only)  
**Namespace:** `showmidi::layout`  
**Purpose:** Centralized UI dimension constants

**Categories:**
- Main layout dimensions
- Typography
- Sidebar layout
- Port list layout
- Button layout
- Scrollbar layout
- MIDI device layout
- Popup windows
- Settings component layout
- Separator elements
- MIDI display positions (port, clock, sysex, channel, etc.)

**Example Constants:**
```cpp
static constexpr int STANDARD_WIDTH = 150;
static constexpr int SIDEBAR_EXPANDED_WIDTH = 210;
static constexpr int FONT_SIZE = 9;
static constexpr int LINE_HEIGHT_NORMAL = 22;
static constexpr int X_NOTE = 48;
static constexpr int X_CC = 177;
```

**Accessible via:**
```cpp
using namespace showmidi::layout;
// or
sm::layout::STANDARD_WIDTH
```

---

### Settings

**File:** `Settings.h`  
**Purpose:** Settings interface

**Key Methods:**
```cpp
virtual Theme& getTheme() = 0;
virtual void setTheme(const Theme&) = 0;
virtual bool getGraphMode() = 0;
virtual void setGraphMode(bool) = 0;
```

**Implementations:**
- `PropertiesSettings` (standalone - XML file)
- `PluginSettings` (plugin - MemoryBlock)

---

### PropertiesSettings

**File:** `PropertiesSettings.h/cpp`  
**Inherits:** `Settings`  
**Purpose:** File-based settings for standalone

**Storage Location:**
- macOS: `~/Library/Application Support/ShowMIDI/settings.xml`
- Linux: `~/.config/ShowMIDI/settings.xml`
- Windows: `%APPDATA%\ShowMIDI\settings.xml`

**Key Methods:**
```cpp
void load();
void save();
Theme& getTheme() override;
void setTheme(const Theme&) override;
```

---

### PluginSettings

**File:** `PluginSettings.h/cpp`  
**Inherits:** `Settings`  
**Purpose:** MemoryBlock-based settings for plugin

**Key Methods:**
```cpp
void serialize(MemoryBlock& dest);
void deserialize(const void* data, int size);
Theme& getTheme() override;
void setTheme(const Theme&) override;
```

---

### SettingsManager

**File:** `SettingsManager.h`  
**Purpose:** Interface for settings management

**Key Methods:**
```cpp
virtual bool isPlugin() = 0;
virtual Component* getTopLevelComponent() = 0;
virtual Settings& getSettings() = 0;
virtual void applySettings() = 0;
virtual void storeSettings() = 0;
virtual MidiDevicesListeners& getMidiDevicesListeners() = 0;
```

**Implementations:**
- `ShowMidiApplication` (standalone)
- `PluginSettings` (plugin)

---

## Data Structures

### ChannelState.h

**Purpose:** Complete MIDI state model

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
    std::vector<TimedValue> history_;  // For graph mode
};
```

#### Notes
```cpp
struct Notes
{
    NoteOn noteOn_[128];    // 0-127
    NoteOff noteOff_[128];  // 0-127
    Time time_;
};
```

#### NoteOn
```cpp
struct NoteOn : public ChannelMessage
{
    int number_;
    ChannelMessage polyPressure_;  // Aftertouch
};
```

#### NoteOff
```cpp
struct NoteOff : public ChannelMessage
{
    int number_;
};
```

#### ControlChanges
```cpp
struct ControlChanges
{
    ControlChange controlChange_[128];  // 0-127
    Time time_;
};
```

#### ControlChange
```cpp
struct ControlChange : public ChannelMessage
{
    int number_;
};
```

#### ProgramChange
```cpp
struct ProgramChange : public ChannelMessage
{
    // value_ is program number (0-127)
};
```

#### ChannelPressure
```cpp
struct ChannelPressure : public ChannelMessage
{
    // value_ is pressure (0-127)
};
```

#### PitchBend
```cpp
struct PitchBend : public ChannelMessage
{
    // value_ is -8192 to +8191
};
```

#### Parameters
```cpp
struct Parameters
{
    std::map<int, Parameter> param_;  // Keyed by parameter number
    Time time_;
};
```

**Used for:**
- HRCCs (Hi-Res CC) - 14-bit controllers
- RPNs (Registered Parameter Numbers)
- NRPNs (Non-Registered Parameter Numbers)

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

**BPM Calculation:**
- Queue MIDI clock timestamps
- Filter outliers (1% deviation)
- Average intervals between clocks
- Formula: `BPM = 600 / (avg_interval * 24)`

#### Sysex
```cpp
struct Sysex
{
    static constexpr int MAX_SYSEX_DATA = 20;
    uint8 data_[MAX_SYSEX_DATA];
    int length_;
    Time time_;
};
```

#### ActiveChannel
```cpp
struct ActiveChannel
{
    int number_;  // 0-15
    Time time_;
    
    Notes notes_;
    ControlChanges controlChanges_;
    ProgramChange programChange_;
    ChannelPressure channelPressure_;
    PitchBend pitchBend_;
    
    Parameters hrccs_;   // 14-bit CCs
    Parameters rpns_;
    Parameters nrpns_;
    
    // MPE support
    bool mpeManager_;
    MpeMember mpeMember_;  // mpeNone, mpeLower, mpeUpper
    
    // Internal state for RPN/NRPN
    int lastRpnMsb_;
    int lastRpnLsb_;
    int lastNrpnMsb_;
    int lastNrpnLsb_;
};
```

#### ActiveChannels
```cpp
struct ActiveChannels
{
    ActiveChannel channel_[16];  // MIDI channels 1-16
    Sysex sysex_;
    Clock clock_;
    
    void handleMpeActivation(Time t, ActiveChannel& channel, int range);
};
```

---

## Utilities

### MidiDeviceInfoComparator

**File:** `MidiDeviceInfoComparator.h`  
**Purpose:** Sort MIDI devices alphabetically

```cpp
struct MidiDeviceInfoComparator
{
    int compareElements(const MidiDeviceInfo& first,
                       const MidiDeviceInfo& second) const;
};
```

---

### DetectDevice

**File:** `DetectDevice.h/mm`  
**Purpose:** Platform-specific device detection (macOS)

**Objective-C++ Integration:**
- Uses CoreMIDI to detect device types
- Distinguishes input/output ports
- Provides device metadata

---

## Summary

### Component Count by Category

| Category            | Count | Key Components |
|---------------------|-------|----------------|
| Application         | 4     | ShowMidiApplication, StandaloneWindow, PluginProcessor, PluginEditor |
| UI Components       | 8     | MainLayout, MidiDevice, Sidebar, Settings, PortList, About, PaintedButton, PopupColourSelector |
| MIDI Processing     | 3     | DeviceListener, MidiDevicesListener, DeviceManager |
| Infrastructure      | 6     | Theme, DpiScaling, LayoutConstants, Settings, SettingsManager, UwynLookAndFeel |
| Data Structures     | 15    | ChannelState.h (all structs) |
| Utilities           | 2     | MidiDeviceInfoComparator, DetectDevice |
| **Total**           | **38**|  |

### Lines of Code by Component

| Component             | Approx. LOC | Complexity |
|-----------------------|-------------|------------|
| MidiDeviceComponent   | 1,500       | High       |
| ChannelState.h        | 514         | Medium     |
| MainLayoutComponent   | 400         | Medium     |
| SidebarComponent      | 300         | Low        |
| SettingsComponent     | 300         | Low        |
| Theme                 | 200         | Low        |
| Others                | ~4,000      | Varies     |

---

**Generated by:** Component Reference Generator  
**Repository:** https://github.com/peternicholls/ShowMIDI-Fork  
**Last Updated:** November 9, 2025
