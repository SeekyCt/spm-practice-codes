#pragma once

#include <types.h>
#include <spm/camdrv.h>

namespace spm::dispdrv {

typedef void (DispCallback)(s8 cameraId, void * param);

extern "C" {

void dispEntry(s8 cameraId, u8 renderMode, f32 z, DispCallback * cb, void * cbParam);

}

}
