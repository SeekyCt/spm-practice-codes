#include <common.h>
#include <msl/stdio.h>
#include <wii/wpad.h>

#include "mod_ui_base/menuscrollgroup.h"
#include "util.h"

namespace mod {

MenuScrollGroup::MenuScrollGroup(MenuWindow * parent, u32 value, f32 x, f32 y, MenuScrollGroupCb * scrollCb,
                                 void * scrollParam, s32 digits, bool hex, MenuAction * action, void * actionParam,
                                 f32 scale, wii::gx::GXColor * colour, wii::gx::GXColor * selColour)
                                : MenuButton(parent, nullptr, x, y, action, actionParam, false, scale, colour, selColour)
{
    // Store extra parameters
    mDispValue = value;
    mScrollCb = scrollCb;
    mScrollParam = scrollParam;
    mDigits = digits;
    mBase = hex ? 16 : 10;
}

void MenuScrollGroup::handleInputs(u32 btn, u32 btnRpt)
{
    // Trigger scroll cbs if moving up or down
    if (btnRpt & (WPAD_BTN_RIGHT | WPAD_BTN_LEFT))
    {
        s32 delta = pow(mBase, mDigits - mCurDigit - 1);
        if (btnRpt & WPAD_BTN_LEFT)
            delta *= -1;
        mScrollCb(this, delta, mScrollParam);
    }

    // Move left a digit if holding left and not on edge
    if ((btnRpt & WPAD_BTN_UP) && mCurDigit > 0)
    {
        mCurDigit--;
        btnRpt &= ~WPAD_BTN_UP;
    }

    // Move right a digit if holding right and not on edge
    if ((btnRpt & WPAD_BTN_DOWN) && mCurDigit < mDigits - 1)
    {
        mCurDigit++;
        btnRpt &= ~WPAD_BTN_DOWN;
    }

    // Carry out default behaviour for other inputs
    btn &= ~(WPAD_BTN_RIGHT | WPAD_BTN_LEFT);
    btnRpt &= ~(WPAD_BTN_RIGHT | WPAD_BTN_LEFT);
    MenuButton::handleInputs(btn, btnRpt);
}

void MenuScrollGroup::disp(f32 centreX, f32 centreY, bool selected)
{
    // Draw scrollers
    u32 remaining = mDispValue;
    f32 x = centreX + mPosX;
    const f32 y = centreY + mPosY;
    const f32 arrowGap = 10.0f * mScale;
    const f32 arrowScale = mScale / 2.0f;
    const f32 bottomArrowOffset = (FONT_HEIGHT * mScale) + (FONT_NUM_TOP_GAP * mScale);
    const f32 topArrowOffset = FONT_ARROW_HEIGHT * arrowScale;
    const f32 arrowXDiff = (FONT_NUM_LEFT_GAP * mScale) + (FONT_NUM_WIDTH * mScale / 2.0f)
                           - (FONT_WIDTH * arrowScale / 2.0f);
    for (s32 i = 0; i < mDigits; i++)
    {
        // Calculate digit value
        u32 placeValue = pow(mBase, mDigits - i - 1);
        u32 digit = remaining / placeValue;
        remaining -= digit * placeValue;

        // Draw digit
        char str[2];
        const char * fmt = mBase == 16 ? "%x" : "%d";
        msl::stdio::sprintf(str, fmt, digit);
        wii::gx::GXColor * colour = i == mCurDigit && selected ? &mSelColour : &mColour;
        mWindow->drawString(str, x, y, colour, mScale, true);

        // Draw arrows
        mWindow->drawString("/\\", x + arrowXDiff, y + topArrowOffset + arrowGap, colour, arrowScale, true);
        mWindow->drawString("\\/", x + arrowXDiff, y - bottomArrowOffset - arrowGap, colour, arrowScale, true);

        // Move to next digit
        x += FONT_WIDTH * mScale;
    }
}

}
