/*
    Centred Button
    MenuButton that draws text centred on the x-axis
*/

#pragma once

#include "menubutton.h"

namespace mod {

class CentredButton : public MenuButton
{
protected:
    virtual void disp(f32 centreX, f32 centreY, bool selected);

public:
    CentredButton(class MenuWindow * parent, const char * msg, f32 y,
                  MenuAction * action = nullptr, void * actionParam = nullptr,
                  bool autoPress = false, f32 scale = 1.0f,
                  wii::gx::GXColor * colour = nullptr, wii::gx::GXColor * selColour = nullptr);
};

}
