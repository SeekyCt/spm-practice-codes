#include <common.h>
#include <wii/wpad.h>

#include "mod_ui_base/menuscroller.h"

namespace mod {

MenuScroller::MenuScroller(MenuWindow * parent, const char * msg, f32 x, f32 y, f32 arrowXDiff, MenuScrollCb * scrollUp, 
                           MenuScrollCb * scrollDown, void * scrollParam, MenuAction * action, void * actionParam,
                           f32 scale, wii::gx::GXColor * colour, wii::gx::GXColor * selColour)
                          :MenuButton(parent, msg, x, y, action, actionParam, false, scale, colour, selColour)
{
    // Store extra parameters
    mScrollUpCb = scrollUp;
    mScrollDownCb = scrollDown;
    mScrollParam = scrollParam;
    mArrowXDiff = arrowXDiff;
}

void MenuScroller::handleInputs(u32 btn, u32 btnRpt)
{
    // Trigger scroll cbs if moving up or down
    if (btnRpt & WPAD_BTN_RIGHT)
        mScrollUpCb(this, mScrollParam);
    if (btnRpt & WPAD_BTN_LEFT)
        mScrollDownCb(this, mScrollParam);

    // Carry out default behaviour for other inputs
    btn &= ~(WPAD_BTN_RIGHT | WPAD_BTN_LEFT);
    btnRpt &= ~(WPAD_BTN_RIGHT | WPAD_BTN_LEFT);
    MenuButton::handleInputs(btn, btnRpt);
}

void MenuScroller::disp(f32 centreX, f32 centreY, bool selected)
{
    // Draw arrows
    const f32 x = centreX + mPosX;
    const f32 y = centreY + mPosY;
    const f32 arrowScale = mScale / 2.0f;
    const f32 bottomArrowOffset = (FONT_HEIGHT * mScale) + (FONT_NUM_TOP_GAP * mScale);
    const f32 topArrowOffset = FONT_ARROW_HEIGHT * arrowScale;
    const f32 arrowGap = 10.0f * mScale;
    wii::gx::GXColor * colour = selected ? &mSelColour : &mColour;
    mWindow->drawString("/\\", x + mArrowXDiff, y + topArrowOffset + arrowGap, colour, arrowScale, true);
    mWindow->drawString("\\/", x + mArrowXDiff, y - bottomArrowOffset - arrowGap, colour, arrowScale, true);


    // Draw main button
    MenuButton::disp(centreX, centreY, selected);
}

}
