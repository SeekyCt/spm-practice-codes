#include <types.h>
#include <wii/OSInterrupt.h>
#include <wii/OSRtc.h>
#include <wii/pad.h>
#include <wii/vi.h>
#include <wii/si.h>
#include <wii/string.h>

namespace wii::PAD {

// Decompiled from TTYD / MKW, non-matching

/*
PADRead
    x SIIsChanBusy
    x SIGetStatus
    x PADOriginUpdateCallback
    x PADReceiveCheckCallback
*/

static PADStatus prevStatus[4];

// Always inlined
static void disableChannel(s32 chan)
{
    u32 intr = wii::OSInterrupt::OSDisableInterrupts();

    u32 chanMask = 0x80000000 >> chan;

    wii::SI::SIDisablePolling(chanMask);

    wii::PAD::EnabledBits &= ~chanMask;
    wii::PAD::WaitingBits &= ~chanMask;
    wii::PAD::CheckingBits &= ~chanMask;
    wii::PAD::PendingBits &= ~chanMask;
    wii::PAD::BarrelBits &= ~chanMask;

    wii::OSRtc::OSSetWirelessID(chan, 0);

    wii::OSInterrupt::OSRestoreInterrupts(intr);
}

void PADOriginUpdateCallback(s32 chan, u32 flags)
{
    u32 chanMask = 0x80000000 >> chan;
    if ((wii::PAD::EnabledBits & chanMask) == 0)
        return;
    
    if ((flags & 0xf) == 0)
        wii::PAD::UpdateOrigin(chan);
    
    if (flags & 8)
        disableChannel(chan);
}

void PADReceiveCheckCallback(s32 chan, u32 flags)
{
    u32 chanMask = 0x80000000 >> chan;
    if ((wii::PAD::EnabledBits & chanMask) == 0)
        return;

    if ((flags & 0xf) == 0 &&
        (flags & 0x80000000) != 0 &&
        (flags & 0x100000) != 0 &&
        (flags & 0x40000000) != 0 && 
        (flags & 0x4000000) == 0 &&
        (flags & 0x80000) == 0 &&
        (flags & 0x40000) == 0
    )
        wii::SI::SITransfer(chan, &wii::PAD::CmdReadOrigin, 1, wii::PAD::Origin + chan, 10,
                            PADOriginUpdateCallback, 0, 0);
    else
        disableChannel(chan);
}

u32 PADRead(wii::PAD::PADStatus * pads)
{
    u32 intr = wii::OSInterrupt::OSDisableInterrupts();

    wii::SI::SIResponse siResponse;
    u32 readMask = 0;

    for (s32 i = 0; i < 4; i++)
    {
        u32 chanMask = 0x80000000 >> i;
        if (wii::PAD::PendingBits & chanMask)
        {
            wii::PAD::PADReset(0);
            pads[i].error = 0xfe;
            wii::string::memset(pads + i, 0, 10);
        }
        else if ((wii::PAD::ResettingBits & chanMask) || wii::PAD::ResettingChan == i)
        {
            pads[i].error = 0xfe;
            wii::string::memset(pads + i, 0, 10);
        }
        else if ((wii::PAD::EnabledBits & chanMask) == 0)
        {
            pads[i].error = 0xff;
            wii::string::memset(pads + i, 0, 10);
        }
        else if (wii::SI::SIIsChanBusy(i))
        {
            pads->error = 0xfd;
            wii::string::memset(pads,0,10);
        }
        else if ((wii::SI::SIGetStatus(i) & 8) == 0)
        {
            if ((wii::SI::SIGetType(i) & 0x20000000) == 0)
                readMask |= chanMask;

            if (wii::SI::SIGetResponse(i, &siResponse) == 0)
            {
                pads[i].error = 0xfd;
                wii::string::memset(pads,0,10);
            }
            else if ((siResponse.unknown_0x0 & 0x80000000) != 0)
            {
                pads[i].error = 0xfd;
                wii::string::memset(pads, 0, 10);
            }
            else
            {
                wii::PAD::MakeStatus(i, pads + i, &siResponse);

                s32 thres;
                if (((wii::SI::Type[i] & 0xffff0000) == 0x09000000) &&
                    ((pads[i].buttonsPressed & 0x80) == 0))
                    thres = 10;
                else
                    thres = 3;

                #define abs __builtin_abs

                if ((abs(abs(pads[i].lStickX) - abs(prevStatus[i].lStickX))) >= thres ||
                    (abs(abs(pads[i].lStickY) - abs(prevStatus[i].lStickY))) >= thres ||
                    (abs(abs(pads[i].rStickX) - abs(prevStatus[i].rStickX))) >= thres ||
                    (abs(abs(pads[i].rStickY) - abs(prevStatus[i].rStickY))) >= thres ||
                    (abs(abs(pads[i].lTrigger) - abs(prevStatus[i].lTrigger))) >= thres ||
                    (abs(abs(pads[i].rTrigger) - abs(prevStatus[i].rTrigger))) >= thres ||
                    pads[i].buttonsPressed != prevStatus[i].buttonsPressed
                )
                    wii::VI::__VIResetSIIdle();

                #undef abs

                wii::string::memcpy(&prevStatus[i], pads + i, 0xc);

                if ((pads[i].buttonsPressed & 0x2000) == 0)
                {
                    pads[i].error = 0;
                    pads[i].buttonsPressed &= ~0x80;
                }
                else
                {
                    pads[i].error = 0xfd;
                    wii::string::memset(pads,0,10);
                    wii::SI::SITransfer(i,&wii::PAD::CmdReadOrigin, 1, wii::PAD::Origin + i,
                                        10, PADOriginUpdateCallback, 0, 0);
                }
            }
        }
        else
        {
            wii::SI::SIGetResponse(i, &siResponse);
            if ((wii::PAD::WaitingBits & chanMask) == 0)
            {
                disableChannel(i);
                pads[i].error = 0xff;
                wii::string::memset(pads, 0, 10);
            }
            else
            {
                pads[i].error = 0;
                wii::string::memset(pads, 0, 10);
                if ((wii::PAD::CheckingBits & chanMask) == 0)
                {
                    wii::PAD::CheckingBits |= chanMask;
                    wii::SI::SIGetTypeAsync(i, PADReceiveCheckCallback);
                }
            }
        }
    }

    wii::OSInterrupt::OSRestoreInterrupts(intr);

    return readMask;
}

}