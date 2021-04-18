#pragma once

#include <types.h>

namespace spm::dispdrv {

enum
{
    // 0-12 unknown
    CAM_DEBUG = 13,
    CAM_DEBUG_3D = 14
};

typedef void (DispCallback)(s8 cameraId, void * param);

extern "C" {

void dispEntry(s8 cameraId, u8 renderMode, f32 z, DispCallback * cb, void * cbParam);

}

}
