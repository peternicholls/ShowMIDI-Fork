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

#include "MockMidiAdapter.h"

namespace showmidi
{

MockMidiAdapter::MockMidiAdapter(const juce::String& id)
    : identifier(id)
{
}

void MockMidiAdapter::start()
{
    started = true;
}

void MockMidiAdapter::stop()
{
    started = false;
}

bool MockMidiAdapter::isStarted() const
{
    return started;
}

void MockMidiAdapter::injectMessage(const juce::MidiMessage& message, double timestamp)
{
    if (!started)
        return; // Silently ignore messages when not started (matches real MIDI behavior)
    
    ++messageCount;
    
    if (callback != nullptr)
    {
        // Note: We pass nullptr for the MidiInput source since this is a test double
        callback->handleIncomingMidiMessage(nullptr, message);
    }
    
    // TODO: Implement timestamp tracking for time-based test scenarios
    // See tasks.md Phase 5 (T040: ChannelAutoHideTests) for use case
    // Future: Store timestamps for tests that validate timing-dependent behavior
    // (e.g., channel auto-hide after inactivity, latency measurements)
    juce::ignoreUnused(timestamp);
}

juce::String MockMidiAdapter::getIdentifier() const
{
    return identifier;
}

void MockMidiAdapter::setCallback(juce::MidiInputCallback* newCallback)
{
    callback = newCallback;
}

} // namespace showmidi
