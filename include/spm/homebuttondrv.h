#pragma once

#include <types.h>

namespace spm::homebuttondrv {

#define HOMEBUTTON_FLAG_OPEN 0x10
#define HOMEBUTTON_FLAG_CONTROLLER_DC 0x20
#define HOMEBUTTON_FLAG_FORBIDDEN 0x40

struct HomeButtonWork
{
    s32 state;
    u16 flags;
    u8 unknown_0x6[0xa0 - 0x6];
};
static_assert(sizeof(HomeButtonWork) == 0xa0);

extern "C" {

extern HomeButtonWork * homebuttonWp;

bool homebuttonMain();

}

}
