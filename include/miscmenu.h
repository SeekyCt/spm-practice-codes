/*
    MiscMenu
    Menu window to configure settings less related to speedrunning / RE
*/

#pragma once

#include "mod_ui_base/menuwindow.h"
#include "mod_ui_base/menuscrollerh.h"
#include "nandsettings.h"

namespace mod {

class MiscMenu : public MenuWindow
{
private:
    void optionToggle(MenuButton * button, bool &setting);

protected:
    virtual void close() override;

public:
    MiscMenu();
};

}
