#include "mapmenu.h"

#include <spm/wpadmgr.h>
#include <wii/wpad.h>

namespace mod {

void MapMenu::disp()
{
    // Run main display function
    MenuWindow::disp();

    // Menu may have been closed during that function
    if (mShouldClose)
    {
        // Change back to parent menu
        ChildMenu::close();
    }
    else
    {    
        // Toggle teleport if minus is pressed
        if (spm::wpadmgr::wpadGetButtonsPressed(0) & WPAD_BTN_MINUS)
            gSettings->mapChangeEffect = !gSettings->mapChangeEffect;
        
        // Display option text
        const char * msg = gSettings->mapChangeEffect ?
            "Display teleport effect (<icon PAD_MINUS 0.8 0 35 0><col ffffffff>): on" : 
            "Display teleport effect (<icon PAD_MINUS 0.8 0 35 0><col ffffffff>): off";
        drawMessage(msg, -320.0f, -100.0f, nullptr, 0.8f);
    }
}

void MapMenu::close()
{
    // Schedule changing back to parent menu
    // (can't just delete now since this will return to the rest of the custom disp function)
    mShouldClose = true;
}

}
