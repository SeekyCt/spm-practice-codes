#include <common.h>
#include <spm/fontmgr.h>
#include <spm/hud.h>
#include <spm/pausewin.h>
#include <spm/homebuttondrv.h>
#include <spm/wpadmgr.h>

#include "mod_ui_base/colours.h"
#include "mod_ui_base/menuwindow.h"

namespace mod {

MenuWindow * MenuWindow::sCurMenu = nullptr;

MenuWindow::MenuWindow()
{
    // Init background properties
    mPosX = -335.0f;
    mPosY = 200.0f;
    mWidth = 670.0f;
    mHeight = 400.0f;
    mCurve = 0.0f;
    mColour = {0, 0, 1, 0xff};

    // Set title - should be replaced by inheriting classes
    mTitle = "Abstract Menu Window";

    // Initialise buttons
    //  - current button should be replaced by inheriting classes
    //  - mButtons is automatically handled by button ctor
    mButtons = nullptr;
    mCurButton = nullptr;

    // Pause game, hide hud and disable home menu
    spm::pausewin::pausewinPauseGame();
    spm::hud::hudHide();
    spm::homebuttondrv::homebuttondrv_wp->flags |= HOMEBUTTON_FLAG_FORBIDDEN;
}

MenuWindow::~MenuWindow()
{
    // List pointer set to null so that buttons know not to remove themselves from it
    MenuButton * p = mButtons;
    mButtons = nullptr;

    // Clean up buttons
    while (p != nullptr)
    {
        MenuButton * temp = p;
        p = temp->mNext;
        delete temp;
    }

    // Set game back to original pause state and re-enable home menu
    spm::pausewin::pausewinUnpauseGame();
    spm::homebuttondrv::homebuttondrv_wp->flags &= ~HOMEBUTTON_FLAG_FORBIDDEN;
}

f32 MenuWindow::getCentreAlignX(const char * str, f32 scale)
{
    // Half the text left of centre x, half the text right
    f32 centreX = mPosX + (mWidth / 2);
    f32 width = spm::fontmgr::FontGetMessageWidth(str) * scale;
    return centreX - (width / 2);
}

void MenuWindow::drawStringCentre(const char * str, f32 y, const wii::gx::GXColor * colour,
                                  f32 scale, bool edge, bool noise, bool rainbow)
{
    f32 x = getCentreAlignX(str, scale);
    drawString(str, x, y, colour, scale, edge, noise, rainbow);
}

void MenuWindow::disp()
{
    // Draw background
    BgWindow::disp();

    // Draw title
    drawStringCentre(mTitle, mPosY - 5, &colours::white, 0.9, true, false, false);

    // Render buttons
    f32 centreX = mPosX + (mWidth / 2);
    f32 centreY = mPosY - (mHeight / 2);
    for (MenuButton * p = mButtons; p != nullptr; p = p->mNext)
        p->disp(centreX, centreY, p == mCurButton);

    u32 btn = spm::wpadmgr::wpadGetButtonsPressed(0);
    u32 btnRpt = spm::wpadmgr::wpadGetButtonsHeldRepeat(0);

    MenuButton * prevButton = mCurButton;
    // Handle inputs on current button
    if (mCurButton != nullptr)
        mCurButton->handleInputs(btn, btnRpt);
    
    if (prevButton != mCurButton && mCurButton != nullptr)
    {
        if (mCurButton->mOnSelectAction != nullptr)
            mCurButton->mOnSelectAction(mCurButton, mCurButton->mOnSelectActionParam);
    }

    // Handle other buttons listening for inputs
    for (MenuButton * p = mButtons; p != nullptr; p = p->mNext)
        p->handlePassiveInputs(btn, btnRpt);
}

void MenuWindow::close()
{
    sCurMenu = nullptr;
    delete this;
}

void MenuWindow::fullClose()
{
    sCurMenu = nullptr;
    delete this;
}

void MenuWindow::buttonLinkVertical(MenuButton * top, MenuButton * bottom)
{
    top->mNeighbours[MENUDIR_DOWN] = bottom;
    bottom->mNeighbours[MENUDIR_UP] = top;
}

void MenuWindow::buttonLinkHorizontal(MenuButton * left, MenuButton * right)
{
    left->mNeighbours[MENUDIR_RIGHT] = right;
    right->mNeighbours[MENUDIR_LEFT] = left;
}

}
