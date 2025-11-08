# Scaling API Contract

**Version**: 1.0.0  
**Date**: 2025-11-07  
**Feature**: 001-dpi-scaling

## Overview

This document specifies the public API contract for ShowMIDI's DPI-aware scaling utilities. All functions are header-only inline functions defined in `Source/utility/Scaling.h`.

## Namespace

All scaling utilities are within the `sm` namespace (showmidi scaling).

## API Functions

### 1. `dpiScale()` - Get DPI Scale Factor

#### Signature

```cpp
float dpiScale(const juce::Component& component)
```

#### Description

Queries the display's DPI scale factor for the given component. Returns the ratio between the display's DPI and the baseline 96 DPI.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `component` | `const juce::Component&` | Reference to the component (used to determine which display to query) |

#### Return Value

| Type | Description | Range |
|------|-------------|-------|
| `float` | DPI scale factor | Typically 0.5-4.0 (extremes: 0.25-8.0) |

**Common values**:
- `1.0` = 96 DPI (standard desktop)
- `1.25` = 120 DPI (Windows 125%)
- `1.5` = 144 DPI (Windows 150%)
- `2.0` = 192 DPI (macOS Retina, Windows 200%)
- `3.0` = 288 DPI (high-DPI mobile/tablets)
- `4.0` = 384 DPI (4K displays at 200% scaling)

#### Behavior

1. Queries `juce::Desktop::getInstance().getDisplays()` to get all displays
2. Finds the display containing the component's screen bounds via `getDisplayForRect()`
3. Returns `display->scale` property
4. If no display found (headless rendering), returns `1.0` as fallback

#### Preconditions

- `component` must be a valid JUCE Component (but need not be visible or on-screen)

#### Postconditions

- Returns positive float value
- Never returns `NaN` or infinity
- Never throws exceptions

#### Error Handling

| Condition | Behavior |
|-----------|----------|
| No display detected | Returns `1.0f` (safe fallback) |
| Component off-screen | Uses component's last known bounds |
| Headless context | Returns `1.0f` |

#### Performance

- **Complexity**: O(n) where n = number of displays (typically 1-3)
- **Latency**: <0.1ms (display query is cached by OS)
- **Allocations**: Zero (uses JUCE's cached display list)

#### Example Usage

```cpp
void MyComponent::resized() {
    float scale = sm::dpiScale(*this);
    // scale = 2.0 on Retina display, 1.0 on standard display
    
    int scaledWidth = static_cast<int>(100 * scale);
    childComponent.setBounds(0, 0, scaledWidth, scaledWidth);
}
```

---

### 2. `scaled<T>(value)` - Scale Single Value

#### Signature

```cpp
template <typename T>
T scaled(T value)
```

#### Description

Scales a single numeric value by the global DPI scale factor. Uses the primary display's scale if multiple displays exist.

#### Template Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `T` | `typename` | Numeric type (int, float, double, long, etc.) |

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `T` | The logical (96 DPI) value to scale |

#### Return Value

| Type | Description |
|------|-------------|
| `T` | Scaled value: `value * dpiScale(primaryDisplay)` |

#### Behavior

1. Queries primary display's scale factor (first display in JUCE's display list)
2. Multiplies `value * scale`
3. Returns result as same type `T`

#### Type Behavior

| Input Type | Output Type | Rounding |
|------------|-------------|----------|
| `int` | `int` | Truncates (e.g., `100 * 1.25 = 125.0 → 125`) |
| `float` | `float` | Preserves fractional (e.g., `100.0 * 1.25 = 125.0`) |
| `double` | `double` | Preserves fractional (high precision) |

#### Preconditions

- `T` must be a numeric type (int, float, double, etc.)
- `value` can be positive, negative, or zero

#### Postconditions

- Returns `value * scale` with type `T`
- Sign preservation: negative inputs produce negative outputs
- Zero preservation: `scaled(0) == 0`

#### Performance

