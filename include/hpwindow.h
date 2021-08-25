/*
    HPWindow
    Enemy health displays
*/
#pragma once

#include "mod_ui_base/window.h"

#include <spm/npcdrv.h>

namespace mod {

class HPWindow final : public Window
{
private:
    /*
        If a door has been entered in this map, checks that the NPC is shown by it
    */
    bool npcDoorCheck(const char * name);

    /*
        Checks that an NPC isn't behind the camera in 3d
    */
    bool npcPosCheck(spm::npcdrv::NPCEntry * npc);

protected:
    virtual void disp() override;
    HPWindow();

public:
    static HPWindow * sInstance;

    static void init();
};

}
