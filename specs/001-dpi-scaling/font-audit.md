# Font Size Audit Report

**Date**: 2025-11-07  
**Task**: T006 - Complete font size audit  
**Feature**: 001-dpi-scaling

## Summary

Total `setFont()` calls found in ShowMIDI Source/: **43 calls**  
Files with font operations: **5 files**

## Critical Findings

### 🔴 Theme.cpp - Hardcoded Font Sizes (HIGH PRIORITY)

**File**: `Source/Theme.cpp`  
**Lines**: 65-77  
**Issue**: Theme font methods return hardcoded 16pt fonts without DPI scaling

```cpp
Font Theme::fontLabel()
{
    return Font(16, Font::bold);  // ❌ Hardcoded 16pt
}

Font Theme::fontData()
{
    return Font(16, Font::italic);  // ❌ Hardcoded 16pt
}
```

**Impact**: HIGH - These methods are used throughout the codebase (43 calls). Fixing these two methods will automatically fix all components.

**Recommendation**: Update both methods to use `sm::scaled(16.0f)` for font height.

## File-by-File Analysis

### ✅ MidiDeviceComponent.cpp (Reference Implementation)
- **Status**: Uses theme fonts (theme_.fontLabel(), theme_.fontData())
- **Action**: None - will automatically benefit from Theme.cpp fix
- **Calls**: 30 instances

### ✅ SettingsComponent.cpp
- **Status**: Uses theme fonts (theme.fontLabel(), theme.fontData())
- **Action**: None - will automatically benefit from Theme.cpp fix
- **Calls**: 10 instances

### ✅ StandaloneDevicesComponent.cpp
- **Status**: Uses theme fonts (theme.fontLabel())
- **Action**: None - will automatically benefit from Theme.cpp fix
- **Calls**: 1 instance

### ✅ AboutComponent.cpp
- **Status**: Uses theme fonts (theme_.fontLabel(), theme_.fontData())
- **Action**: None - will automatically benefit from Theme.cpp fix
- **Calls**: 3 instances

### ✅ PortListComponent.cpp
- **Status**: Uses theme fonts (theme.fontLabel())
- **Action**: None - will automatically benefit from Theme.cpp fix
- **Calls**: 1 instance

## Implementation Plan

### Single Fix Strategy

Instead of updating 43 individual `setFont()` calls, we can fix the root cause:

1. Update `Theme::fontLabel()` to return `Font(sm::scaled(16.0f), Font::bold)`
2. Update `Theme::fontData()` to return `Font(sm::scaled(16.0f), Font::italic)`

This will automatically propagate DPI-aware fonts to all 43 call sites across 5 components.

### Required Changes

**File**: `Source/Theme.cpp`

```cpp
// Add include at top
#include "utility/Scaling.h"

// Update fontLabel()
Font Theme::fontLabel()
{
    return Font(sm::scaled(16.0f), Font::bold);
}

// Update fontData()
Font Theme::fontData()
{
    return Font(sm::scaled(16.0f), Font::italic);
}
```

## Verification Checklist

After implementing the fix:

- [ ] Verify `Theme.cpp` includes `utility/Scaling.h`
- [ ] Verify `fontLabel()` uses `sm::scaled(16.0f)`
- [ ] Verify `fontData()` uses `sm::scaled(16.0f)`
- [ ] Build project and confirm no compile errors
- [ ] Test text rendering at 100% DPI (baseline)
- [ ] Test text rendering at 200% DPI (should be 32pt effective size)
- [ ] Verify text is crisp and not blurry at all DPI settings

## Additional Notes

### labelHeight() and dataHeight() Methods

These methods call `fontLabel().getHeight()` and `fontData().getHeight()` respectively. They will automatically return scaled heights after the font methods are updated. No additional changes needed.

### Other Components

The following components don't have any `setFont()` calls yet:
- SidebarComponent
- PaintedButton
- PluginEditor
- UwynLookAndFeel

These will be addressed in their respective component update tasks (T011-T025).

## Task Status

- [x] T006: Font size audit complete
- [ ] T007: Create font scaling utility wrapper (may not be needed - Theme.cpp fix is sufficient)
- [ ] T008: Update all identified setFont() calls (will be handled by Theme.cpp fix)

## Next Steps

Proceed to implement the Theme.cpp fix as described above. This single change will resolve font scaling for all existing components.
