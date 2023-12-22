#include <common.h>
#include <wii/os/OSInterrupt.h>
#include <wii/si.h>

// Decompiled from TTYD, non-matching

namespace wii::SI {

bool SIIsChanBusy(s32 chan)
{
    if (wii::SI::Packet[chan].unknown_0x0 == -1 && wii::SI::Si.channel != chan)
        return false;
    else
        return true;
}

u32 SIGetStatus(s32 chan)
{
    u32 intr = wii::os::OSDisableInterrupts();

    // Read SI reg
    u32 status = *(u32 *)0xcc006438;

    // Extract byte for channel
    status = (status >> (8 * (3 - chan))) & 0xff;

    // Update type
    if ((status & 8) && (wii::SI::Type[chan] & 0x80) == 0)
        wii::SI::Type[chan] = 8;

    wii::os::OSRestoreInterrupts(intr);

    return status;
}

}