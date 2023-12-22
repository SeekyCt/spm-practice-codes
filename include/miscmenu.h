/*
    MiscMenu
    Menu window to configure settings less related to speedrunning / RE
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuscrollerh.h"
#include "childmenu.h"
#include "nandsettings.h"

namespace mod {

#define MISC_SETTING_COUNT 2

class MiscMenu final : public ChildMenu
{
private:
    MenuButton * mOptions[MISC_SETTING_COUNT];
    void optionToggle(MenuButton * button, bool &setting);

protected:
    virtual void disp() override;

public:
    MiscMenu();
};

}
