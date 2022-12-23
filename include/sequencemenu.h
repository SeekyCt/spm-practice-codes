/*
    SequenceMenu
    Menu window to view and change sequence position
*/

#pragma once

#include <common.h>

#include "mod_ui_base/centredbutton.h"
#include "mod_ui_base/menuscrollgroup.h"
#include "childmenu.h"

namespace mod {

class SequenceMenu final : public ChildMenu
{
private:
    CentredButton * mLevelDisp;
    MenuScrollGroup * mScroller;

    const char * getLevel();
    s32 getVal();
    void setVal(s32 val);

    static void valChange(MenuScrollGroup * scroller, s32 delta, void * param);

    virtual void close() override;

public:
    virtual void fullClose() override;

    SequenceMenu();
};

}