- **Complexity**: O(1) after display query cached
- **Latency**: <0.01ms (inline multiply)
- **Allocations**: Zero

#### Example Usage

```cpp
// Scale integer dimensions
int width = sm::scaled(130);      // 130 → 260 on 2× display
int height = sm::scaled(50);      // 50 → 100 on 2× display

// Scale floating-point font sizes
float fontSize = sm::scaled(14.0f); // 14.0 → 28.0 on 2× display

// Zero and negative values preserved
int offset = sm::scaled(0);      // Always 0
int margin = sm::scaled(-5);     // -5 → -10 on 2× display
```

---

### 3. `scaled<T>(value, component)` - Scale Value for Specific Component

#### Signature

```cpp
template <typename T>
T scaled(T value, const juce::Component& component)
```

#### Description

Scales a value by the DPI scale factor of the display containing the specified component. **Preferred for multi-monitor setups** where component may be on a display with different DPI than primary display.

#### Template Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `T` | `typename` | Numeric type (int, float, double, etc.) |

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `T` | The logical (96 DPI) value to scale |
| `component` | `const juce::Component&` | Reference to component (determines which display to query) |

#### Return Value

| Type | Description |
|------|-------------|
| `T` | Scaled value: `value * dpiScale(component)` |

#### Behavior

1. Calls `dpiScale(component)` to get scale for component's display
2. Multiplies `value * scale`
3. Returns result as type `T`

#### When to Use

| Scenario | Use This Overload |
|----------|-------------------|
| Single display setup | Optional (both overloads equivalent) |
| Multi-monitor with same DPI | Optional (both overloads equivalent) |
| Multi-monitor with different DPI | **Required** (ensures correct scale per display) |
| Component may move between displays | **Required** (dynamic scale updates) |

#### Example Usage

```cpp
void MyComponent::resized() {
    // Use component-specific scaling (correct on any display)
    int width = sm::scaled(130, *this);    // Scales per this component's display
    int height = sm::scaled(50, *this);
    
    setBounds(0, 0, width, height);
}

void MyComponent::parentHierarchyChanged() {
    Component::parentHierarchyChanged();
    
    // Component may have moved to different display - recalculate
    resized();
}
```

---

### 4. `scaled(rectangle)` - Scale Rectangle

#### Signature

```cpp
juce::Rectangle<int> scaled(juce::Rectangle<int> rect)
```

#### Description

Scales a JUCE Rectangle's position and dimensions by the global DPI scale factor.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `rect` | `juce::Rectangle<int>` | Rectangle with logical (96 DPI) coordinates |

#### Return Value

| Type | Description |
|------|-------------|
| `juce::Rectangle<int>` | Rectangle with scaled coordinates and dimensions |

#### Behavior

Scales all four components of the rectangle:
```cpp
return {
    scaled(rect.getX()),
    scaled(rect.getY()),
    scaled(rect.getWidth()),
    scaled(rect.getHeight())
};
```

#### Example Usage

```cpp
// Define logical layout (96 DPI)
juce::Rectangle<int> logicalBounds(10, 20, 100, 50);

// Scale to current DPI
juce::Rectangle<int> actualBounds = sm::scaled(logicalBounds);
// On 2× display: (20, 40, 200, 100)

childComponent.setBounds(actualBounds);
```

---

### 5. `scaled(rectangle, component)` - Scale Rectangle for Component

#### Signature

```cpp
juce::Rectangle<int> scaled(juce::Rectangle<int> rect, const juce::Component& component)
```

#### Description

Scales a Rectangle using the DPI scale factor of the display containing the component.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `rect` | `juce::Rectangle<int>` | Rectangle with logical coordinates |
| `component` | `const juce::Component&` | Component determining which display to query |

#### Return Value

| Type | Description |
|------|-------------|
| `juce::Rectangle<int>` | Rectangle scaled per component's display |

#### Example Usage

