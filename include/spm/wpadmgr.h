#pragma once

#include <types.h>

namespace spm::wpadmgr {

extern "C" {

u32 wpadGetButtonsPressed(s32 controller);
u32 wpadGetButtonsHeld(s32 controller);
u32 wpadGetButtonsHeldRepeat(s32 controller);

}

}
