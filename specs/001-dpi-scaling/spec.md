# Feature Specification: DPI-Aware UI Scaling

**Feature Branch**: `001-dpi-scaling`  
**Created**: 2025-11-07  
**Updated**: 2025-11-07  
**Status**: In Progress  
**Input**: Implement comprehensive DPI-aware scaling across all UI components for consistent appearance on high-resolution displays and varied screen densities

## Implementation Status

**Groundwork Completed**: Significant infrastructure and partial component updates have been completed (2 commits, ~5,400 lines changed).

### ✅ Completed Infrastructure
- `Source/utility/Scaling.h` - Complete DPI scaling API
- `Source/layout/Constants.h` - All layout constants centralized (206 lines)
- `Source/MidiDeviceComponent.cpp` - Fully DPI-aware implementation (reference implementation)

### 🟡 Partially Completed
- `Source/MainLayoutComponent.cpp` - Structural changes, needs scaling audit
- `Source/StandaloneDevicesComponent.cpp` - Structural changes, needs scaling audit  
- `Source/SettingsComponent.cpp` - Reformatted, needs complete scaling audit

### ⚠️ Needs Work
- `Source/PluginEditor.cpp` - Structural changes but no `sm::scaled()` usage detected
- `Source/AboutComponent.cpp` - Minimal changes only
- `Source/SidebarComponent.cpp` - Not yet started
- `Source/PortListComponent.cpp` - Not yet started
- `Source/PaintedButton.cpp` - Not yet started
- `Source/UwynLookAndFeel.cpp` - Not yet started

### 📋 Remaining Tasks
1. Complete component DPI audit (6 components)
2. Font size audit across all components
3. Theme asset verification at multiple DPI levels
4. Multi-monitor and plugin host testing
5. Developer documentation with code examples

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Consistent Appearance on High-DPI Displays (Priority: P1)

Musicians using ShowMIDI on high-resolution displays (e.g., 4K monitors, Retina displays, high-DPI Windows laptops) need all UI elements to appear at the correct physical size, not scaled inappropriately by the OS or appearing pixelated.

**Why this priority**: This is the core value proposition - without proper DPI scaling, the application is unusable on modern high-resolution displays which are increasingly common in professional music production environments.

**Independent Test**: Can be fully tested by opening ShowMIDI on displays with different scale factors (100%, 125%, 150%, 200%) and verifying that all UI elements maintain consistent physical dimensions and remain readable.

**Acceptance Scenarios**:

1. **Given** ShowMIDI is launched on a 4K display (200% scaling), **When** viewing MIDI device columns, **Then** all text, buttons, and channel indicators appear at the same physical size as on a standard 1080p display (100% scaling)
2. **Given** ShowMIDI is running on a standard resolution display, **When** user moves the window to a high-DPI external monitor, **Then** the UI automatically rescales without restart and maintains visual consistency
3. **Given** a user has a 150% scaled Windows laptop display, **When** they launch ShowMIDI plugin in their DAW, **Then** all UI elements scale proportionally with crisp rendering and no pixelation
4. **Given** ShowMIDI is running on any display, **When** user toggles between line and graph visualization modes, **Then** all visual elements in both modes scale correctly for the current DPI

---

### User Story 2 - Cross-Platform DPI Consistency (Priority: P2)

Musicians working across different platforms (macOS, Windows, Linux) need ShowMIDI to provide a consistent visual experience regardless of operating system DPI handling differences.

**Why this priority**: Professional musicians often use multiple systems, and visual consistency reduces cognitive load when switching between setups during live performance or studio work.

**Independent Test**: Can be tested by running ShowMIDI on macOS (Retina and non-Retina), Windows (various DPI settings 100%-200%), and Linux (multiple DPI configurations) and comparing screenshots scaled to physical dimensions.

**Acceptance Scenarios**:

1. **Given** ShowMIDI is installed on both a MacBook Pro (Retina) and a Windows laptop (150% scaling), **When** both display MIDI from the same controller, **Then** the channel information, fonts, and controls appear at equivalent physical sizes
2. **Given** a Linux user with a mixed-DPI multi-monitor setup, **When** ShowMIDI window spans two monitors with different scales, **Then** each portion of the window scales appropriately for its respective monitor
3. **Given** the standalone application is running on any platform, **When** user interacts with theme customization, **Then** theme previews and color selectors scale correctly for the current system DPI

---

### User Story 3 - Plugin Host DPI Integration (Priority: P2)

DAW users need ShowMIDI to respect their plugin host's DPI scaling settings and integrate seamlessly with the host's UI scaling approach.

**Why this priority**: Plugin windows must match the host DAW's scaling to maintain visual cohesion and prevent UI elements from appearing out of proportion.

**Independent Test**: Can be tested by loading ShowMIDI as VST/VST3/AU/AUv3 in various DAWs with different scaling settings and verifying consistent appearance with host UI.

**Acceptance Scenarios**:

1. **Given** ShowMIDI plugin is loaded in Ableton Live on a 4K display, **When** the plugin window opens, **Then** the UI scales to match Ableton's scaling factor
2. **Given** ShowMIDI plugin is running in Logic Pro on a Retina display, **When** user resizes the plugin window, **Then** all elements rescale proportionally without quality loss
3. **Given** ShowMIDI AUv3 is loaded on an iPad, **When** running on different iPad models (standard and Pro with different screen densities), **Then** the UI adapts to the appropriate scale for each device

---

### User Story 4 - Developer-Friendly Scaling API (Priority: P3)

Developers contributing to ShowMIDI need a simple, consistent API for applying DPI scaling to new UI components without requiring deep understanding of platform-specific DPI handling.

**Why this priority**: Maintainability and contribution velocity depend on having clear, easy-to-use scaling utilities that prevent accidental introduction of non-scaled UI elements.

**Independent Test**: Can be tested by having a developer add a new UI component (e.g., a settings panel) using only the documented scaling utilities and verifying it scales correctly across all target platforms and DPI settings.

**Acceptance Scenarios**:

1. **Given** a developer needs to create a new button with 32px dimensions, **When** they use `sm::scaled(32)` in their component code, **Then** the button appears at 32 logical pixels on all displays regardless of DPI
2. **Given** a developer needs font sizing for channel labels, **When** they use `sm::scaled(14.0f)` for font point size, **Then** text remains readable and consistently sized across all platforms and DPI settings
3. **Given** a developer adds a new rectangular UI element, **When** they use `sm::scaled(Rectangle<int>(x, y, w, h))`, **Then** the rectangle scales correctly on all displays

---

### Edge Cases

- What happens when ShowMIDI runs on a display with non-standard DPI (e.g., 125%, 175%)?
- How does the system handle rapid display changes (e.g., user frequently moves window between monitors with different DPI)?
- What happens when system DPI changes while ShowMIDI is running (Windows 10+ dynamic DPI change)?
- How does the system handle plugin hosts that don't properly report their scaling factor?
- What happens on platforms with per-monitor DPI awareness when window spans multiple monitors?
- How are theme assets (SVG) scaled to maintain visual quality at any DPI?

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: ✅ System MUST detect the current display's DPI scale factor (1.0x, 1.5x, 2.0x, etc.) on application startup
  - *Status: Implemented via `sm::dpiScale()` using JUCE's `Desktop::getInstance().getDisplays()`*
- **FR-002**: ✅ System MUST provide a `sm::scaled()` utility function that multiplies logical dimension values by the current DPI scale factor
  - *Status: Complete template implementation in `Source/utility/Scaling.h`*
- **FR-003**: ✅ System MUST support per-component DPI scaling for windows spanning multiple monitors with different DPI settings
  - *Status: Implemented via `sm::scaled(value, Component&)` and `sm::dpiScale(Component&)`*
- **FR-004**: 🟡 All font sizes MUST be scaled using `sm::scaled()` to maintain readability across different DPI settings
  - *Status: Needs audit - search for all `setFont()` calls and verify scaling*
