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

/**
 * Layout and positioning constants for ShowMIDI UI components.
 * 
 * All values are in logical units (96 DPI baseline) and should be
 * scaled using sm::scaled() for DPI-aware rendering.
 * 
 * Constants are organized by functional area for easy maintenance.
 */

namespace showmidi::layout
{
    // =================================================================
    // MAIN LAYOUT DIMENSIONS
    // =================================================================
    
    /** Standard width for MIDI device components (logical units). */
    static constexpr int STANDARD_WIDTH = 150;
    
    /** Width of separator elements between components. */
    static constexpr int WIDTH_SEPARATOR = 48;
    
    /** Middle reference point for horizontal alignment. */
    static constexpr int X_MID = 171;
    
    /** Default window height for main layout (logical units). */
    static constexpr int DEFAULT_WINDOW_HEIGHT = 600;
    
    /** Default editor height for plugin (logical units). */
    static constexpr int DEFAULT_EDITOR_HEIGHT = 600;

    // =================================================================
    // TYPOGRAPHY
    // =================================================================
    
    /** Base font size for UI text (logical units at 96 DPI). */
    static constexpr int FONT_SIZE = 9;
    
    /** Line height for narrow layouts (iOS portrait). */
    static constexpr int LINE_HEIGHT_NARROW = 16;
    
    /** Line height for normal layouts. */
    static constexpr int LINE_HEIGHT_NORMAL = 22;

    // =================================================================
    // SIDEBAR LAYOUT
    // =================================================================
    
    /** Expanded sidebar width (logical units). */
    static constexpr int SIDEBAR_EXPANDED_WIDTH = 210;
    
    /** Narrow expanded sidebar width for iOS portrait (logical units). */
    static constexpr int SIDEBAR_EXPANDED_WIDTH_NARROW = 122;
    
    /** Collapsed sidebar width (logical units). */
    static constexpr int SIDEBAR_COLLAPSED_WIDTH = 36;
    
    // Sidebar button positions - collapsed state
    static constexpr int SIDEBAR_X_COLLAPSED = 14;
    static constexpr int SIDEBAR_Y_COLLAPSED = 13;
    
    // Sidebar button positions - expanded state
    static constexpr int SIDEBAR_X_EXPANDED = 12;
    static constexpr int SIDEBAR_Y_EXPANDED = 13;
    
    // Settings button position
    static constexpr int SIDEBAR_X_SETTINGS = 12;
    static constexpr int SIDEBAR_Y_SETTINGS = 11;
    
    // Play button positions
    static constexpr int SIDEBAR_X_PLAY_COLLAPSED = 12;
    static constexpr int SIDEBAR_Y_PLAY_COLLAPSED = 44;
    static constexpr int SIDEBAR_X_PLAY_EXPANDED = 40;
    static constexpr int SIDEBAR_Y_PLAY_EXPANDED = 11;
    
    // Visualization button positions
    static constexpr int SIDEBAR_X_VISUALIZATION_COLLAPSED = 12;
    static constexpr int SIDEBAR_Y_VISUALIZATION_COLLAPSED = 78;
    static constexpr int SIDEBAR_X_VISUALIZATION_EXPANDED = 68;
    static constexpr int SIDEBAR_Y_VISUALIZATION_EXPANDED = 11;
    
    // Reset button positions
    static constexpr int SIDEBAR_X_RESET_COLLAPSED = 12;
    static constexpr int SIDEBAR_Y_RESET_COLLAPSED = 46;
    static constexpr int SIDEBAR_X_RESET_EXPANDED = 39;
    static constexpr int SIDEBAR_Y_RESET_EXPANDED = 11;
    
    // Help button position
    static constexpr int SIDEBAR_X_HELP = 12;
    static constexpr int SIDEBAR_Y_HELP = 12;

    // =================================================================
    // PORT LAYOUT IN SIDEBAR
    // =================================================================

    /** Positions for port list in sidebar. */
    static constexpr int SIDEBAR_X_PORTLIST = 12;
    static constexpr int SIDEBAR_Y_PORTLIST = 24;

    /** Positions for visibility toggle in port list. */
    static constexpr int PORTLIST_Y_VISIBILITY = -2;
    static constexpr int PORTLIST_X_VISIBILITY = 10;

    /** Vertical spacing between ports in port list. */
    static constexpr int PORTLIST_PORT_Y_SPACING = 14;

    /** Horizontal position for port name in port list. */
    static constexpr int PORTLIST_X_PORT = 34;
    
    /** Margins for port names in port list. */
    static constexpr int PORTLIST_PORT_RIGHT_MARGIN = 8;

    /** Bottom margin for port list in sidebar. */
    static constexpr int SIDEBAR_PORTLIST_BOTTOM_MARGIN = 36;



    // =================================================================
    // BUTTON LAYOUT
    // =================================================================
    
    /** Default touch outset for buttons (increases touch target size). */
    static constexpr int BUTTON_DEFAULT_TOUCH_OUTSET = 12;

    // =================================================================
    // SCROLLBAR LAYOUT
    // =================================================================
    
    /** Thickness of scrollbars in viewports. */
    static constexpr int SCROLLBAR_THICKNESS = 8;

    // =================================================================
    // MIDI DEVICE LAYOUT
    // =================================================================
    
    /** Spacing between MIDI device components. */
    static constexpr int MIDI_DEVICE_SPACING = 2;

    // =================================================================
    // POPUP WINDOWS
    // =================================================================
    
    /** Default width for color selector popup. */
    static constexpr int POPUP_COLOR_SELECTOR_WIDTH = 300;
    
    /** Default height for color selector popup. */
    static constexpr int POPUP_COLOR_SELECTOR_HEIGHT = 400;

