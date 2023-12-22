#include "gcpadpatches.h"
#include "mod.h"
#include "patch.h"
#include "mod_ui_base/colours.h"
#include "mod_ui_base/window.h"

#include <msl/stdio.h>
#include <msl/string.h>
#include <spm/homebuttondrv.h>
#include <spm/mario_sbr.h>
#include <spm/wpadmgr.h>
#include <wii/kpad.h>
#include <wii/pad.h>
#include <wii/wpad.h>

namespace mod {

void (*wpadMainReal)();
s32 (*KPADReadReal)(s32 chan, wii::kpad::KPADStatus * statuses, u32 length);

wii::PAD::PADStatus pads[4];
wii::PAD::PADStatus prevPads[4];

static void readGcn()
{
    msl::string::memcpy(prevPads, pads, sizeof(prevPads));

    wii::PAD::PADRead(pads);

    u32 resetMask = 0;
    for (s32 i = 0; i < 4; i++)
    {
        u32 chanMask = 0x80000000 >> i;
        if (pads[i].error == -1)
            resetMask |= chanMask;
    }
    if (resetMask != 0)
        wii::PAD::PADReset(resetMask);

    wpadMainReal();
}

static u32 padToWpad(wii::PAD::PADStatus * pad)
{
    u32 ret = 0;

    // Dpad values are equal
    ret |= pad->buttonsPressed & (PAD_BTN_LEFT | PAD_BTN_UP | PAD_BTN_DOWN | PAD_BTN_UP);

    if (pad->buttonsPressed & PAD_BTN_START)
        ret |= WPAD_BTN_PLUS;
    
    // Map ABXY to 21BA
    ret |= pad->buttonsPressed & (PAD_BTN_A | PAD_BTN_B | PAD_BTN_X | PAD_BTN_Y);

    // Map minus to Z
    if (pad->buttonsPressed & PAD_BTN_Z)
        ret |= WPAD_BTN_MINUS;

    // TODO: sticks & triggers

    return ret;
}

static s32 injectGcn(s32 chan, wii::kpad::KPADStatus * statuses, u32 length)
{
    s32 ret = KPADReadReal(chan, statuses, length);

    u32 cur = padToWpad(pads + chan);
    u32 prev = padToWpad(prevPads + chan);
    statuses[0].buttonsHeld |= cur & prev;
    statuses[0].buttonsPressed |= cur & ~prev;
    statuses[0].buttonsReleased |= ~cur & prev;

    return ret;
}

static s32 dcOverride(s32 chan, void * p2)
{
    s32 ret = wii::wpad::WPADProbe(chan, p2);
    if (ret == -1)
    {
        if (pads[0].error == 0 || pads[0].error == -3)
            ret = 0;
    }

    return ret;
}

void gcpadPatch()
{
    wpadMainReal = patch::hookFunction(spm::wpadmgr::wpadMain, readGcn);
    KPADReadReal = patch::hookFunction(wii::kpad::KPADRead, injectGcn);

    writeBranchLink(spm::homebuttondrv::homebuttonMain, 0x5f0, dcOverride);
    writeBranchLink(spm::homebuttondrv::homebuttonMain, 0x63c, dcOverride);

    writeBranch(spm::mario_sbr::marioUpdateKeyData, 0, mod::gcpadpatches::marioUpdateKeyDataPatch);
}

}