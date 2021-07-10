/*
    MapMenu
    Abstract menu window which displays the teleport effect toggle option
*/

#pragma once

#include "mod_ui_base/menuscrollerh.h"
#include "childmenu.h"

namespace mod {

class MapMenu : public ChildMenu
{
private:
    bool mShouldClose;

protected:
    virtual void close() override;
    virtual void disp() override;
};

}
