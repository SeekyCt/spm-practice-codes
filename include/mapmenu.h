/*
    MapMenu
    Abstract menu window which displays the teleport effect toggle option
*/

#pragma once

#include <common.h>

#include "mod_ui_base/passivebutton.h"
#include "childmenu.h"

namespace mod {

class MapMenu : public ChildMenu
{
private:
    PassiveButton * mWarpButton;
    static const char * getWarpEffectMsg();
    static bool toggleWarpEffect(MenuButton * button, void * param);

protected:
    MapMenu();
};

}
