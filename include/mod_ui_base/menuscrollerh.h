/*
    MenuScrollerH
    Horizontal MenuScroller
*/

#pragma once

#include <common.h>

#include "menubutton.h"

namespace mod {

typedef void (MenuScrollHCb)(class MenuScrollerH * scroller, void * param);

class MenuScrollerH : public MenuButton
{
private:
    virtual void handleInputs(u32 btn, u32 btnRpt) override;
    virtual void disp(f32 centreX, f32 centreY, bool selected) override;

public:
    MenuScrollHCb * mScrollLeftCb, * mScrollRightCb;
    void * mScrollParam;
    f32 mArrowXDiff;

    MenuScrollerH(class MenuWindow * parent, const char * msg, f32 x, f32 y, f32 arrowXDiff,
                  MenuScrollHCb * scrollLeft, MenuScrollHCb * scrollRight, void * scrollParam,
                  MenuAction * action = nullptr, void * actionParam = nullptr, f32 scale = 1.0f,
                  wii::gx::GXColor * colour = nullptr, wii::gx::GXColor * selColour = nullptr);
};

}
