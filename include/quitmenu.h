/*
    QuitMenu
    Menu window to return to the title screen
*/

#pragma once

#include "mod_ui_base/menuwindow.h"

namespace mod {

class QuitMenu : public MenuWindow
{
protected:
    virtual void close() override;

public:
    QuitMenu();
};

}
