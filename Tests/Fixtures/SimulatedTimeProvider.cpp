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

#include "SimulatedTimeProvider.h"

namespace showmidi
{

SimulatedTimeProvider::SimulatedTimeProvider()
    : simulatedTime(0.0)
{
}

double SimulatedTimeProvider::getCurrentTime() const
{
    return simulatedTime;
}

void SimulatedTimeProvider::advanceTime(double seconds)
{
    jassert(seconds >= 0.0); // Time can't go backwards
    simulatedTime += seconds;
}

void SimulatedTimeProvider::reset()
{
    simulatedTime = 0.0;
}

void SimulatedTimeProvider::setTime(double newTime)
{
    jassert(newTime >= 0.0); // Time must be non-negative
    simulatedTime = newTime;
}

} // namespace showmidi
