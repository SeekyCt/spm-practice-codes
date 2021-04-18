/*
    ScriptVarMenu
    Menu window to view and change evt variables
*/

#pragma once

#include "mod_ui_base/menuscroller.h"
#include "mod_ui_base/menuwindow.h"

namespace mod {

class ScriptVarMenu : public MenuWindow
{
private:
    int mGroup;
    int mId;
    MenuScroller * mGroupDisp;
    MenuScroller * mSelectScrollers[4];
    char mSelectStrs[4][2]; // 1 digit + null
    MenuScroller * mEditScrollers[8];
    char mEditStrs[8][2]; // 1 digit + null

    void updateGroupDisp();
    void updateVarDisp();
    u32 getVal();
    void setVal(u32 val);

    static void groupUp(MenuScroller * scroller, void * param);
    static void groupDown(MenuScroller * scroller, void * param);
    static void idUp(MenuScroller * scroller, void * param);
    static void idDown(MenuScroller * scroller, void * param);
    static void valUp(MenuScroller * scroller, void * param);
    static void valDown(MenuScroller * scroller, void * param);

protected:
    virtual void close() override;

public:
    ScriptVarMenu();
};

}
