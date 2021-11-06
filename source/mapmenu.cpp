#include "mapmenu.h"

#include <spm/wpadmgr.h>
#include <wii/wpad.h>

namespace mod {

const char * MapMenu::getWarpEffectMsg()
{
    return gSettings->mapChangeEffect ?
        "Display teleport effect (<icon PAD_MINUS 0.8 0 35 0><col ffffffff>): on" : 
        "Display teleport effect (<icon PAD_MINUS 0.8 0 35 0><col ffffffff>): off";
}

bool MapMenu::toggleWarpEffect(MenuButton * button, void * param)
{
    (void) button;
    
    MapMenu * instance = reinterpret_cast<MapMenu *>(param);

    // Toggle setting
    gSettings->mapChangeEffect = !gSettings->mapChangeEffect;
    
    // Update option text
    instance->mWarpButton->mMsg = getWarpEffectMsg();

    // Signal menu hasn't been closed
    return true;
}

MapMenu::MapMenu()
{
    mWarpButton = new PassiveButton(this, getWarpEffectMsg(), -320.0f, -100.0f,
                                    WPAD_BTN_MINUS, toggleWarpEffect, this, 0.8f,
                                    nullptr, true);
}

}
