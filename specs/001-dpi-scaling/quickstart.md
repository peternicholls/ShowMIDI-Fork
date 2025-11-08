# Quickstart Guide: DPI-Aware UI Scaling

**For**: ShowMIDI Component Developers  
**Version**: 1.0.0  
**Date**: 2025-11-07

## Overview

This guide shows you how to make ShowMIDI UI components DPI-aware in 5 minutes. Follow the patterns from `MidiDeviceComponent` (the reference implementation) to ensure consistent scaling across all components.

## Prerequisites

- ShowMIDI codebase checked out on branch `001-dpi-scaling`
- Familiarity with JUCE Component class
- Basic C++ knowledge (templates, const references)

## Quick Reference

### The Golden Rule

**Replace hardcoded pixel values with scaled constants:**

```cpp
// ❌ BEFORE (hardcoded)
setBounds(0, 0, 130, 200);
font.setHeight(14.0f);

// ✅ AFTER (DPI-aware)
setBounds(0, 0, sm::scaled(sm::STANDARD_WIDTH), sm::scaled(200));
font.setHeight(sm::scaled(14.0f));
```

### Essential Includes

```cpp
#include "utility/Scaling.h"   // sm::scaled() functions
#include "layout/Constants.h"  // sm::STANDARD_WIDTH, etc.
```

### Common Patterns

| Operation | Hardcoded (Old) | DPI-Aware (New) |
|-----------|-----------------|-----------------|
| Set width/height | `setBounds(x, y, 100, 50)` | `setBounds(x, y, sm::scaled(100), sm::scaled(50))` |
| Font size | `font.setHeight(14.0f)` | `font.setHeight(sm::scaled(14.0f))` |
| Use constant | `int width = 130;` | `int width = sm::scaled(sm::STANDARD_WIDTH);` |
| Scale rectangle | `child.setBounds(rect)` | `child.setBounds(sm::scaled(rect, *this))` |
| Get standard width | `return 130;` | `return sm::getStandardWidth();` |

## Step-by-Step Migration

### Step 1: Add Includes

At the top of your `.cpp` file:

```cpp
#include "utility/Scaling.h"
#include "layout/Constants.h"
```

### Step 2: Find Hardcoded Dimensions

Search your component for:
- Numbers in `setBounds()` calls
- Numbers in `font.setHeight()` calls
- Arithmetic with pixel values (e.g., `130 - 2`)
- Width/height getter methods returning constants

Example findings:
```cpp
// Found in MyComponent.cpp:
setBounds(0, 0, 130, 200);           // Line 45
font.setHeight(14.0f);               // Line 78
int width = 130 - 2;                 // Line 103
```

### Step 3: Replace with Scaled Values

#### For Simple Dimensions

```cpp
// BEFORE:
setBounds(0, 0, 130, 200);

// AFTER:
using namespace sm;  // Optional: cleaner code
setBounds(0, 0, scaled(STANDARD_WIDTH), scaled(200));
```

#### For Arithmetic Expressions

```cpp
// BEFORE:
int separatorWidth = 130 - 2;

// AFTER:
using namespace sm;
int separatorWidth = scaled(STANDARD_WIDTH) - scaled(WIDTH_SEPARATOR);
// Or better: extract to constant first, then scale
```

#### For Font Sizes

```cpp
// BEFORE:
juce::Font font("JetBrains Mono", 14.0f, juce::Font::plain);
g.setFont(font);

// AFTER:
using namespace sm;
juce::Font font("JetBrains Mono", scaled(FONT_SIZE_CHANNEL), juce::Font::plain);
g.setFont(font);
```

### Step 4: Use Component-Specific Scaling (Multi-Monitor)

For components that may appear on different monitors:

```cpp
// BEFORE:
void MyComponent::resized() {
    int width = sm::scaled(sm::STANDARD_WIDTH);  // Uses primary display
    setBounds(0, 0, width, getHeight());
}

// AFTER (multi-monitor aware):
void MyComponent::resized() {
    int width = sm::scaled(sm::STANDARD_WIDTH, *this);  // Uses THIS component's display
    setBounds(0, 0, width, getHeight());
}
```

### Step 5: Handle DPI Changes

When window moves between monitors with different DPI:

