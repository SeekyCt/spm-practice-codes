/*
    HitboxMenu
    Menu window to configure hitbox displays
*/

#pragma once

#include <common.h>
#include <spm/dispdrv.h>

#include "childmenu.h"
#include "nandsettings.h"

namespace mod {

class HitboxMenu final : public ChildMenu
{
private:
    static void activateCheck(s32 cameraId, u8 renderMode, f32 z, spm::dispdrv::DispCallback * cb, void * cbParam);
    void optionToggle(MenuButton * button, bool &setting);

public:
    HitboxMenu();

    static void hitboxPatch();
};

}
