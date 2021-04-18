/*
    MenuButton
    Interactable text element of a window
*/

#pragma once

#include "menuwindow.h"

namespace mod {

enum
{
    MENUDIR_UP = 0,
    MENUDIR_DOWN,
    MENUDIR_LEFT,
    MENUDIR_RIGHT
};

// Return true if button still exists, false if it was deleted
typedef bool (MenuAction)(class MenuButton * button, void * param);

class MenuButton
{
protected:
    friend class MenuWindow;
    class MenuWindow * mWindow;
    MenuButton * mNext;
    MenuButton * mNeighbours[4];

    virtual void handleInputs(u32 btn, u32 btnRpt);
    virtual void disp(f32 centreX, f32 centreY, bool selected);

public:
    const char * mMsg;
    f32 mPosX, mPosY;
    f32 mScale;
    wii::RGBA mColour;
    wii::RGBA mSelColour;
    MenuAction * mAction;
    void * mActionParam;
    bool mAutoPress;

    MenuButton(class MenuWindow * parent, const char * msg, f32 x, f32 y,
               MenuAction * action = nullptr, void * actionParam = nullptr,
               bool autoPress = false, f32 scale = 1.0f,
               wii::RGBA * colour = nullptr, wii::RGBA * selColour = nullptr);
    virtual ~MenuButton();
};

}