```cpp
void MyComponent::parentHierarchyChanged() override {
    Component::parentHierarchyChanged();
    
    // Component may have moved to different display - recalculate layout
    resized();
}
```

## Complete Example

### Before: Hardcoded Component

```cpp
// MyComponent.cpp (BEFORE)
#include "MyComponent.h"

MyComponent::MyComponent() {
    setSize(130, 200);
}

void MyComponent::paint(juce::Graphics& g) {
    juce::Font font("JetBrains Mono", 14.0f, juce::Font::plain);
    g.setFont(font);
    g.drawText("Hello", 10, 10, 100, 20, juce::Justification::centred);
}

void MyComponent::resized() {
    header.setBounds(0, 0, 130, 30);
    content.setBounds(0, 30, 130, 170);
}
```

### After: DPI-Aware Component

```cpp
// MyComponent.cpp (AFTER)
#include "MyComponent.h"
#include "utility/Scaling.h"
#include "layout/Constants.h"

MyComponent::MyComponent() {
    using namespace sm;
    setSize(scaled(STANDARD_WIDTH), scaled(200));
}

void MyComponent::paint(juce::Graphics& g) {
    using namespace sm;
    
    juce::Font font("JetBrains Mono", scaled(FONT_SIZE_CHANNEL), juce::Font::plain);
    g.setFont(font);
    
    // Scale text bounds
    g.drawText("Hello", 
               scaled(10), scaled(10), 
               scaled(100), scaled(20), 
               juce::Justification::centred);
}

void MyComponent::resized() {
    using namespace sm;
    
    int width = scaled(STANDARD_WIDTH, *this);  // Multi-monitor aware
    header.setBounds(0, 0, width, scaled(30));
    content.setBounds(0, scaled(30), width, scaled(170));
}

void MyComponent::parentHierarchyChanged() override {
    Component::parentHierarchyChanged();
    resized();  // Recalculate when moved to different monitor
}
```

## Common Pitfalls

### ❌ Pitfall 1: Scaling Already-Scaled Values

```cpp
// WRONG: Double-scaling
int width = sm::scaled(sm::STANDARD_WIDTH);
setBounds(0, 0, sm::scaled(width), getHeight());  // width is already scaled!

// CORRECT: Scale once
int width = sm::scaled(sm::STANDARD_WIDTH);
setBounds(0, 0, width, getHeight());
```

### ❌ Pitfall 2: Forgetting to Scale Font Sizes

```cpp
// WRONG: Hardcoded font size
font.setHeight(14.0f);  // Will be tiny on high-DPI displays

// CORRECT: Scale font size
font.setHeight(sm::scaled(14.0f));
```

### ❌ Pitfall 3: Scaling Zero/One Constants

```cpp
// UNNECESSARY: Zero and one don't need scaling
setBounds(sm::scaled(0), sm::scaled(0), width, height);

// BETTER: Skip scaling for 0/1
setBounds(0, 0, width, height);
```

### ❌ Pitfall 4: Not Handling Monitor Switches

```cpp
// INCOMPLETE: Component won't update when moved to different monitor
void MyComponent::resized() {
    int width = sm::scaled(sm::STANDARD_WIDTH, *this);
    setBounds(0, 0, width, getHeight());
}
// Missing: parentHierarchyChanged() override!

// COMPLETE: Add monitor switch handling
void MyComponent::parentHierarchyChanged() override {
    Component::parentHierarchyChanged();
    resized();
}
```

## Reference Implementation

**Study this component for all patterns**: `Source/MidiDeviceComponent.cpp` (2,568 lines, fully DPI-aware)

Key sections to examine:
- **Lines 150-200**: Font size scaling
- **Lines 450-550**: Layout calculations with `sm::scaled()`
- **Lines 800-900**: Rectangle scaling for child components
- **Entire file**: Consistent use of Constants.h values

## Testing Checklist

After migrating your component:

- [ ] **Compile**: No errors or warnings
- [ ] **Visual**: Component looks correct at 100% DPI (96 DPI / 1.0× scale)
- [ ] **High-DPI**: Test at 200% DPI (192 DPI / 2.0× scale) - component should be 2× larger
- [ ] **Fractional**: Test at 125% DPI (120 DPI / 1.25× scale) - component scales smoothly
- [ ] **Multi-monitor**: Drag window between monitors with different DPI - component updates correctly
- [ ] **Fonts**: Text is crisp and readable at all DPI settings (not blurry)
- [ ] **Themes**: SVG themes render correctly (no pixelation)
- [ ] **No hardcoded values**: Search for numbers in your code - should all be using `sm::scaled()`

