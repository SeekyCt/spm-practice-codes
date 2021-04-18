#pragma once

#include <types.h>

namespace wii::OSInterrupt {

extern "C" {

u32 OSDisableInterrupts();
void OSRestoreInterrupts(u32 interruptMask);

}

}
