# Bug Fix: Startup Crash (nullptr dereference)

## Issue
**Crash Report:** macOS crash on startup with `EXC_BAD_ACCESS (SIGSEGV)` at address `0x0000000000000000`

**Stack Trace:**
```
Thread 0 Crashed:
0   ShowMIDI    MidiDeviceComponent::Pimpl::Pimpl(...) + 80 (MidiDeviceComponent.cpp:90)
4   ShowMIDI    std::make_unique<MidiDeviceComponent>(...) + 88
5   ShowMIDI    StandaloneDevicesComponent::Pimpl::getStandardWidth() const + 156 (StandaloneDevicesComponent.cpp:278)
```

## Root Cause
In `StandaloneDevicesComponent.cpp:278`, the fallback code attempted to create a temporary `MidiDeviceComponent` with **`nullptr`** as the `SettingsManager*` parameter:

```cpp
auto tempDevice = std::make_unique<MidiDeviceComponent>(nullptr, "temp");
return tempDevice->getStandardWidth();
```

The `MidiDeviceComponent::Pimpl` constructor at line 90 immediately dereferences this pointer:
```cpp
theme_(manager->getSettings().getTheme()),  // CRASH: manager is nullptr!
```

## Solution
Replaced the temporary device creation with a direct call to the scaling utility function `sm::getStandardWidth()`, which performs the same calculation without requiring a `SettingsManager`:

**File:** `Source/StandaloneDevicesComponent.cpp`

### Changes:
1. **Added include:** `#include "utility/Scaling.h"`
2. **Modified `getStandardWidth()` fallback:**
   ```cpp
   // Before (CRASHES):
   auto tempDevice = std::make_unique<MidiDeviceComponent>(nullptr, "temp");
   return tempDevice->getStandardWidth();
   
   // After (SAFE):
   return sm::getStandardWidth();
   ```

## Verification
- ✅ Build succeeds: `** BUILD SUCCEEDED **`
- ✅ No compiler warnings
- ✅ Logic preserved: `sm::getStandardWidth()` returns `scaled(STANDARD_WIDTH)` - the same calculation the temporary device would have performed

## Related Context
- This bug was introduced during DPI scaling work (feature 001-dpi-scaling)
- The scaling utility (`utility/Scaling.h`) already provides the correct static method
- The temporary device creation was unnecessary complexity that created this vulnerability

## Test Recommendation
Launch the standalone application with **zero MIDI devices** connected to verify the fallback path no longer crashes.
