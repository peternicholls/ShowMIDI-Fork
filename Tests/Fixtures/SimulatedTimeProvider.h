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

#include "ITimeProvider.h"
#include <JuceHeader.h>

namespace showmidi
{

/**
 * @brief Simulated time provider for deterministic testing
 * 
 * Provides manual control over time progression, enabling deterministic tests
 * for time-dependent behavior (timeouts, animation, latency measurements).
 * 
 * **Example Usage**:
 * @code
 * SimulatedTimeProvider simTime;
 * simTime.advanceTime(1.5); // Advance by 1.5 seconds
 * expect(simTime.getCurrentTime() == 1.5);
 * @endcode
 * 
 * **Thread Safety**: NOT thread-safe; intended for single-threaded test execution
 */
class SimulatedTimeProvider : public ITimeProvider
{
public:
    /**
     * Construct a simulated time provider starting at time zero.
     */
    SimulatedTimeProvider();
    
    ~SimulatedTimeProvider() override = default;
    
    // ITimeProvider implementation
    double getCurrentTime() const override;
    
    /**
     * Manually advance the simulated time forward.
     * @param seconds Number of seconds to advance (must be >= 0)
     */
    void advanceTime(double seconds);
    
    /**
     * Reset simulated time back to zero.
     */
    void reset();
    
    /**
     * Set the simulated time to a specific value.
     * @param newTime New time value in seconds (must be >= 0)
     */
    void setTime(double newTime);
    
private:
    double simulatedTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimulatedTimeProvider)
};

} // namespace showmidi
