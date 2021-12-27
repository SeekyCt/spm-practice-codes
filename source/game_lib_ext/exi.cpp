#include <types.h>
#include <wii/exi.h>

namespace wii::EXI {

// Decompiled from TTYD, non-matching
s32 EXIProbe(s32 chan)
{
    s32 ret;

    ret = wii::EXI::__EXIProbe(chan);
    if (ret != 0 && wii::EXI::Ecb[chan].unknown_0x20 == 0)
    {
        u32 id;
        ret = wii::EXI::EXIGetID(chan, 0, &id);
        if (ret == 0)
            return 0;
        else
            return 1;
    }
    else
    {
        return ret;
    }
}

}