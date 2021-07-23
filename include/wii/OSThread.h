#pragma once

#include <types.h>
#include <wii/OSContext.h>

namespace wii::OSThread {

struct OSThread
{
    wii::OSContext::OSContext context;
    u8 unknown_0x2c8[0x2fc - 0x2c8];
    OSThread * next;
    OSThread * prev;
    u8 unknown_0x304[0x318 - 0x304];
};
static_assert(sizeof(OSThread) == 0x318);

extern "C" {

extern OSThread * currentThread;
int OSSuspendThread(OSThread * thread);

}

}
