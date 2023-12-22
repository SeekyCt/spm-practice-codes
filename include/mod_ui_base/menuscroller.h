/*
    MenuScroller
    MenuButton that cycles through values on up and down input
*/

#pragma once

#include <common.h>

#include "menubutton.h"

namespace mod {

typedef void (MenuScrollCb)(class MenuScroller * scroller, void * param);

class MenuScroller : public MenuButton
{
private:
    virtual void handleInputs(u32 btn, u32 btnRpt) override;
    virtual void disp(f32 centreX, f32 centreY, bool selected) override;

public:
    MenuScrollCb * mScrollUpCb;
    MenuScrollCb * mScrollDownCb;
    void * mScrollParam;
    f32 mArrowXDiff;

    MenuScroller(class MenuWindow * parent, const char * msg, f32 x, f32 y, f32 arrowXDiff,
                 MenuScrollCb * scrollUp, MenuScrollCb * scrollDown, void * scrollParam,
                 MenuAction * action = nullptr, void * actionParam = nullptr,
                 f32 scale = 1.0f, wii::gx::GXColor * colour = nullptr, wii::gx::GXColor * selColour = nullptr);
};

}
