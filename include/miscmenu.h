/*
    MiscMenu
    Menu window to configure settings less related to speedrunning / RE
*/

#pragma once

#include "mod_ui_base/menuwindow.h"
#include "mod_ui_base/menuscrollerh.h"
#include "nandsettings.h"

namespace mod {

#define MISC_SETTING_COUNT 1

class MiscMenu : public MenuWindow
{
private:
    MenuButton * mOptions[MISC_SETTING_COUNT];
    void optionToggle(MenuButton * button, bool &setting);

protected:
    virtual void close() override;
    virtual void disp() override;

public:
    MiscMenu();
};

}
