# Component DPI Scaling Audit Results

**Date**: 2025-11-07  
**Tasks**: T015-T017 (Phase 3 User Story 1)

## Summary

Audited three partially-complete components to identify all hardcoded dimensional values requiring `sm::scaled()` wrapping for DPI awareness.

---

## T015: MainLayoutComponent Audit

**File**: `Source/MainLayoutComponent.cpp`  
**Status**: ✅ Complete  
**Findings**: Minimal changes required

### Hardcoded Values Found

1. **Line 29**: `DEFAULT_WINDOW_HEIGHT = 600`
   - **Usage**: Used in `setBounds()` calls (lines 46, 58) and `setSize()` (line 159)
   - **Action Required**: Wrap with `sm::scaled(600, *owner_)`
   - **Impact**: Window initial height

### Already DPI-Aware

- ✅ `showmidi::layout::STANDARD_WIDTH` (line 50) - uses Constants.h
- ✅ `Theme::SCROLLBAR_THICKNESS` (line 55) - theme constant
- ✅ `Theme::MIDI_DEVICE_SPACING` (line 52) - theme constant
- ✅ `getSidebarWidth()` - returns scaled value from SidebarComponent

### Implementation Plan (T018)

**Files to modify**:
1. `Source/MainLayoutComponent.cpp`
   - Add `#include "utility/Scaling.h"` if not present
   - Change `static constexpr int DEFAULT_WINDOW_HEIGHT = 600;` to calculate scaled value
   - Option A: `sm::scaled(600, *owner_)` at usage sites (3 locations)
   - Option B: Store owner pointer early and calculate once
   - **Recommended**: Option A for consistency with PluginEditor pattern

**Locations requiring changes**:
- Line 46: `sidebar_->setBounds(0, 0, getSidebarWidth(), sm::scaled(600, *owner_));`
- Line 58: `viewport_->setBounds(sidebar_->getWidth(), 0, default_width, sm::scaled(600, *owner_));`
- Line 159: `setSize(pimpl_->sidebar_->getWidth() + pimpl_->viewport_->getWidth(), sm::scaled(600, *owner_));`

**Effort**: ~1 hour

---

## T016: StandaloneDevicesComponent Audit

**File**: `Source/StandaloneDevicesComponent.cpp`  
**Status**: ✅ Complete  
**Findings**: Minimal DPI work needed (mostly already using scaled utilities)

### Hardcoded Values Found

1. **Line 87**: Text positioning for "No MIDI devices" message
   ```cpp
   g.drawMultiLineText("No MIDI devices...",
                       Theme::MIDI_DEVICE_SPACING + 23, 24, getStandardWidth() - 46);
   ```
   - **Values**: `23` (x offset), `24` (y position), `46` (width reduction)
   - **Action Required**: Wrap with `sm::scaled(23)`, `sm::scaled(24)`, `sm::scaled(46)`
   - **Impact**: Centering of "no devices" message

### Timing Values (NOT dimensional - no action needed)

- Line 46: `startTimer(RenderDevices, 1000 / 30);` - 30 FPS refresh rate
- Line 48: `startTimer(GrabKeyboardFocus, 100);` - 100ms delay

### Already DPI-Aware

- ✅ `Theme::MIDI_DEVICE_SPACING` - theme constant
- ✅ `getStandardWidth()` - calls `sm::getStandardWidth()`
- ✅ `theme.fontLabel()` - uses scaled fonts (T008 complete)
- ✅ All `setSize()` and `setBounds()` calls use scaled getters

### Implementation Plan (T019)

**Files to modify**:
1. `Source/StandaloneDevicesComponent.cpp`
   - Verify `#include "utility/Scaling.h"` is present (added in bugfix)
   - Line 87: Apply `sm::scaled()` to three magic numbers

**Location requiring changes**:
```cpp
// Before:
g.drawMultiLineText("No MIDI devices are visible.\n\n"
                    "Either no devices are connected, or all connected devices are hidden in the settings.",
                    Theme::MIDI_DEVICE_SPACING + 23, 24, getStandardWidth() - 46);

// After:
g.drawMultiLineText("No MIDI devices are visible.\n\n"
                    "Either no devices are connected, or all connected devices are hidden in the settings.",
                    Theme::MIDI_DEVICE_SPACING + sm::scaled(23), sm::scaled(24), getStandardWidth() - sm::scaled(46));
```

