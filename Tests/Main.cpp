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

//==============================================================================
int main (int, char**)
{
#if ! JUCE_WINDOWS
  // Non-Windows: initialise JUCE subsystems for tests that may touch GUI types
  juce::ScopedJuceInitialiser_GUI juceInitialiser;
#endif

  juce::UnitTestRunner runner;
  runner.runAllTests();

  int numFailures = 0;
  for (int i = 0; i < runner.getNumResults(); ++i)
  {
    if (runner.getResult(i)->failures > 0)
      ++numFailures;
  }

  return numFailures > 0 ? 1 : 0;
}
