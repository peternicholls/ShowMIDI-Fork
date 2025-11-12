/*
  ==============================================================================

   ShowMIDI
   Copyright (C) 2023 Uwyn LLC.  https://www.uwyn.com
   Copyright (C) 2025 Peter Nicholls.  https://www.peternicholls.me.uk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace showmidi
{

/**
 * @brief Interface for time providers (enables deterministic timing in tests)
 * 
 * This interface abstracts time retrieval to enable deterministic, repeatable tests
 * without relying on wall-clock time (Time::getMillisecondCounterHiRes()).
 * 
 * **Usage**:
 * - Production: Use SystemTimeProvider (wraps juce::Time)
 * - Tests: Use SimulatedTimeProvider (manual time control)
 * 
 * **Pattern**: Strategy Pattern for testability
 */
class ITimeProvider
{
public:
    virtual ~ITimeProvider() = default;
    
    /**
     * Get the current time in seconds.
     * @return Current time (seconds since epoch for production, simulated time for tests)
     */
    virtual double getCurrentTime() const = 0;
};

} // namespace showmidi
