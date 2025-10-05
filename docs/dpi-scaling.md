# DPI-Aware Layout

This document explains the utilities for DPI-aware drawing and how to introduce new layout constants.

## Scaling utilities

`Source/utility/Scaling.h` provides header-only helpers that convert logical layout units to physical pixels based on the monitor's scale factor.  All coordinates and sizes in ShowMIDI are expressed in logical units (assuming a 96 DPI baseline).

### `sm::scaled`

`sm::scaled(value)` multiplies any numeric value by the current display's scale factor.  Overloads exist for `juce::Rectangle<int>` and for supplying a `juce::Component` when you need the scale for the monitor on which that component resides.

```cpp
// Scale a font size for the main display
g.setFont(sm::scaled(14.0f));

// Scale a rectangle for a specific component's monitor
auto area = sm::scaled(juce::Rectangle<int>{0, 0, 32, 20}, *this);
```

### `sm::dpiScale`

`sm::dpiScale()` returns the raw scale factor of the main monitor (e.g. `1.0`, `1.5`, `2.0`).  The overload `sm::dpiScale(component)` queries JUCE for the scale of the monitor that contains the given component.  Call this when you need to perform custom calculations with the scale value.

## Adding layout constants

`Source/layout/Constants.h` centralises positions and sizes for the UI.  When adding a constant:

1. Place it inside the `showmidi::layout` namespace under the most appropriate section heading.
2. Declare it as `static constexpr` using an uppercase name with underscores (e.g. `X_FOO_BAR`).
3. Precede the declaration with a brief Doxygen comment (`/** ... */`) describing its purpose and units.
4. Keep the value in logical units and always wrap it with `sm::scaled()` at the call site.

Following these conventions keeps layout code consistent and ensures new elements scale correctly on high‑DPI displays.
