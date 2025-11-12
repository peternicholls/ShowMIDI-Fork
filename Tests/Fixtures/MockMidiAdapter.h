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

#include "IMidiDeviceAdapter.h"
#include <JuceHeader.h>

namespace showmidi
{

/**
 * @brief Mock MIDI device adapter for deterministic testing
 * 
 * Provides controlled MIDI message injection without requiring physical hardware.
 * Supports pre-allocated message buffers for real-time safety validation.
 * 
 * **Example Usage**:
 * @code
 * MockMidiAdapter mockDevice("TestDevice");
 * mockDevice.start();
 * mockDevice.injectMessage(juce::MidiMessage::noteOn(1, 60, 0.8f), 0.0);
 * // Assert expected behavior in test
 * @endcode
 * 
 * **Thread Safety**: Methods are NOT thread-safe; tests should use single-threaded execution
 */
class MockMidiAdapter : public IMidiDeviceAdapter
{
public:
    /**
     * Construct a mock MIDI device.
     * @param identifier Device name for identification
     */
    explicit MockMidiAdapter(const juce::String& identifier = "MockMIDIDevice");
    
    ~MockMidiAdapter() override = default;
    
    // IMidiDeviceAdapter implementation
    void start() override;
    void stop() override;
    bool isStarted() const override;
    void injectMessage(const juce::MidiMessage& message, double timestamp) override;
    juce::String getIdentifier() const override;
    
    /**
     * Set the callback to receive injected MIDI messages.
     * @param callback Listener to notify when messages are injected
     */
    void setCallback(juce::MidiInputCallback* callback);
    
    /**
     * Get the total number of messages injected since construction.
     * Useful for test assertions.
     */
    int getMessageCount() const { return messageCount; }
    
    /**
     * Clear the message counter (reset to zero).
     */
    void resetMessageCount() { messageCount = 0; }
    
private:
    juce::String identifier;
    juce::MidiInputCallback* callback = nullptr;
    bool started = false;
    int messageCount = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MockMidiAdapter)
};

} // namespace showmidi
