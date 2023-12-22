#include <common.h>
#include <spm/fontmgr.h>
#include <wii/wpad.h>

#include "mod_ui_base/menuscrollerh.h"

namespace mod {

MenuScrollerH::MenuScrollerH(MenuWindow * parent, const char * msg, f32 x, f32 y, f32 arrowXDiff, MenuScrollHCb * scrollLeft,
                             MenuScrollHCb * scrollRight, void * scrollParam, MenuAction * action, void * actionParam,
                             f32 scale, wii::gx::GXColor * colour, wii::gx::GXColor * selColour)
                            :MenuButton(parent, msg, x, y, action, actionParam, false, scale, colour, selColour)
{
    // Store extra parameters
    mScrollLeftCb = scrollLeft;
    mScrollRightCb = scrollRight;
    mScrollParam = scrollParam;
    mArrowXDiff = arrowXDiff;
}

void MenuScrollerH::handleInputs(u32 btn, u32 btnRpt)
{
    // Trigger scroll cbs if moving left or right
    if (btnRpt & WPAD_BTN_UP)
        mScrollLeftCb(this, mScrollParam);
    if (btnRpt & WPAD_BTN_DOWN)
        mScrollRightCb(this, mScrollParam);

    // Carry out default behaviour for other inputs
    btn &= ~(WPAD_BTN_UP | WPAD_BTN_DOWN);
    btnRpt &= ~(WPAD_BTN_UP | WPAD_BTN_DOWN);
    MenuButton::handleInputs(btn, btnRpt);
}

void MenuScrollerH::disp(f32 centreX, f32 centreY, bool selected)
{
    const f32 width = spm::fontmgr::FontGetMessageWidth(mMsg) * mScale;
    const f32 offset = width / 2;
    const f32 x = centreX + mPosX;
    const f32 y = centreY + mPosY;
    const f32 scale = 0.8f * mScale;
    wii::gx::GXColor * colour = selected ? &mSelColour : &mColour;
    mWindow->drawString("+", x + mArrowXDiff, y, colour, scale, true);
    mWindow->drawString("-", x - mArrowXDiff, y, colour, scale, true);
    mWindow->drawString(mMsg, x - offset, mPosY + centreY, colour, mScale, true);
}

}
