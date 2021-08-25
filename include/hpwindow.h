/*
    HPWindow
    Enemy health displays
*/
#pragma once

#include "mod_ui_base/window.h"

namespace mod {

class HPWindow : public Window
{
protected:
    virtual void disp() override;

public:
    static HPWindow * sInstance;

    HPWindow();
};

}
