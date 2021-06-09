#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::camdrv {

#define CAM_FLAG_16_9 0x10000000

enum
{
    // 0-12 unknown
    CAM_DEBUG = 13,
    CAM_DEBUG_3D = 14
};

struct CamEntry
{
    u32 flag;
    u8 unknown_0x4[0x15c - 0x4];
    wii::Vec3 pos;
    wii::Vec3 target;
    u8 unknown_0x174[0x2f0 - 0x174]; 
};
static_assert(sizeof(CamEntry) == 0x2f0);

extern "C" {

CamEntry * camGetPtr(int camId);

}

}
