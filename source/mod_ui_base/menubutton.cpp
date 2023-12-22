#include <common.h>
#include <wii/wpad.h>

#include "mod_ui_base/menubutton.h"

namespace mod {

void MenuButton::handleInputs(u32 btn, u32 btnRpt)
{
    // Handle action
    if (mAction != nullptr && (btn & WPAD_BTN_2 || mAutoPress))
    {
        bool ret = mAction(this, mActionParam);

        // If this button has been deleted, stop
        if (!ret)
            return;
    }

    // Handle movement
    if ((mNeighbours[MENUDIR_UP] != nullptr) && (btnRpt & WPAD_BTN_RIGHT))
        mWindow->mCurButton = mNeighbours[MENUDIR_UP];
    if ((mNeighbours[MENUDIR_DOWN] != nullptr) && (btnRpt & WPAD_BTN_LEFT))
        mWindow->mCurButton = mNeighbours[MENUDIR_DOWN];
    if ((mNeighbours[MENUDIR_LEFT] != nullptr) && (btnRpt & WPAD_BTN_UP))
        mWindow->mCurButton = mNeighbours[MENUDIR_LEFT];
    if ((mNeighbours[MENUDIR_RIGHT] != nullptr) && (btnRpt & WPAD_BTN_DOWN))
        mWindow->mCurButton = mNeighbours[MENUDIR_RIGHT];
    
    // Handle escape
    if (btn & WPAD_BTN_1)
        mWindow->close();
}

void MenuButton::handlePassiveInputs(u32 btn, u32 btnRpt)
{
    (void) btn;
    (void) btnRpt;
}

void MenuButton::disp(f32 centreX, f32 centreY, bool selected)
{
    wii::gx::GXColor * colour = selected ? &mSelColour : &mColour;
    if (mDrawAsMessage)
        mWindow->drawMessage(mMsg, mPosX + centreX, mPosY + centreY, colour, mScale, true);
    else
        mWindow->drawString(mMsg, mPosX + centreX, mPosY + centreY, colour, mScale, true);
}

MenuButton::MenuButton(MenuWindow * parent, const char * msg, f32 x, f32 y, MenuAction * action, void * actionParam,
                       bool autoPress, f32 scale, const wii::gx::GXColor * colour, const wii::gx::GXColor * selColour, bool drawAsMessage, MenuAction * onSelectAction, void * onSelectActionParam)
{
    // Record parent and insert into linked list
    mWindow = parent;
    mNext = mWindow->mButtons;
    mWindow->mButtons = this;

    // Init properties
    mMsg = msg;
    mPosX = x;
    mPosY = y;
    mScale = scale;
    if (colour != nullptr)
        mColour = *colour;
    else
        mColour = {0xff, 0xff, 0xff, 0xff};
    if (selColour != nullptr)
        mSelColour = *selColour;
    else
        mSelColour = {0xff, 0, 0, 0xff};
    mAction = action;
    mActionParam = actionParam;
    mAutoPress = autoPress;
    mDrawAsMessage = drawAsMessage;
    mOnSelectAction = onSelectAction;
    mOnSelectActionParam = onSelectActionParam;
    
    // Neighbours may not exist yet so they're left to be set externally
    for (int i = 0; i < 4; i++)
        mNeighbours[i] = nullptr;
}

MenuButton::~MenuButton()
{
    // Null indicates the whole list is being deleted, no need to remove
    if (mWindow->mButtons != nullptr)
    {
        // Remove from linked list
        if (mWindow->mButtons == this)
        {
            // If this is the first button in the list, set the next button as the first
            mWindow->mButtons = mNext;
        }
        else
        {
            // If this isn't the first button, search for the button before it
            MenuButton * prev = mWindow->mButtons;
            while (prev->mNext != this)
                prev = prev->mNext;
            
            // Set the previous buttons's next to this buttons's next
            prev->mNext = mNext;
        }

        // Remove references from neighbours
        if (mNeighbours[MENUDIR_UP] != nullptr)
            mNeighbours[MENUDIR_UP]->mNeighbours[MENUDIR_DOWN] = mNeighbours[MENUDIR_DOWN];
        if (mNeighbours[MENUDIR_DOWN] != nullptr)
            mNeighbours[MENUDIR_DOWN]->mNeighbours[MENUDIR_UP] = mNeighbours[MENUDIR_UP];
        if (mNeighbours[MENUDIR_LEFT] != nullptr)
            mNeighbours[MENUDIR_LEFT]->mNeighbours[MENUDIR_RIGHT] = mNeighbours[MENUDIR_RIGHT];
        if (mNeighbours[MENUDIR_RIGHT] != nullptr)
            mNeighbours[MENUDIR_RIGHT]->mNeighbours[MENUDIR_LEFT] = mNeighbours[MENUDIR_LEFT];
        
        // Make sure the current button doesn't reference this
        if (mWindow->mCurButton == this)
            mWindow->mCurButton = mWindow->mButtons;
    }
}

}
