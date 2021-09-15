/*
    InputWindow
    Window that displays the current player inputs
*/
#pragma once

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
        u32 icon;
        f32 pressScale;
        f32 holdScale;
    };
    static ButtonDef sDefs[];

    // Parameters for dpad direction overlays
    struct DpadDef
    {
        u32 mask;
        f32 xOffset;
        f32 yOffset;
    };
    static DpadDef sDpadDefs[];

    // Log of previous frames' pressed buttons
    static u32 sPressed[3];
    static u32 sPressedIdx;

    // Checks whether a button has been pressed in the past 3 frames
    bool showAsPressed(u32 mask);

    // Updates button press log
    void updatePressed();

protected:
    virtual void disp() override;

    InputWindow();
    
public:
    // Window::mPosX/Y are used for top left corner of window

    static InputWindow * sInstance;

    static void init();
};

}
