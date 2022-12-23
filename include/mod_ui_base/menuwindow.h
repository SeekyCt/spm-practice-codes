/*
    MenuWindow
    Abstract menu window
*/

#pragma once

#include <common.h>

#include "bgwindow.h"
#include "menubutton.h"

namespace mod {

class MenuWindow : public BgWindow
{
protected:
    friend class MenuButton;
    const char * mTitle;
    class MenuButton * mButtons;
    class MenuButton * mCurButton;

    virtual void close();
    virtual void disp() override;
    f32 getCentreAlignX(const char * msg, f32 scale);

    static void buttonLinkVertical(class MenuButton * top, class MenuButton * bottom);
    static void buttonLinkHorizontal(class MenuButton * left, class MenuButton * right);

public:
    // Window::mPosX/Y are used for top left corner of window

    static MenuWindow * sCurMenu;
    virtual void fullClose();

    MenuWindow();
    ~MenuWindow();
    void drawStringCentre(const char * str, f32 y, const wii::gx::GXColor * colour = nullptr, f32 scale = 1.0f,
                          bool edge = false, bool noise = false, bool rainbow = false);
};

}
