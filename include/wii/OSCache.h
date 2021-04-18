#pragma once

#include <types.h>

namespace wii::OSCache {

extern "C" {

void DCFlushRange(void * ptr, u32 size);
void ICInvalidateRange(void * ptr, u32 size);

}

}
