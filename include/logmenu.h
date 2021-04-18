/*
    LogMenu
    Menu window to configure logging patches
*/

#pragma once

#include "mod_ui_base/menuwindow.h"
#include "nandsettings.h"
#include "scriptlog.h"

namespace mod {

struct LogOptionGroup
{
    MenuButton * label;
    MenuButton * value;
};

class LogMenu : public MenuWindow
{
private:
    LogOptionGroup mOptions[LOG_OPTION_COUNT];

    static bool optionCycle(MenuButton * button, void * param);

protected:
    virtual void close() override;

public:
    LogMenu();
};

}
