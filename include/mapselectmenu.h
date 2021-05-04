/*
    MapSelectMenu
    Menu window to change the current map
*/

#pragma once

#include "mod_ui_base/menuscroller.h"
#include "mod_ui_base/menuwindow.h"

namespace mod {

class MapSelectMenu : public MenuWindow
{
private:
    int mGroup;
    int mMap;
    char mMapStr[3];
    MenuScroller * mGroupDisp;
    MenuScroller * mMapDisp;

    static bool playEffectThisMapChange;

    void updateGroupDisp();
    void updateMapDisp();
    void updateEffectDisp();
    void _doMapChange();

    static void groupUp(MenuScroller * scroller, void * param);
    static void groupDown(MenuScroller * scroller, void * param);
    static void mapUp(MenuScroller * scroller, void * param);
    static void mapDown(MenuScroller * scroller, void * param);
    static bool doMapChange(MenuButton * button, void * param);

protected:
    virtual void disp() override;
    virtual void close() override;

public:
    static char sFullMapStr[7];

    MapSelectMenu();
    static void onMapChanged();
    static void scanEntrances();
};

}
