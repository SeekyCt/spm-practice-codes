#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::camdrv {

#define CAM_FLAG_16_9 0x10000000

enum
{
    // 0-4 unknown
    CAM_3D = 5,
    // 6-10 unknown
    CAM_2D = 11,
    // 12 unknown
    CAM_DEBUG = 13,
    CAM_DEBUG_3D = 14,
    CAM_ID_MAX = 15
};

struct CamEntry
{
    u32 flag;
    u8 unknown_0x4[0x54 - 0x4];
    wii::Mtx34 viewMtx;
    u8 unknown_0x84[0x94 - 0x84];
    wii::Mtx44 projMtx;
    u8 unknown_0xd4[0xf0 - 0xd4];
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
    u8 unknown_0x100[0x158 - 0x100];
    BOOL isOrtho;
    wii::Vec3 pos;
    wii::Vec3 target;
    u8 unknown_0x174[0x2f0 - 0x174]; 
};
static_assert(sizeof(CamEntry) == 0x2f0);

extern "C" {

CamEntry * camGetPtr(int camId);
CamEntry * camGetCurPtr();
void getScreenPoint(wii::Vec3 * worldPos, wii::Vec3 * screenPosOut);

}

}
