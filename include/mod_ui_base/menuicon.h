/*
    MenuIcon
    Interactable icon element of a window
*/

#pragma once

#include <common.h>

#include "menubutton.h"

namespace mod {

class MenuIcon : public MenuButton
{
private:
    virtual void disp(f32 centreX, f32 centreY, bool selected) override;

public:
    int mIconId;

    MenuIcon(class MenuWindow * parent, int iconId, f32 x, f32 y,
             MenuAction * action = nullptr, void * actionParam = nullptr,
             bool autoPress = false, f32 scale = 1.0f,
             wii::gx::GXColor * selColour = nullptr);
};

}
