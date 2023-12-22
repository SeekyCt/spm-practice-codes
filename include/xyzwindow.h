/*
    XYZWindow
    Window that displays the current player position
*/
#pragma once

#include <common.h>

#include "mod_ui_base/window.h"

namespace mod {

class XYZWindow final : public Window
{
protected:
    virtual void disp() override;
    char mMsgs[3][32];
    wii::gx::GXColor mColour;
    f32 mScale;
    int mFrameCount;

    XYZWindow();
    
public:
    // Window::mPosX/Y are used for top left corner of window

    static XYZWindow * sInstance;

    static void init();
};

}
