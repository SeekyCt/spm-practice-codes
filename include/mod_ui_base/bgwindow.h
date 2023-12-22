/*
    BgWindow
    Abstract window class that draws a background box
*/

#pragma once

#include <common.h>
#include <wii/gx.h>

#include "window.h"

namespace mod {

class BgWindow : public Window
{
protected:
    virtual void disp() override;
    
public:
    // Window::mPosX/Y are used for top left corner of window
    f32 mHeight, mWidth;
    f32 mCurve;
    wii::gx::GXColor mColour; // note: if RGB are all the same, game forces it max white

    BgWindow();
};

}
