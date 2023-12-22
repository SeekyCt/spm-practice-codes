/*
    LogMenu
    Menu window to configure logging patches
*/

#pragma once

#include <common.h>

#include "childmenu.h"
#include "nandsettings.h"
#include "scriptlog.h"

namespace mod {

struct LogOptionGroup
{
    MenuButton * label;
    MenuButton * value;
};

class LogMenu final : public ChildMenu
{
private:
    LogOptionGroup mOptions[LOG_OPTION_COUNT];

    static bool optionCycle(MenuButton * button, void * param);

public:
    LogMenu();
};

}