```cpp
void MyComponent::resized() {
    juce::Rectangle<int> logicalLayout(0, 0, 130, 200);
    juce::Rectangle<int> actualLayout = sm::scaled(logicalLayout, *this);
    
    childComponent.setBounds(actualLayout);
}
```

---

### 6. `scaled(point)` - Scale Point

#### Signature

```cpp
juce::Point<int> scaled(juce::Point<int> point)
```

#### Description

Scales a JUCE Point's coordinates by the global DPI scale factor.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `point` | `juce::Point<int>` | Point with logical (96 DPI) coordinates |

#### Return Value

| Type | Description |
|------|-------------|
| `juce::Point<int>` | Point with scaled coordinates |

#### Example Usage

```cpp
juce::Point<int> logicalPosition(50, 75);
juce::Point<int> actualPosition = sm::scaled(logicalPosition);
// On 2× display: (100, 150)
```

---

### 7. `scaled(line)` - Scale Line

#### Signature

```cpp
juce::Line<int> scaled(juce::Line<int> line)
```

#### Description

Scales a JUCE Line's start and end coordinates by the global DPI scale factor.

#### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `line` | `juce::Line<int>` | Line with logical (96 DPI) coordinates |

#### Return Value

| Type | Description |
|------|-------------|
| `juce::Line<int>` | Line with scaled coordinates |

#### Example Usage

```cpp
juce::Line<int> logicalLine(0, 0, 100, 100);
juce::Line<int> actualLine = sm::scaled(logicalLine);
// On 2× display: (0, 0) to (200, 200)
```

---

### 8. `getStandardWidth()` - Get Scaled Standard Width

#### Signature

```cpp
int getStandardWidth()
```

#### Description

Returns the scaled MIDI device standard width (130 logical pixels at 96 DPI).

#### Return Value

| Type | Description |
|------|-------------|
| `int` | Scaled standard width (130 × DPI scale) |

#### Behavior

Convenience function equivalent to:
```cpp
return scaled(sm::STANDARD_WIDTH);
```

#### Example Usage

```cpp
void MidiDeviceComponent::resized() {
    int totalWidth = sm::getStandardWidth();
    int separatorWidth = totalWidth - sm::scaled(WIDTH_SEPARATOR);
    
    // Use separatorWidth for layout...
}
```

---

## Constants API

All layout constants are defined in `Source/layout/Constants.h` within the `sm` namespace.

### Standard Dimensions

```cpp
namespace sm {
    static constexpr int STANDARD_WIDTH = 130;
}
```

### Component Dimensions

```cpp
namespace sm {
    static constexpr int WIDTH_SEPARATOR = 2;
    static constexpr int WIDTH_DEVICE_LEFT = 52;
    static constexpr int WIDTH_DEVICE_MIDDLE = 23;
    static constexpr int HEIGHT_PORTS = 20;
    static constexpr int HEIGHT_CHANNEL = 28;
    // ... 50+ more constants (see Constants.h for full list)
}
```

### Typography Constants

```cpp
namespace sm {
    static constexpr float FONT_SIZE_PORTS = 12.0f;
    static constexpr float FONT_SIZE_CHANNEL = 14.0f;
    static constexpr float FONT_SIZE_DATA = 11.0f;
}
```

### Usage Pattern

```cpp
#include "utility/Scaling.h"
#include "layout/Constants.h"

void MyComponent::resized() {
    using namespace sm;
    
    // Scale constants to actual pixels
    int width = scaled(STANDARD_WIDTH);
    int height = scaled(HEIGHT_CHANNEL);
    float fontSize = scaled(FONT_SIZE_CHANNEL);
    
    // Use scaled values for layout
    setBounds(0, 0, width, height);
    font.setHeight(fontSize);
}
```

---

## API Guarantees

### Thread Safety

