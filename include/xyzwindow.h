/*
    XYZWindow
    Window that displays the current player position
*/
#pragma once

#include "mod_ui_base/window.h"

namespace mod {

class XYZWindow : public Window
{
protected:
    virtual void disp() override;
    char mMsgX[16];
    char mMsgY[16];
    char mMsgZ[16];
    wii::RGBA mColour;
    f32 mScale;
    u32 mFrameCount;
    u32 mFrameDelay;
    
public:
    // Window::mPosX/Y are used for top left corner of window

    static XYZWindow * sInstance;

    XYZWindow();    
};

}
