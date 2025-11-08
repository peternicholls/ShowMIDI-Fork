/*
 * This file is part of ShowMIDI.
 * Copyright (command) 2023 Uwyn LLC.  https://www.uwyn.com
 *
 * ShowMIDI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ShowMIDI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <JuceHeader.h>

#include "DeviceManager.h"
#include "LayoutConstants.h"
#include "SettingsManager.h"

namespace showmidi
{
    enum SidebarType
    {
        sidebarExpandable = 1,
        sidebarFixed
    };
    
    class SidebarListener
    {
    public:
        SidebarListener();
        virtual ~SidebarListener();
        
        virtual void sidebarChangedWidth() = 0;
    };
    
    class SidebarComponent : public Component
    {
    public:        
        static constexpr int X_COLLAPSED = showmidi::layout::SIDEBAR_X_COLLAPSED;
        static constexpr int Y_COLLAPSED = showmidi::layout::SIDEBAR_Y_COLLAPSED;

        static constexpr int X_EXPANDED = showmidi::layout::SIDEBAR_X_EXPANDED;
        static constexpr int Y_EXPANDED = showmidi::layout::SIDEBAR_Y_EXPANDED;

        static constexpr int X_SETTINGS = showmidi::layout::SIDEBAR_X_SETTINGS;
        static constexpr int Y_SETTINGS = showmidi::layout::SIDEBAR_Y_SETTINGS;

        static constexpr int X_PLAY_COLLAPSED = showmidi::layout::SIDEBAR_X_PLAY_COLLAPSED;
        static constexpr int Y_PLAY_COLLAPSED = showmidi::layout::SIDEBAR_Y_PLAY_COLLAPSED;

        static constexpr int X_PLAY_EXPANDED = showmidi::layout::SIDEBAR_X_PLAY_EXPANDED;
        static constexpr int Y_PLAY_EXPANDED = showmidi::layout::SIDEBAR_Y_PLAY_EXPANDED;

        static constexpr int X_VISUALIZATION_COLLAPSED = showmidi::layout::SIDEBAR_X_VISUALIZATION_COLLAPSED;
        static constexpr int Y_VISUALIZATION_COLLAPSED = showmidi::layout::SIDEBAR_Y_VISUALIZATION_COLLAPSED;

        static constexpr int X_VISUALIZATION_EXPANDED = showmidi::layout::SIDEBAR_X_VISUALIZATION_EXPANDED;
        static constexpr int Y_VISUALIZATION_EXPANDED = showmidi::layout::SIDEBAR_Y_VISUALIZATION_EXPANDED;

        static constexpr int X_RESET_COLLAPSED = showmidi::layout::SIDEBAR_X_RESET_COLLAPSED;
        static constexpr int Y_RESET_COLLAPSED = showmidi::layout::SIDEBAR_Y_RESET_COLLAPSED;

        static constexpr int X_RESET_EXPANDED = showmidi::layout::SIDEBAR_X_RESET_EXPANDED;
        static constexpr int Y_RESET_EXPANDED = showmidi::layout::SIDEBAR_Y_RESET_EXPANDED;

        static constexpr int X_HELP = showmidi::layout::SIDEBAR_X_HELP;
        static constexpr int Y_HELP = showmidi::layout::SIDEBAR_Y_HELP;

        SidebarComponent(SettingsManager*, DeviceManager*, SidebarType, SidebarListener*);
        ~SidebarComponent() override;
        
        void setup();

        void paint(Graphics&) override;
        
        void resized() override;
        
        int getActiveWidth();
        
        void updateSettings();
        
        struct Pimpl;
    private:
        std::unique_ptr<Pimpl> pimpl_;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidebarComponent)
    };
}
