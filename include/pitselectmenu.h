/*
    PitSelectMenu
    Menu window to warp to a floor of a pit of 100 trials
*/

#pragma once

#include "mod_ui_base/menuscroller.h"
#include "childmenu.h"

namespace mod {

class PitSelectMenu : public ChildMenu
{
private:
    int mGroup;
    int mFloor;
    char mFloorStrs[3][2];
    MenuScroller * mGroupScroller;
    MenuScroller * mFloorScrollers[3];

    void updateGroupDisp();
    void updateFloorDisp();
    void _doMapChange();

    static void groupSwap(MenuScroller * scroller, void * param);
    static void floorUp(MenuScroller * scroller, void * param);
    static void floorDown(MenuScroller * scroller, void * param);
    static bool doMapChange(MenuButton * button, void * param);

protected:
    virtual void disp() override;
    
public:
    static const char * sMapName;
    static int sGSW1;

    PitSelectMenu();
};

}
