/*
    TitleTextWindow
    Window that prints text at the top of the title screen
*/

#pragma once

#include <common.h>

#include "mod_ui_base/window.h"

namespace mod {

class TitleTextWindow final : public Window
{
protected:
    const char * mMsg;
    wii::gx::GXColor mColour;
    f32 mScale;

    virtual void disp() override;
    TitleTextWindow();
    
public:
    // Window::mPosX/Y are used for top left corner of window
    static TitleTextWindow * sInstance;

    static void init();
};

}
