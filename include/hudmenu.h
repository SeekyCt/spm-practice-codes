/*
    HudMenu
    Menu window to configure HUD displays
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuscrollerh.h"
#include "childmenu.h"
#include "nandsettings.h"

namespace mod {

class HudMenu final : public ChildMenu
{
private:
    char mDelayStr[9];
    char mDecimalPlaceStr[2];

    void updateDelayStr();
    void updateDecimalPlaceStr();
    void optionToggle(MenuButton * button, bool &setting);
    void optionDelta(s8 &setting, int change);

public:
    HudMenu();
};

}
