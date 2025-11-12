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

#include <JuceHeader.h>
#include "../Fixtures/MockMidiAdapter.h"
#include "../Fixtures/SimulatedTimeProvider.h"

/**
 * Example test demonstrating ShowMIDI TDD protocol.
 * 
 * This test serves as a reference implementation for:
 * - JUCE UnitTest framework usage
 * - Test fixture patterns (MockMidiAdapter, SimulatedTimeProvider)
 * - Behavior-focused naming ("should X when Y")
 * - Arrange-Act-Assert structure
 * 
 * For complete TDD workflow, see: specs/004-tdd-adoption/contracts/test-protocol.md
 * For quickstart guide, see: specs/004-tdd-adoption/quickstart.md
 */
class ExampleTest : public juce::UnitTest
{
public:
    ExampleTest() : juce::UnitTest("Example Test Suite", "Tutorial") {}
    
    void runTest() override
    {
        demonstrateBasicAssertions();
        demonstrateMockMidiAdapter();
        demonstrateSimulatedTime();
    }
    
private:
    void demonstrateBasicAssertions()
    {
        beginTest("Basic assertions should validate expected behavior");
        
        // Integer equality
        int result = 2 + 2;
        expectEquals(result, 4, "2 + 2 should equal 4");
        
        // Boolean conditions
        bool condition = true;
        expect(condition, "Condition should be true");
        
        // Floating-point comparison with tolerance
        double value = 0.1 + 0.2;
        expectWithinAbsoluteError(value, 0.3, 0.0001, "0.1 + 0.2 should equal 0.3 (within tolerance)");
        
        // String comparison
        juce::String text = "ShowMIDI";
        expect(text.contains("MIDI"), "Text should contain 'MIDI'");
    }
    
    void demonstrateMockMidiAdapter()
    {
        beginTest("MockMidiAdapter should inject MIDI messages for deterministic testing");
        
        // Arrange: Create mock MIDI device
        showmidi::MockMidiAdapter mockDevice;
        
        struct TestCallback : public juce::MidiInputCallback
        {
            juce::MidiMessage lastMessage;
            int messageCount = 0;
            
            void handleIncomingMidiMessage(juce::MidiInput*, const juce::MidiMessage& message) override
            {
                lastMessage = message;
                ++messageCount;
            }
        };
        
        TestCallback callback;
        mockDevice.setCallback(&callback);
        
        // Act: Start device and inject message
        mockDevice.start();
        mockDevice.injectMessage(juce::MidiMessage::noteOn(1, 60, 0.8f), 0.0);
        
        // Assert: Verify message received
        expectEquals(callback.messageCount, 1, "One message should be received");
        expect(callback.lastMessage.isNoteOn(), "Message should be Note On");
        expectEquals(callback.lastMessage.getNoteNumber(), 60, "Note number should be 60 (middle C)");
        expectEquals(callback.lastMessage.getChannel(), 1, "Channel should be 1");
        
        mockDevice.stop();
    }
    
    void demonstrateSimulatedTime()
    {
        beginTest("SimulatedTimeProvider should enable deterministic time-based testing");
        
        // Arrange: Create simulated time provider
        showmidi::SimulatedTimeProvider timeProvider;
        
        // Act: Advance time by 1.5 seconds
        double startTime = timeProvider.getCurrentTime();
        timeProvider.advanceTime(1.5);
        double endTime = timeProvider.getCurrentTime();
        
        // Assert: Verify time advanced correctly
        expectWithinAbsoluteError(endTime - startTime, 1.5, 0.001, "Time should advance by exactly 1.5 seconds");
        
        // Demonstrate reset
        timeProvider.reset();
        expectWithinAbsoluteError(timeProvider.getCurrentTime(), 0.0, 0.001, "Reset should return time to zero");
        
        // Demonstrate manual time setting
        timeProvider.setTime(42.0);
        expectWithinAbsoluteError(timeProvider.getCurrentTime(), 42.0, 0.001, "setTime should set exact time value");
    }
};

// Register test suite with JUCE UnitTest framework
static ExampleTest exampleTest;
