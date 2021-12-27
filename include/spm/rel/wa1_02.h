#pragma once

#include <types.h>

namespace spm::wa1_02 {

struct SammerDef
{
    const char * mapName;
    u8 unknown_0x4[0x8 - 0x4];
    s32 alreadyBeatenLsw1;
    u8 unknown_0xc[0x10 - 0xc];
    s32 alreadyBeatenGsw0;
    u8 unknown_0x14[0x28 - 0x14];
};
static_assert(sizeof(SammerDef) == 0x28);

extern "C" {

extern SammerDef sammerDefsCh6[30];
extern SammerDef sammerDefsEndgame[30];

}

}