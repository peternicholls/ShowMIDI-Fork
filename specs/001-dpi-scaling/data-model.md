# Data Model: DPI-Aware UI Scaling

**Date**: 2025-11-07 | **Feature**: 001-dpi-scaling

## Overview

The DPI-aware UI scaling feature is primarily a **presentation layer concern** with minimal data model requirements. The core "data" consists of compile-time layout constants and runtime DPI detection results. This document defines the entities, their relationships, and validation rules.

## Entities

### 1. DPI Scale Factor (Runtime)

**Description**: The scale factor representing the ratio between the current display's DPI and the baseline 96 DPI.

**Properties**:
| Property | Type | Description | Validation |
|----------|------|-------------|------------|
| `scale` | `float` | DPI scale factor (1.0 = 96 DPI, 2.0 = 192 DPI) | `0.5 <= scale <= 4.0` |
| `display` | `juce::Display*` | JUCE Display object (source of scale factor) | Not null except in headless context |
| `componentBounds` | `juce::Rectangle<int>` | Component's screen bounds (for multi-monitor) | Valid rectangle |

**Lifecycle**:
- **Created**: When component queries DPI via `dpiScale(component)`
- **Updated**: When component moves between monitors (parentHierarchyChanged)
- **Destroyed**: Ephemeral (not persisted, recalculated on demand)

**State Transitions**:
```
[Component Rendered] → [Query Display] → [Calculate Scale] → [Apply to Layout]
                              ↓
                     [Display Not Found] → [Fallback to 1.0]
```

**Validation Rules**:
- Scale factor must be positive (enforced by JUCE Display API)
- Scale factor outside 0.5-4.0 range should log warning (extreme DPI settings)
- Null display pointer → fallback to 1.0× scale (headless rendering)

### 2. Layout Constants (Compile-Time)

**Description**: Centralized dimensional values representing the logical (96 DPI) pixel dimensions for all UI elements.

**Namespace**: `sm` (showmidi scaling utilities)

**Properties** (50+ constants organized by functional area):

#### Standard Dimensions
| Constant | Type | Value | Description |
|----------|------|-------|-------------|
| `STANDARD_WIDTH` | `int` | 130 | Base width for MIDI device component |

#### Separators
| Constant | Type | Value | Description |
|----------|------|-------|-------------|
| `WIDTH_SEPARATOR` | `int` | 2 | Width of vertical separator lines |

#### Device Layout
| Constant | Type | Value | Description |
|----------|------|-------|-------------|
| `WIDTH_DEVICE_LEFT` | `int` | 52 | Left section width |
| `WIDTH_DEVICE_MIDDLE` | `int` | 23 | Middle section width |
| `HEIGHT_PORTS` | `int` | 20 | Height of port indicators |

#### MIDI Message Display
| Constant | Type | Value | Description |
|----------|------|-------|-------------|
| `HEIGHT_CHANNEL` | `int` | 28 | Height of channel display row |
| `HEIGHT_PROGRAM_CHANGE` | `int` | 22 | Height of program change display |
| `HEIGHT_PITCH_BEND` | `int` | 22 | Height of pitch bend display |
| `NOTE_SIZE` | `int` | 4 | Size of note indicator squares |

#### Typography
| Constant | Type | Value | Description |
|----------|------|-------|-------------|
| `FONT_SIZE_PORTS` | `float` | 12.0 | Font size for port labels |
| `FONT_SIZE_CHANNEL` | `float` | 14.0 | Font size for channel numbers |
| `FONT_SIZE_DATA` | `float` | 11.0 | Font size for MIDI data values |

*(Full list in `Source/layout/Constants.h` - 50+ constants)*

**Lifecycle**:
- **Created**: Compile-time (constexpr)
- **Updated**: Never (immutable constants)
- **Destroyed**: N/A (no runtime instance)

**Validation Rules**:
- All constants must be positive integers or floats
- Constants representing widths/heights must be > 0
- Font sizes must be >= 8.0 (minimum readable size)
- Total component width = sum of constituent widths (arithmetic consistency)

**Relationships**:
```
STANDARD_WIDTH = WIDTH_DEVICE_LEFT + WIDTH_DEVICE_MIDDLE + WIDTH_SEPARATOR + ...
```

### 3. Component Dimensions (Runtime)

**Description**: Calculated pixel dimensions for UI components after applying DPI scaling to layout constants.

**Properties**:
| Property | Type | Description | Validation |
|----------|------|-------------|------------|
| `logicalValue` | `int/float` | Original constant from Constants.h | > 0 |
| `scaleFactor` | `float` | DPI scale from Display API | 0.5-4.0 |
| `scaledValue` | `int/float` | Result of `logicalValue * scaleFactor` | >= logicalValue * 0.5 |

**Lifecycle**:
- **Created**: During component layout calculation (e.g., `resized()` callback)
- **Updated**: When DPI changes (monitor switch) or component resized
- **Destroyed**: Ephemeral (recalculated every layout pass)

**State Transitions**:
```
[Layout Constants] + [DPI Scale] → [sm::scaled()] → [Scaled Dimensions] → [Component Bounds]
```

**Validation Rules**:
- Scaled value must maintain aspect ratios (proportional scaling)
- Minimum dimension after scaling: 1 pixel (avoid zero-size components)
- Integer dimensions use truncation (no rounding to avoid cumulative errors)

## Relationships

### Dependency Graph

