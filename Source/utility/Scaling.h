//
//  Created by Peter Nicholls on 23/05/2025.
//  Copyright © 2025 Uwyn. All rights reserved.
//

#pragma once
/**
 Scaling helpers for DPI‑aware UI code in ShowMIDI.
 
 All numbers in your component code should be *logical* units
 (the size you would design for at 100 % / 96 dpi).  Wrap them
 with `sm::scaled()` so the helper multiplies them by the
 current monitor’s scale‑factor at run‑time.
 
 g.setFont (sm::scaled (14.0f));
 constexpr int knobDiameter = sm::scaled (32);
 
 If you need the raw scale for manual maths you can call
 `sm::dpiScale()` directly.
 
 The helpers are header‑only and incur zero overhead once the
 compiler inlines them.
 */
#include <juce_gui_basics/juce_gui_basics.h>
#include "../layout/Constants.h"

namespace sm
{
    /** Returns the scale factor of the main display (e.g. 1.0, 1.5, 2.0). */
    inline float dpiScale() noexcept
    {
        auto& displays = juce::Desktop::getInstance().getDisplays();
        if (auto* pd = displays.getPrimaryDisplay())
            return pd->scale;
        
        return 1.0f;
    }

    /** Overload that asks JUCE for the scale factor of the component’s
     current screen.  Use this if your window can span monitors
     of different DPI or moves between them. */
    inline float dpiScale (const juce::Component& c) noexcept
    {
        return c.getApproximateScaleFactorForComponent (&c);
    }

    /** Scale any arithmetic value (int, float, double, etc.). */
    template <typename Numeric>
    inline Numeric scaled (Numeric value) noexcept
    {
        return static_cast<Numeric> (value * dpiScale());
    }

    /** Scale a value relative to a particular component’s monitor. */
    template <typename Numeric>
    inline Numeric scaled (Numeric value, const juce::Component& c) noexcept
    {
        return static_cast<Numeric> (value * dpiScale (c));
    }

    /** Convenience overload for rectangles (using main‑display scale). */
    inline juce::Rectangle<int> scaled (juce::Rectangle<int> r) noexcept
    {
        return { scaled (r.getX()),  scaled (r.getY()),
            scaled (r.getWidth()), scaled (r.getHeight()) };
    }

    /** Convenience overload for rectangles relative to a component. */
    inline juce::Rectangle<int> scaled (juce::Rectangle<int> r,
                                        const juce::Component& c) noexcept
    {
        return { scaled (r.getX(), c),  scaled (r.getY(), c),
            scaled (r.getWidth(), c), scaled (r.getHeight(), c) };
    }

    /** Get the standard MIDI device width, scaled for current DPI. */
    inline int getStandardWidth() noexcept
    {
        return scaled(::showmidi::layout::STANDARD_WIDTH);
    }

    /** Get the standard MIDI device width, scaled for a specific component's DPI. */
    inline int getStandardWidth(const juce::Component& c) noexcept
    {
        return scaled(::showmidi::layout::STANDARD_WIDTH, c);
    }
} // namespace sm
