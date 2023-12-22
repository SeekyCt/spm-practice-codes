/*
    ScriptVarMenu
    Menu window to view and change evt variables
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuscroller.h"
#include "mod_ui_base/menuscrollgroup.h"
#include "childmenu.h"

namespace mod {

class ScriptVarMenu final : public ChildMenu
{
private:
    int mGroup;
    int mId;
    MenuScroller * mGroupDisp;
    MenuScrollGroup * mSelectScroller;
    MenuScrollGroup * mEditScroller;

    void updateGroupDisp();
    void updateVarDisp();
    u32 getVal();
    void setVal(u32 val);

    static void groupUp(MenuScroller * scroller, void * param);
    static void groupDown(MenuScroller * scroller, void * param);
    static void idChange(MenuScrollGroup * scroller, s32 delta, void * param);
    static void valChange(MenuScrollGroup * scroller, s32 delta, void * param);

    virtual void close() override;

public:
    virtual void fullClose() override;

    ScriptVarMenu();
};

}