- **FR-005**: 🟡 All dimensional values (widths, heights, margins, padding) MUST be specified in logical units and scaled via `sm::scaled()`
  - *Status: ~40% complete - MidiDeviceComponent done, 6+ components need work*
- **FR-006**: ✅ System MUST provide `sm::dpiScale()` function for manual calculations requiring the raw scale factor
  - *Status: Implemented with both display and component overloads*
- **FR-007**: ✅ System MUST provide convenience overloads for common types (int, float, double, Rectangle)
  - *Status: Complete - template supports all numeric types, Rectangle overloads provided*
- **FR-008**: ❓ System MUST update scaling factor dynamically when window moves between displays with different DPI
  - *Status: Component-aware API provided, needs multi-monitor testing*
- **FR-009**: ✅ System MUST maintain the existing `STANDARD_WIDTH` constant as a logical value and scale it via `sm::getStandardWidth()`
  - *Status: Implemented - constant moved to `layout/Constants.h`, accessed via `sm::getStandardWidth()`*
- **FR-010**: ❓ System MUST ensure theme SVG assets render crisply at any scale factor
  - *Status: Needs verification testing at 150%, 200% DPI*
- **FR-011**: ❓ Plugin versions (VST, VST3, AU, AUv3) MUST respect host-provided scaling hints
  - *Status: Needs testing in multiple DAWs*
- **FR-012**: ❓ System MUST maintain visual consistency between standalone and plugin versions at equivalent DPI settings
  - *Status: Needs side-by-side testing*
- **FR-013**: 🟡 All existing UI components (MidiDeviceComponent, MainLayoutComponent, SidebarComponent, SettingsComponent) MUST be audited and updated to use scaling utilities
  - *Status: 1 of 10+ components complete (MidiDeviceComponent), 3 partially updated, 6+ not started*
- **FR-014**: ❌ System MUST handle edge cases where display reports unusual scale factors (e.g., 1.25x, 1.75x) by scaling proportionally
  - *Status: Not yet tested - proportional scaling should work but needs validation*
- **FR-015**: ❌ Documentation MUST be provided for developers explaining how to use scaling utilities when adding new UI components
  - *Status: Not created - needs developer guide with 5+ code examples*

### Key Entities

- **Scaling Utility Namespace (sm)**: Provides DPI-aware scaling functions for all numeric types and geometric primitives; lives in `Source/utility/Scaling.h`
- **Display Scale Factor**: Platform-reported multiplier (float) representing the current display's DPI relative to standard 96 DPI baseline; ranges typically from 1.0 to 3.0
- **Logical Units**: Dimensions specified in code at the base 100% scale (96 DPI), which get multiplied by the scale factor at runtime
- **Physical Units**: Actual pixel dimensions rendered on screen after applying the scale factor
- **Component Context**: JUCE Component reference used to determine scale factor for multi-monitor scenarios where different parts of the window have different DPI

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: All text remains readable at font sizes between 10pt and 18pt across DPI settings from 100% to 300%
  - *Status: ❓ Needs testing - font scaling implementation incomplete*
- **SC-002**: UI elements maintain consistent physical dimensions (±5% tolerance) when measured with a ruler across different displays with scale factors from 1.0x to 2.0x
  - *Status: 🟡 Partially validated - MidiDeviceComponent expected to pass, other components need testing*
- **SC-003**: Window can be dragged between monitors with different DPI (e.g., 100% and 200%) without visible rendering artifacts or incorrect sizing
  - *Status: ❓ Needs testing - component-aware API supports this but requires validation*
- **SC-004**: ShowMIDI plugin integrates visually with host DAW UI in at least 3 major DAWs (Ableton Live, Logic Pro, Reaper) on high-DPI displays without appearing oversized or undersized
  - *Status: ❓ Needs testing - PluginEditor component needs work before testing*
- **SC-005**: Code review of all UI components confirms 100% of dimensional values use `sm::scaled()` or equivalent scaling utilities
  - *Status: ❌ ~40% complete - MidiDeviceComponent passes, 9+ components need audit*
