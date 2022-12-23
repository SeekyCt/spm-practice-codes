/*
    MinusButton
    Unselectable MenuButton which triggers when inputs
    from a specific mask are pressed anywhere
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menubutton.h"

namespace mod {

class PassiveButton : public MenuButton
{
protected:
    virtual void handlePassiveInputs(u32 btn, u32 btnRpt) override;

public:
    u32 mPassiveBtnMask;

    PassiveButton(class MenuWindow * parent, const char * msg, f32 x, f32 y,
                  u32 passiveBtnMask, MenuAction * action = nullptr,
                  void * actionParam = nullptr, f32 scale = 1.0f,
                  wii::gx::GXColor * colour = nullptr, bool drawAsMessage = false);
};

}
