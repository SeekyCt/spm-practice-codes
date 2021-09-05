#pragma once

#include <types.h>

namespace wii::EXI {

struct EXIEcb
{
    u8 unknown_0x0[0x20 - 0x0];
    s32 unknown_0x20;
    u8 unknown_0x24[0x40 - 0x24];
};
static_assert(sizeof(EXIEcb) == 0x40);

extern "C" {

extern EXIEcb Ecb[3];

s32 EXIImm(s32 chan, void * data, s32 len, s32 mode, void * completionCb);
s32 EXISync(s32 chan);
s32 __EXIProbe(s32 chan);
s32 EXISelect(s32 chan, s32 dev, s32 freq);
s32 EXIDeselect(s32 chan);
s32 EXILock(s32 chan, s32 dev, void * unlockCb);
s32 EXIUnlock(s32 chan);
s32 EXIGetID(s32 chan, s32 dev, u32 * outId);

}

// Re-implemented in game_lib_ext:
s32 EXIProbe(s32 chan);

}
