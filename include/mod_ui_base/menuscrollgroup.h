/*
    MenuScrollGroup
    Multi digit MenuScroller
*/

#pragma once

#include <common.h>

#include "menubutton.h"

namespace mod {

typedef void (MenuScrollGroupCb)(class MenuScrollGroup * scroller, s32 delta, void * param);

class MenuScrollGroup : public MenuButton
{
private:
    virtual void handleInputs(u32 btn, u32 btnRpt) override;
    virtual void disp(f32 centreX, f32 centreY, bool selected) override;

public:
    MenuScrollGroupCb * mScrollCb;
    void * mScrollParam;
    s32 mDigits;
    s32 mCurDigit;
    u32 mDispValue;
    s32 mBase;

    MenuScrollGroup(class MenuWindow * parent, u32 value, f32 x, f32 y, MenuScrollGroupCb * scrollCb,
                    void * scrollParam, s32 digits, bool hex, MenuAction * action = nullptr,
                    void * actionParam = nullptr, f32 scale = 1.0f, wii::gx::GXColor * colour = nullptr,
                    wii::gx::GXColor * selColour = nullptr);
};

}