**Effort**: ~30 minutes (single line change)

---

## T017: SettingsComponent Audit

**File**: `Source/SettingsComponent.cpp`  
**Status**: ✅ Complete  
**Findings**: **Extensive DPI scaling work required** - many hardcoded layout values

### Hardcoded Values Found

#### Layout Margins and Spacing (Lines 183-186)

```cpp
auto left_margin = 23;
auto button_spacing = 73;
auto button_spacing4 = 45;
```

- **Usage**: Used throughout `resized()` for button positioning (~30 locations)
- **Action Required**: Wrap each with `sm::scaled()`
- **Impact**: All settings buttons, all color selectors

#### Button Positioning Hardcoded Values

**Middle C buttons** (lines 190-195):
- `left_margin`, `left_margin + button_spacing`, `left_margin + button_spacing * 2`
- All use `setBoundsForTouch()` with `getWidth()` (not DPI-aware!)

**Note format buttons** (lines 201-204)
**Number format buttons** (lines 210-213)
**Timeout buttons** (lines 219-226)
**Window position buttons** (lines 234-237) - if not plugin/iOS
**Graph height buttons** (lines 244-249)
**Theme buttons** (lines 255-262)

#### Color Selector Layout (Lines 268-293)

```cpp
int color_x = 23;
// ... 10 color button placements using color_x
color_x = 23;  // Reset
// ... 4 more color button placements
```

- **Values**: `color_x = 23` (used 14 times)
- **Action Required**: Use `sm::scaled(23)` for initial value

#### Close Button (Lines 296-297)

```cpp
closeButton_->setBoundsForTouch(getWidth() - 24, theme.linePosition(1) - theme.labelHeight() + 3,
                                28, theme.labelHeight());
```

- **Values**: `24` (right margin), `3` (vertical adjustment), `28` (button width)
- **Action Required**: Wrap each with `sm::scaled()`

#### Text Drawing Positions (Multiple lines)

Lines 317, 336, 353, 370, 393, 411, 430:
```cpp
g.drawText("...", 23, y_offset, getWidth(), theme.labelHeight(), ...);
```

- **Value**: `23` (left margin for labels)
- **Action Required**: Use `sm::scaled(23)`
- **Count**: 7 occurrences

### Already DPI-Aware

- ✅ `theme.linePosition(N)` - uses scaled line height calculation
- ✅ `theme.labelHeight()` - returns scaled font height (T008 complete)
- ✅ `theme.fontLabel()`, `theme.fontData()` - use scaled fonts
- ✅ `showmidi::layout::STANDARD_WIDTH` - Constants.h (line 175)
- ✅ `SidebarComponent::X_SETTINGS` - constant from SidebarComponent

### Problematic Patterns

1. **`getWidth()` without component context**:
   - Many `setBoundsForTouch()` calls use `getWidth()` as the width parameter
   - This may need review - width should be explicit for touch targets
   - **Decision needed**: Keep for relative sizing or replace with explicit widths?

2. **`color_x` reset pattern** (line 284):
   - Resets to `23` after 5 colors
   - Suggests column layout with wrapping
   - **Must preserve**: Scaling should maintain this pattern

3. **Window size calculation** (lines 169-175):
   ```cpp
   if (manager_->isPlugin())
       height = theme.linePosition(22.5);
   else
       height = theme.linePosition(25.5);
   owner_->setSize(showmidi::layout::STANDARD_WIDTH - SidebarComponent::X_SETTINGS * 2, height);
   ```
   - Uses fractional line positions (22.5, 25.5)
   - `linePosition()` is already DPI-aware
   - Width calculation uses constants (already scaled)
   - **No action required** for this calculation

### Implementation Plan (T020)

**Complexity**: HIGH - ~40+ locations need modification

**Files to modify**:
1. `Source/SettingsComponent.cpp`
   - Verify `#include "utility/Scaling.h"` is present
   - Add `#include "layout/Constants.h"` if not present
   - Modify all hardcoded dimensional values

**Recommended approach**:
1. Define scaled constants at function start:
   ```cpp
   const auto scaled_left_margin = sm::scaled(23);
   const auto scaled_button_spacing = sm::scaled(73);
   const auto scaled_button_spacing4 = sm::scaled(45);
   const auto scaled_color_x = sm::scaled(23);
   ```

2. Replace all usages of raw numbers with scaled constants

