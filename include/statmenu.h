/*
    StatMenu
    Menu window to edit values like HP and coins
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuscrollerh.h"
#include "childmenu.h"

namespace mod {

#define STAT_COUNT 7

struct StatOptionGroup
{
    MenuButton * label;
    MenuScrollerH * value;
};

class StatMenu final : public ChildMenu
{
private:
    StatOptionGroup mStats[STAT_COUNT];
    int mIncrement;
    bool mShouldClose;
    char mStatStrs[STAT_COUNT][9];
    char mIncrementStr[91];

    int getValue(int stat);
    void setValue(int stat, int value);
    void updateStatDisps();
    static void valueUp(MenuScrollerH * button, void * param);
    static void valueDown(MenuScrollerH * button, void * param);

protected:
    virtual void close() override;
    virtual void disp() override;

public:
    StatMenu();
};

}