    // =================================================================
    // SETTINGS COMPONENT LAYOUT
    // =================================================================
    
    /** Left margin for settings content. */
    static constexpr int SETTINGS_LEFT_MARGIN = 14;
    
    /** Gap between settings buttons (edge to edge). */
    static constexpr int SETTINGS_BUTTON_GAP = 16;
    
    /** Gap for 4-column button layouts (edge to edge). */
    static constexpr int SETTINGS_BUTTON_GAP_4COL = 13;
    
    /** Close button position. */
    static constexpr int SETTINGS_CLOSE_BUTTON_RIGHT = 12;
    static constexpr int SETTINGS_CLOSE_BUTTON_TOP = 3;
    static constexpr int SETTINGS_CLOSE_BUTTON_WIDTH = 28;

    // =================================================================
    // SEPARATOR ELEMENTS
    // =================================================================
    
    /** Horizontal position for separator lines. */
    static constexpr int X_SEPARATOR = 1;
    
    /** Vertical position for separator lines. */
    static constexpr int Y_SEPARATOR = 2;
    
    /** Height of separator lines. */
    static constexpr int HEIGHT_SEPARATOR = 1;
    
    /** Height of indicator elements. */
    static constexpr int HEIGHT_INDICATOR = 1;

    // =================================================================
    // MIDI DEVICE COMPONENT LAYOUT
    // =================================================================

    /** Positions for port labels. */
    static constexpr int X_PORT = 24;
    static constexpr int Y_PORT = 12;

    // =================================================================
    // CLOCK DISPLAY
    // =================================================================
    
    /** Horizontal position for clock labels. */
    static constexpr int X_CLOCK = 23;
    
    /** Vertical position for clock labels. */
    static constexpr int Y_CLOCK = 12;
    
    /** Horizontal position for BPM display. */
    static constexpr int X_CLOCK_BPM = 24;
    
    /** Vertical padding for clock elements. */
    static constexpr int Y_CLOCK_PADDING = 8;

    // =================================================================
    // SYSEX DISPLAY
    // =================================================================
    
    /** Horizontal position for SysEx labels. */
    static constexpr int X_SYSEX = 23;
    
    /** Vertical position for SysEx labels. */
    static constexpr int Y_SYSEX = 12;
    
    /** Horizontal position for SysEx length display. */
    static constexpr int X_SYSEX_LENGTH = 24;
    
    /** Vertical padding for SysEx elements. */
    static constexpr int Y_SYSEX_PADDING = 8;
    
    /** Horizontal position for SysEx data display. */
    static constexpr int X_SYSEX_DATA = 75;
    
    /** Width of individual SysEx data elements. */
    static constexpr int X_SYSEX_DATA_WIDTH = 31;

    // =================================================================
    // CHANNEL DISPLAY
    // =================================================================
    
    /** Horizontal position for channel labels. */
    static constexpr int X_CHANNEL = 23;
    
    /** Horizontal position for MPE channel display. */
    static constexpr int X_CHANNEL_MPE = 84;
    
    /** Horizontal position for MPE type display. */
    static constexpr int X_CHANNEL_MPE_TYPE = 110;
    
    /** Vertical position for channel labels. */
    static constexpr int Y_CHANNEL = 12;
    
    /** Vertical margin for channel elements. */
    static constexpr int Y_CHANNEL_MARGIN = 8;
    
    /** Vertical padding for channel elements. */
    static constexpr int Y_CHANNEL_PADDING = 8;

    // =================================================================
    // PROGRAM CHANGE DISPLAY
    // =================================================================
    
    /** Horizontal position for program change display. */
    static constexpr int X_PRGM = 24;

    // =================================================================
    // PITCH BEND DISPLAY
    // =================================================================
    
    /** Horizontal position for pitch bend labels. */
    static constexpr int X_PB = 84;
    
    /** Vertical position for pitch bend labels. */
    static constexpr int Y_PB = 7;
    
    /** Horizontal position for pitch bend data. */
    static constexpr int X_PB_DATA = 24;

    // =================================================================
    // PARAMETER DISPLAY
    // =================================================================
    
    /** Horizontal position for parameter labels. */
    static constexpr int X_PARAM = 84;
    
    /** Vertical position for parameter labels. */
    static constexpr int Y_PARAM = 7;
    
    /** Horizontal position for parameter data. */
    static constexpr int X_PARAM_DATA = 24;

    // =================================================================
    // NOTE DISPLAY
    // =================================================================
    
    /** Horizontal position for note labels. */
    static constexpr int X_NOTE = 48;
    
    /** Vertical position for note labels. */
    static constexpr int Y_NOTE = 7;
    
    /** Horizontal position for note on/off display. */
    static constexpr int X_ON_OFF = 84;
    
    /** Horizontal position for note data display. */
    static constexpr int X_NOTE_DATA = X_MID - 6;

    // =================================================================
    // POLYPHONIC PRESSURE DISPLAY
    // =================================================================
    
    /** Horizontal position for polyphonic pressure labels. */
    static constexpr int X_PP = 84;
    
    /** Vertical position for polyphonic pressure labels. */
    static constexpr int Y_PP = 7;
    
    /** Horizontal position for polyphonic pressure data. */
    static constexpr int X_PP_DATA = X_MID - 6;

    // =================================================================
    // CONTROL CHANGE DISPLAY
    // =================================================================
    
    /** Horizontal position for control change labels. */
    static constexpr int X_CC = X_MID + 6;
    
    /** Vertical position for control change labels. */
    static constexpr int Y_CC = 7;
    
    /** Horizontal position for control change data. */
    static constexpr int X_CC_DATA = 24;

} // namespace showmidi::layout
