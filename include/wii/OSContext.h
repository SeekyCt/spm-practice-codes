#pragma once

#include <types.h>

namespace wii::OSContext {

struct OSContext
{
    u32 gpr[32];
    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;
    f64 fpr[32];
    u8 unknown_0x190[0x194 - 0x190];
    u32 fpscr;
    u32 srr0;
    u32 srr1;
    u16 mode;
    u16 state;
    u32 gqr[8];
    u8 unknown_0x1c4[0x1c8 - 0x1c4];
    f64 psf[32];
};
static_assert(sizeof(OSContext) == 0x2c8);

extern "C" {

void OSDumpContext(OSContext * context);

}

}
