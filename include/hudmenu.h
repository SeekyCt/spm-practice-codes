/*
    HudMenu
    Menu window to configure HUD displays
*/

#pragma once

#include "mod_ui_base/menuwindow.h"
#include "mod_ui_base/menuscrollerh.h"
#include "nandsettings.h"

namespace mod {

class HudMenu : public MenuWindow
{
private:
    char mDelayStr[9];
    char mDecimalPlaceStr[2];

    void updateDelayStr();
    void updateDecimalPlaceStr();
    void optionToggle(MenuButton * button, bool &setting);
    void optionDelta(s8 &setting, int change);
    static bool xyzIntervalToggle(MenuButton * button, void * param);
    static void xyzDPUp(MenuScrollerH * scroller, void * param);
    static void xyzDPDown(MenuScrollerH * scroller, void * param);

protected:
    virtual void close() override;

public:
    HudMenu();
};

}
