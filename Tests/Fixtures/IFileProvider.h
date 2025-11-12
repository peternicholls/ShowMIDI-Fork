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
 * @brief Adapter interface for file operations (enables file dialog mocking)
 * 
 * This interface abstracts file selection (FileChooser, drag-and-drop) to enable
 * deterministic testing without requiring user interaction or GUI.
 * 
 * **Usage**:
 * - Production: Wrap juce::FileChooser with ProductionFileProvider
 * - Tests: Use MockFileProvider to return pre-defined file paths
 * 
 * **Pattern**: Adapter Pattern for testability
 */
class IFileProvider
{
public:
    virtual ~IFileProvider() = default;
    
    /**
     * Prompt the user to select a file (or return mock file in tests).
     * @param title Dialog title
     * @param patterns File patterns (e.g., "*.svg")
     * @return Selected file, or invalid File if cancelled
     */
    virtual juce::File selectFile(const juce::String& title, const juce::String& patterns) = 0;
    
    /**
     * Check if a file exists and is readable.
     * @param file File to check
     * @return true if file exists and can be read
     */
    virtual bool fileExists(const juce::File& file) const = 0;
};

} // namespace showmidi
