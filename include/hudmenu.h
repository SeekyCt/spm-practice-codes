/*
    HudMenu
    Menu window to configure HUD displays
*/

#pragma once

#include "mod_ui_base/menuwindow.h"
#include "nandsettings.h"

namespace mod {

struct HudOptionGroup
{
    MenuButton * label;
    MenuButton * value;
};

class HudMenu : public MenuWindow
{
private:
    HudOptionGroup mOptions[HUD_OPTION_COUNT];

    static bool optionToggle(MenuButton * button, void * param);

protected:
    virtual void close() override;

public:
    HudMenu();
};

}
