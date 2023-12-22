/*
    MapSelectMenu
    Menu window to change the current map
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuscroller.h"
#include "mapmenu.h"

namespace mod {

class MapSelectMenu final : public MapMenu
{
private:
    int mGroup;
    int mMap;
    int mEntrance;
    char mMapStr[3];
    MenuScroller * mGroupDisp;
    MenuScroller * mMapDisp;
    MenuScroller * mEntranceDisp;

    static bool playEffectThisMapChange;

    void updateGroupDisp();
    void updateMapDisp();
    void updateEntranceDisp();
    void updateEffectDisp();
    void _doMapChange();

    static void groupUp(MenuScroller * scroller, void * param);
    static void groupDown(MenuScroller * scroller, void * param);
    static void mapUp(MenuScroller * scroller, void * param);
    static void mapDown(MenuScroller * scroller, void * param);
    static void entranceUp(MenuScroller * scroller, void * param);
    static void entranceDown(MenuScroller * scroller, void * param);
    static bool doMapChange(MenuButton * button, void * param);

public:
    static char sFullMapStr[7];
    static char sDoorStr[32];

    MapSelectMenu();
    static void onMapChanged();
    static void scanEntrances();
};

}