| API | Thread-Safe? | Notes |
|-----|--------------|-------|
| `dpiScale()` | ✅ Yes | JUCE Display API is thread-safe |
| `scaled<T>()` | ✅ Yes | Pure function (no shared state) |
| All overloads | ✅ Yes | Read-only display queries |

### Exception Safety

| API | Throws? | Guarantee |
|-----|---------|-----------|
| All functions | ❌ No | `noexcept` (no exceptions thrown) |

### Performance Guarantees

| API | Time Complexity | Allocations |
|-----|-----------------|-------------|
| `dpiScale()` | O(n) displays | 0 |
| `scaled<T>()` | O(1) | 0 |
| All overloads | O(1) after initial query | 0 |

### Const-Correctness

All functions:
- Accept `const juce::Component&` (never modify component)
- Return by value (no mutable references)
- Are `const`-qualified where applicable

---

## Deprecation Policy

This is version 1.0.0 of the scaling API. Future changes will follow semantic versioning:

- **Major version**: Incompatible API changes (e.g., removing functions)
- **Minor version**: Adding new functions (backwards-compatible)
- **Patch version**: Bug fixes, documentation updates

Deprecated functions will be marked with:
```cpp
[[deprecated("Use scaled(value, component) instead")]]
float legacyScaleFactor();
```

And maintained for at least one major version before removal.

---

## Integration Examples

### Example 1: Simple Component Layout

```cpp
#include "utility/Scaling.h"
#include "layout/Constants.h"

class MyButton : public juce::Component {
public:
    void resized() override {
        using namespace sm;
        
        // Define logical layout
        constexpr int BUTTON_WIDTH = 80;
        constexpr int BUTTON_HEIGHT = 30;
        
        // Scale to actual DPI
        setBounds(0, 0, scaled(BUTTON_WIDTH), scaled(BUTTON_HEIGHT));
    }
};
```

### Example 2: Multi-Monitor Aware Layout

```cpp
void MyComponent::resized() {
    // Use component-specific scaling (correct on any monitor)
    int width = sm::scaled(sm::STANDARD_WIDTH, *this);
    int leftSection = sm::scaled(sm::WIDTH_DEVICE_LEFT, *this);
    
    leftPanel.setBounds(0, 0, leftSection, getHeight());
    rightPanel.setBounds(leftSection, 0, width - leftSection, getHeight());
}

void MyComponent::parentHierarchyChanged() override {
    Component::parentHierarchyChanged();
    // Window moved to different monitor - recalculate with new DPI
    resized();
}
```

### Example 3: Font Scaling

```cpp
void MyComponent::paint(juce::Graphics& g) override {
    using namespace sm;
    
    // Scale font size for current DPI
    juce::Font font("JetBrains Mono", scaled(FONT_SIZE_CHANNEL), juce::Font::plain);
    g.setFont(font);
    
    g.drawText("Channel 1", getLocalBounds(), juce::Justification::centred);
}
```

---

## Testing Contract

### Unit Test Coverage

All API functions must have unit tests covering:

1. **Nominal cases**: Standard DPI values (1.0, 1.25, 1.5, 2.0)
2. **Edge cases**: Extreme DPI (0.5, 4.0), zero values, negative values
3. **Error cases**: Headless context (null display)
4. **Type safety**: Integer truncation, float precision preservation

### Manual Test Coverage

1. **Platform testing**: macOS Retina, Windows 125%/150%/200%, Linux multi-DPI
2. **Multi-monitor**: Drag window between displays with different DPI
3. **Visual validation**: Compare screenshots at different DPI (should look identical relative to screen size)

---

## Compliance

This API contract aligns with:
- **ShowMIDI Constitution**: Principle I (Multi-Platform Architecture), Principle II (JUCE Framework Standards)
- **C++17 Standard**: Template parameter deduction, constexpr
- **JUCE Coding Standards**: Uses JUCE types, platform abstractions

## Versioning

**API Version**: 1.0.0  
**Stability**: Stable (ready for production use)  
**Backwards Compatibility**: N/A (initial release)
