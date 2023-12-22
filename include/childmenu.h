/*
    ChildMenu
    Abstract child menu window which returns to MainMenu on closing
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuwindow.h"
#include "mod_ui_base/menuscrollerh.h"
#include "nandsettings.h"

namespace mod {

class ChildMenu : public MenuWindow
{
protected:
    virtual void close() override;
};

}
