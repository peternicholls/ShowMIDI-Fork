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
 * @brief Adapter interface for MIDI device operations (testable abstraction)
 * 
 * This interface provides a testable abstraction over JUCE's MidiInput/MidiOutput classes,
 * which are platform-specific and not designed for mocking. Test doubles can inject MIDI
 * messages deterministically without requiring physical hardware.
 * 
 * **Usage**:
 * - Production: Wrap real juce::MidiInput with ProductionMidiAdapter
 * - Tests: Use MockMidiAdapter to inject pre-defined MIDI sequences
 * 
 * **Pattern**: Adapter Pattern (Gang of Four)
 * **Thread Safety**: Implementations must be thread-safe for real-time MIDI callbacks
 */
class IMidiDeviceAdapter
{
public:
    virtual ~IMidiDeviceAdapter() = default;
    
    /**
     * Start listening for MIDI messages from the device.
     * For real devices, this enables the hardware callback.
     * For mocks, this enables message injection.
     */
    virtual void start() = 0;
    
    /**
     * Stop listening for MIDI messages.
     * Implementations must ensure no further callbacks after this returns.
     */
    virtual void stop() = 0;
    
    /**
     * Check if the device is currently active.
     * @return true if start() has been called and stop() has not
     */
    virtual bool isStarted() const = 0;
    
    /**
     * Inject a MIDI message into the system (test-only method).
     * Production implementations should no-op.
     * Mock implementations should trigger the registered callback.
     * 
     * @param message The MIDI message to inject
     * @param timestamp Timestamp in seconds (for deterministic timing tests)
     */
    virtual void injectMessage(const juce::MidiMessage& message, double timestamp) = 0;
    
    /**
     * Get the device identifier (name or ID).
     * @return Device name string for display purposes
     */
    virtual juce::String getIdentifier() const = 0;
};

} // namespace showmidi