## Performance Tips

### ✅ DO: Cache Scaled Values in resized()

```cpp
void MyComponent::resized() {
    // Calculate once per layout pass
    cachedWidth = sm::scaled(sm::STANDARD_WIDTH, *this);
    cachedHeight = sm::scaled(200, *this);
}

void MyComponent::paint(Graphics& g) {
    // Use cached values (no recalculation)
    g.fillRect(0, 0, cachedWidth, cachedHeight);
}
```

### ❌ DON'T: Scale in paint() Method

```cpp
void MyComponent::paint(Graphics& g) {
    // INEFFICIENT: Recalculates every frame
    int width = sm::scaled(sm::STANDARD_WIDTH, *this);
    g.fillRect(0, 0, width, getHeight());
}
```

## FAQ

### Q: Do I need to scale EVERY number in my component?

**A: No.** Only scale values that represent **screen dimensions** (pixels):
- ✅ Scale: Width, height, x, y positions, font sizes, margins, padding
- ❌ Don't scale: Angles (degrees), percentages (0.0-1.0), counts (number of items), MIDI values (0-127)

### Q: What about SVG graphics and themes?

**A: No changes needed.** SVG is vector format and scales automatically. JUCE's `Drawable` class handles DPI-aware rasterization.

### Q: Should I scale values in constructors or resized()?

**A: Prefer `resized()`.** The constructor may run before component is attached to a display. `resized()` is called when component has a known display context.

```cpp
// PREFER resized():
void MyComponent::resized() {
    int width = sm::scaled(sm::STANDARD_WIDTH, *this);
    setBounds(0, 0, width, getHeight());
}

// Avoid constructor scaling:
MyComponent::MyComponent() {
    // *this may not be on any display yet - scale factor unreliable
    setSize(sm::scaled(130), sm::scaled(200));
}
```

### Q: How do I test on high-DPI if I don't have a Retina display?

**macOS**: Use Accessibility Zoom (`System Preferences → Accessibility → Zoom`)

**Windows**: `Settings → Display → Scale and layout` → set to 200%

**Linux**: 
```bash
# X11: Use xrandr
xrandr --output HDMI-1 --scale 0.5x0.5  # Simulates 2× DPI

# Or use KDE/GNOME display settings
```

### Q: My fonts look blurry on Windows. Why?

**A:** Windows has complex DPI handling. Ensure:
1. ShowMIDI declares DPI awareness in manifest (should be automatic via JUCE)
2. You're using integer font sizes after scaling (e.g., 14.0 → 28.0, not 14.0 → 28.3)
3. Your plugin host supports DPI awareness (some old hosts don't)

### Q: Should I extract every hardcoded number to Constants.h?

**A:** Use judgment:
- **YES** for shared constants used in multiple places (e.g., `STANDARD_WIDTH`)
- **YES** for design-related dimensions (e.g., `HEIGHT_CHANNEL`)
- **NO** for one-off layout-specific values (e.g., a unique component's internal padding)

## Next Steps

1. **Choose a component** to migrate (start small, e.g., `PaintedButton.cpp`)
2. **Follow this guide** step-by-step
3. **Test thoroughly** using the checklist above
4. **Compare with MidiDeviceComponent** if you get stuck
5. **Move to next component** and repeat

## Support

- **Reference implementation**: `Source/MidiDeviceComponent.cpp`
- **API documentation**: `specs/001-dpi-scaling/contracts/scaling-api.md`
- **Full specification**: `specs/001-dpi-scaling/spec.md`
- **Constitution principles**: `.specify/memory/constitution.md`

## Summary

**Three-step pattern for any component:**

1. **Include headers**: `#include "utility/Scaling.h"` and `#include "layout/Constants.h"`
2. **Wrap dimensions**: Replace `100` with `sm::scaled(100)`, replace `WIDTH` with `sm::scaled(WIDTH)`
3. **Handle monitor changes**: Override `parentHierarchyChanged()` → call `resized()`

That's it! You're now writing DPI-aware ShowMIDI code. 🎉
