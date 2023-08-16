#pragma once

#include <wii/types.h>

namespace wii::VI {

extern "C" {

void VISetBlack(bool enabled);
void VIFlush();
void VIWaitForRetrace();
void __VIResetSIIdle();

}

}