```
┌─────────────────────┐
│ JUCE Display API    │ (Platform-specific DPI detection)
└──────────┬──────────┘
           │ provides
           ↓
┌─────────────────────┐
│ DPI Scale Factor    │ (Runtime: 1.0x, 1.5x, 2.0x, etc.)
└──────────┬──────────┘
           │
           │ multiplies
           ↓
┌─────────────────────┐         ┌─────────────────────┐
│ Layout Constants    │ ----->  │ sm::scaled()        │
│ (Compile-time: 130) │  input  │ (Template function) │
└─────────────────────┘         └──────────┬──────────┘
                                           │ produces
                                           ↓
                                ┌─────────────────────┐
                                │ Component Dimensions│
                                │ (Runtime: 260 @ 2x) │
                                └──────────┬──────────┘
                                           │ used by
                                           ↓
                                ┌─────────────────────┐
                                │ JUCE Component      │
                                │ setBounds()         │
                                └─────────────────────┘
```

### Entity Relationships

**1-to-many**: One `Display` provides scale factor for many `Component` instances

**many-to-1**: Many `Component` instances use same `Layout Constants`

**1-to-1**: Each `Component` has one active `DPI Scale Factor` at a time (but may change on monitor switch)

## Data Flow

### Scenario 1: Component Initial Layout

```
1. Component::resized() called by JUCE
2. Component queries: sm::dpiScale(*this)
   a. JUCE Desktop::getDisplays() → list of displays
   b. Find display containing component bounds
   c. Return display->scale (e.g., 2.0 for Retina)
3. Component calculates: sm::scaled(STANDARD_WIDTH) = 130 * 2.0 = 260
4. Component calls: setBounds(x, y, 260, height)
```

### Scenario 2: Multi-Monitor DPI Change

```
1. User drags window from 96 DPI display to 192 DPI display
2. JUCE fires: Component::parentHierarchyChanged()
3. Component re-queries: sm::dpiScale(*this) → now returns 2.0 (was 1.0)
4. Component triggers: resized() → recalculate all dimensions
5. Component updates: setBounds() with new scaled dimensions
```

### Scenario 3: Font Rendering

```
1. Component needs to render text label
2. Component calculates: sm::scaled(FONT_SIZE_CHANNEL) = 14.0 * 2.0 = 28.0
3. Component creates: Font("JetBrains Mono", 28.0, Font::plain)
4. JUCE renders font at 28pt (crisp on Retina display)
```

## Validation Rules Summary

### DPI Scale Factor

| Rule | Validation | Error Handling |
|------|------------|----------------|
| Scale must be positive | `scale > 0` | Log error, fallback to 1.0 |
| Scale in expected range | `0.5 <= scale <= 4.0` | Log warning if outside (but allow) |
| Display must exist | `display != nullptr` | Fallback to 1.0 (headless context) |

### Layout Constants

| Rule | Validation | Error Handling |
|------|------------|----------------|
| Constants positive | `value > 0` | Compile error (static_assert in future) |
| Font sizes readable | `fontSize >= 8.0` | Compile error (static_assert) |
| Width consistency | `sum(parts) == STANDARD_WIDTH` | Manual audit (no automated check) |

### Component Dimensions

| Rule | Validation | Error Handling |
|------|------------|----------------|
| No zero dimensions | `scaledValue >= 1` | Enforce minimum 1px |
| Integer truncation | `static_cast<int>(value * scale)` | Accept truncation (no rounding) |
| Bounds validity | `width > 0 && height > 0` | JUCE Component assertion |

## Edge Case Handling

### Edge Case 1: Extremely High DPI (384 DPI / 4×)

**Scenario**: User has 4K display at 384 DPI (4× scaling)

**Validation**: Scale factor 4.0 is within 0.5-4.0 range → valid

**Handling**: All components scale to 4× size normally

**Risk**: Very large components may exceed screen size (e.g., 130 * 4 = 520 pixels for standard width). This is acceptable; user can scroll if needed.

### Edge Case 2: Fractional DPI (125% = 1.25×)

**Scenario**: Windows user sets DPI to 125% (120 DPI)

**Validation**: Scale factor 1.25 is within range → valid

**Handling**: `sm::scaled<int>(130)` = `static_cast<int>(130 * 1.25)` = 162 pixels

**Risk**: Sub-pixel rounding may cause slight visual misalignment. Mitigated by using integer dimensions (truncation is consistent).

### Edge Case 3: Headless Rendering (CI/CD, Automated Testing)

**Scenario**: Component rendered without attached display

**Validation**: `display == nullptr` → fallback to 1.0

**Handling**: All components render at 96 DPI baseline

**Risk**: None (headless context doesn't care about DPI)

### Edge Case 4: Zero or Negative Input (Defensive Programming)

**Scenario**: Malicious/buggy code calls `sm::scaled(-1)` or `sm::scaled(0)`

**Validation**: Template performs multiplication; sign/zero preserved

**Handling**: 
- `sm::scaled(0) = 0 * scale = 0` (valid for offsets)
- `sm::scaled(-1) = -1 * scale = -2.0` (valid for relative positioning)

**Risk**: None (mathematical correctness maintained)

## Data Persistence

**None required.** All DPI-related data is ephemeral:

- **DPI Scale Factor**: Queried on-demand from OS via JUCE Display API
- **Layout Constants**: Compile-time constants (embedded in binary)
- **Component Dimensions**: Calculated during layout pass (not persisted)

**Future Consideration**: If user override for DPI scale is added, persist in SettingsManager:
```cpp
settings->setValue("ui.dpiScaleOverride", 1.5f); // Future enhancement
```

## API Contracts

See `contracts/scaling-api.md` for detailed function signatures and behavior specifications.

## Conclusion

The DPI-aware UI scaling feature has a minimal data model consisting of:
1. **Runtime DPI scale factor** (queried from JUCE Display API)
2. **Compile-time layout constants** (centralized in Constants.h)
3. **Ephemeral scaled dimensions** (calculated during layout)

No persistence layer, no complex state management, no user configuration (automatic detection). The simplicity of this data model aligns with the constitution's Maintainability principle and ensures zero runtime overhead.
