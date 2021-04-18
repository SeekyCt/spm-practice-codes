/*
    TitleTextWindow
    Window that prints text at the top of the title screen
*/

#pragma once

#include "mod_ui_base/window.h"

namespace mod {

class TitleTextWindow : public Window
{
protected:
    virtual void disp() override;
    const char * mMsg;
    wii::RGBA mColour;
    f32 mScale;
    
public:
    // Window::mPosX/Y are used for top left corner of window
    static TitleTextWindow * sInstance;

    TitleTextWindow();    
};

}
