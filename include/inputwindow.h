/*
    InputWindow
    Window that displays the current player inputs
*/
#pragma once

#include <common.h>

#include "mod_ui_base/window.h"
#include "pyconsole.h"

namespace mod {

class InputWindow final : public Window
{
private:
    // Paramaters for button icons
    struct ButtonDef
    {
        u32 mask;
        u32 tplIdx;
    };
    static ButtonDef sDefs[];

    // Parameters for dpad direction overlays
    struct DpadDef
    {
        u32 mask;
        u32 tplIdx;
    };
    static DpadDef sDpadDefs[];

protected:
    virtual void disp() override;

    InputWindow();
    
public:
    // Window::mPosX/Y are used for top left corner of window

    static InputWindow * sInstance;

    static void init();
};

}
