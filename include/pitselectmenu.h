/*
    PitSelectMenu
    Menu window to warp to a floor of a pit of 100 trials
*/

#pragma once

#include "mod_ui_base/menuscroller.h"
#include "mod_ui_base/menuscrollgroup.h"
#include "mapmenu.h"

namespace mod {

class PitSelectMenu : public MapMenu
{
private:
    int mGroup;
    int mFloor;
    MenuScroller * mGroupScroller;
    MenuScrollGroup * mFloorScroller;

    void updateGroupDisp();
    void updateFloorDisp();
    void _doMapChange();

    static void groupSwap(MenuScroller * scroller, void * param);
    static void floorChange(MenuScrollGroup * scroller, s32 delta, void * param);
    static bool doMapChange(MenuButton * button, void * param);

public:
    static const char * sMapName;
    static int sGSW1;

    PitSelectMenu();
};

}