- **SC-006**: Theme SVG rendering quality remains sharp (no pixelation) at 200% DPI when compared side-by-side with 100% DPI
  - *Status: ❓ Needs testing - vector SVGs should scale but requires verification*
- **SC-007**: Performance overhead of scaling calculations is unmeasurable in real-time MIDI visualization (maintains <10ms latency requirement from constitution)
  - *Status: ✅ Expected to pass - header-only inline functions have zero overhead after compilation*
- **SC-008**: Developer documentation includes at least 5 code examples demonstrating proper use of scaling utilities for common UI patterns
  - *Status: ❌ Not created - MidiDeviceComponent.cpp serves as reference but formal docs needed*

### Current Implementation Statistics

- **Total files changed**: 11
- **New files created**: 3 (Scaling.h, Constants.h, .vscode/settings.json)
- **Lines added**: +2,881
- **Lines removed**: -2,511
- **Commits**: 2
- **Components fully DPI-aware**: 1 of 10+ (10%)
- **API completeness**: 100% (all utilities implemented)
- **Component coverage**: ~40%

### Assumptions

- JUCE framework provides reliable `Desktop::getInstance().getDisplays()` API for querying display scale factors on all target platforms (macOS, Windows, Linux, iOS)
  - *Validated: API is implemented and working in `sm::dpiScale()`*
- Plugin hosts properly report their scaling preferences via standard plugin APIs (VST3 editor scaling, AU view scaling hints)
  - *Status: Needs validation testing*
- Primary use case is scale factors between 1.0x and 2.0x (100% to 200%), with occasional 1.5x and 3.0x scenarios
  - *Confirmed: Design accommodates arbitrary scale factors via proportional multiplication*
- SVG theme assets scale infinitely without quality loss (vector format advantage)
  - *Needs verification: Testing required at multiple DPI levels*
- Baseline reference DPI is 96 (standard Windows DPI at 100% scaling)
  - *Confirmed: Constants defined as logical units at 96 DPI baseline*
- JUCE's `Component::getApproximateScaleFactorForComponent()` provides accurate per-monitor DPI on multi-monitor setups
  - *Implemented: Used in `sm::dpiScale(Component&)` but needs multi-monitor testing*
- ShowMIDI window size and layout are not constrained by minimum platform window sizes after scaling
  - *Needs verification: Test on high DPI to ensure no layout issues*

## Implementation Notes

### Reference Implementation

`Source/MidiDeviceComponent.cpp` serves as the reference implementation demonstrating proper DPI scaling patterns:

```cpp
// Example 1: Basic scaling of constants
int getWidthSeparator() const { 
    return sm::getStandardWidth() - sm::scaled(WIDTH_SEPARATOR); 
}

// Example 2: Using layout constants with scaling
int clock_width = getStandardWidth() - X_PARAM - X_CLOCK_BPM;

// Example 3: Rectangle creation with scaled dimensions
Rectangle<int> area(
    sm::scaled(X_CLOCK), 
    sm::scaled(theme_.labelHeight()),
    sm::scaled(clock_width), 
    sm::scaled(HEIGHT_SEPARATOR)
);
```

### Known Issues

1. **TODO in MidiDeviceComponent.cpp**: "Refactor header underline drawing into a helper function!"
2. **PluginEditor.cpp**: No `sm::scaled()` usage detected despite 339 lines changed
3. **Font Scaling Incomplete**: Audit required for all `setFont()` calls

### Next Commit Recommendations

**Priority 1 - Critical Path**:
1. Update `PluginEditor.cpp` with complete DPI scaling
2. Complete font size audit and add `sm::scaled()` to all font calls
3. Update remaining 6 components (AboutComponent, SidebarComponent, etc.)

**Priority 2 - Validation**:
4. Test multi-monitor window dragging
5. Test plugin in 3 major DAWs on high-DPI displays
6. Verify SVG theme rendering at 150%, 200% DPI

**Priority 3 - Documentation**:
7. Create developer guide with 5+ code examples
8. Document edge cases and platform-specific behaviors