3. For `setBoundsForTouch()` calls:
   - Review whether `getWidth()` should remain or use explicit widths
   - Ensure touch target sizes meet minimum (~44pt at 96 DPI = `sm::scaled(44)`)

**Locations requiring changes** (estimate 45 locations):

**Section 1: Layout constants** (lines 183-186) - 3 lines
**Section 2: Middle C buttons** (lines 190-195) - 6 values
**Section 3: Note format buttons** (lines 201-204) - 4 values  
**Section 4: Number format buttons** (lines 210-213) - 4 values
**Section 5: Timeout buttons** (lines 219-226) - 8 values
**Section 6: Window position buttons** (lines 234-237) - 4 values (conditional)
**Section 7: Graph height buttons** (lines 244-249) - 6 values
**Section 8: Theme buttons** (lines 255-262) - 8 values
**Section 9: Color selectors** (lines 268-293) - 14 values
**Section 10: Close button** (lines 296-297) - 3 values
**Section 11: Text labels** (7 occurrences) - 7 values

**Total**: ~67 individual values requiring `sm::scaled()` wrapping

**Effort**: ~3-4 hours (complexity + testing)

---

## Comparison Summary

| Component | Hardcoded Values | Complexity | Effort | Priority |
|-----------|------------------|------------|--------|----------|
| MainLayoutComponent | 1 constant (3 usages) | LOW | 1 hour | Medium |
| StandaloneDevicesComponent | 3 values (1 line) | LOW | 30 min | Low |
| SettingsComponent | ~67 values | HIGH | 3-4 hours | High |

**Total estimated effort for T018-T020**: ~5 hours

---

## Dependencies for T018-T020

**Before starting implementation**:
- ✅ T006-T008: Font scaling complete (fonts already scaled)
- ✅ T011-T013: PluginEditor complete (reference pattern established)
- ✅ `utility/Scaling.h`: DPI scaling API available
- ✅ `layout/Constants.h`: Centralized constants available

**Implementation order recommendation**:
1. **T019** (StandaloneDevicesComponent) - quick win, 30 minutes
2. **T018** (MainLayoutComponent) - straightforward, 1 hour  
3. **T020** (SettingsComponent) - complex, 3-4 hours

**Rationale**: Start with easiest to build confidence, end with most complex.

---

## Testing Checklist (for T018-T020)

After each component is updated:

### Build Validation
- [ ] Code compiles without errors
- [ ] No JUCE leak detector warnings
- [ ] Zero compiler warnings related to changes

### Visual Validation (100% DPI)
- [ ] Component renders correctly at baseline (96 DPI / 100%)
- [ ] No layout breakage (buttons aligned, no overlaps)
- [ ] Text readable and properly positioned

### DPI Scaling Validation
- [ ] Test at 150% DPI (144 DPI / 1.5x scale)
  - [ ] All elements scale proportionally
  - [ ] Buttons maintain spacing
  - [ ] Text remains readable
- [ ] Test at 200% DPI (192 DPI / 2x scale)
  - [ ] Component doesn't overflow bounds
  - [ ] Touch targets meet minimum size
  - [ ] Visual appearance matches 100% relatively

### Multi-Monitor Test (if available)
- [ ] Drag window between monitors with different DPI
- [ ] Component rescales correctly
- [ ] No rendering artifacts

---

## Next Steps

1. **Immediate**: Mark T015-T017 as complete in `tasks.md`
2. **Next phase**: Begin implementation with T019 (quick win)
3. **Parallel opportunity**: T015-T017 were parallelizable [P] - could have run simultaneously with multiple developers
4. **Blocked tasks**: T026-T031 (User Story 1 validation) cannot proceed until T018-T020 complete

---

## References

- **Task Breakdown**: [tasks.md](./tasks.md)
- **Implementation Plan**: [plan.md](./plan.md)
- **Scaling API Contract**: [contracts/scaling-api.md](./contracts/scaling-api.md)
- **Reference Implementation**: `Source/MidiDeviceComponent.cpp` (2,568 lines, fully DPI-aware)
- **Bugfix Document**: [bugfix-nullptr-crash.md](./bugfix-nullptr-crash.md) (StandaloneDevicesComponent already includes Scaling.h)

---

**Audit completed**: 2025-11-07  
**Ready for implementation**: ✅ T018, T019, T020 can proceed
