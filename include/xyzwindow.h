/*
    XYZWindow
    Window that displays the current player position
*/
#pragma once

#include "mod_ui_base/window.h"

namespace mod {

class XYZWindow final : public Window
{
protected:
    virtual void disp() override;
    char mMsgs[3][32];
    wii::RGBA mColour;
    f32 mScale;
    int mFrameCount;
    
public:
    // Window::mPosX/Y are used for top left corner of window

    static XYZWindow * sInstance;

    XYZWindow();    
};

}
