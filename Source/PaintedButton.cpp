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
#include "AboutComponent.h"

#include "DpiScaling.h"
#include "PaintedButton.h"

namespace showmidi
{
    PaintedButton::PaintedButton() : Button({})
    {
    }
    
    PaintedButton::PaintedButton(const String& name) : Button(name)
    {
    }
    
    void PaintedButton::paintButton(Graphics&, bool, bool)
    {
        // no-op
    }

    void PaintedButton::setBoundsForTouch(int x, int y, int w, int h)
    {
        auto bounds = Rectangle<int> {x, y, w, h};
        auto touchOutset = sm::scaled(sm::layout::BUTTON_DEFAULT_TOUCH_OUTSET, *this);
        Component::setBounds(bounds.expanded(touchOutset));
    }

    Rectangle<float> PaintedButton::getBoundsForDrawing()
    {
        auto touchOutset = sm::scaled(sm::layout::BUTTON_DEFAULT_TOUCH_OUTSET, *this);
        return getBounds().reduced(touchOutset).toFloat();
    }

    void PaintedButton::drawName(Graphics& g, Justification justificationType)
    {
        auto touchOutset = sm::scaled(sm::layout::BUTTON_DEFAULT_TOUCH_OUTSET, *this);
        auto bounds = getBounds().reduced(touchOutset);
        g.drawText(getName(),
                   bounds.getX(), bounds.getY(),
                   bounds.getWidth(), bounds.getHeight(),
                   justificationType);
    }
    
    void PaintedButton::drawDrawable(Graphics& g, Drawable& drawable)
    {
        auto touchOutset = sm::scaled(sm::layout::BUTTON_DEFAULT_TOUCH_OUTSET, *this);
        auto bounds = getBounds().reduced(touchOutset);
        drawable.drawAt(g, (float)bounds.getX(), (float)bounds.getY(), 1.0f);
    }
}
