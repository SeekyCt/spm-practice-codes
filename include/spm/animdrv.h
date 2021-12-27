#pragma once

#include <wii/mtx.h>

namespace spm::animdrv {

struct AnimationModelFileAnimTableEntry
{
    char name[60];
    void * data;
};
static_assert(sizeof(AnimationModelFileAnimTableEntry) == 0x40);

struct AnimationModelFileHeader
{
    u8 unknown_0x0[0x148 - 0x0];
    u32 animCount;
    u8 unknown_0x14c[0x1ac - 0x14c];
    AnimationModelFileAnimTableEntry * anims;
};
static_assert(sizeof(AnimationModelFileHeader) == 0x1b0);

extern "C" {

s32 animPoseEntry(const char * filename, s32 releaseType);
void animPoseSetAnim(s32 id, const char * animName, bool forceReset);
void animPoseMain(s32 id);
void animPoseDrawMtx(s32 id, wii::Mtx34 * mtx, s32 xluStage, f32 rotY, f32 scale);
s32 animPoseRelease(s32 id);
void animPoseAutoRelease(s32 releaseType);
AnimationModelFileHeader * animPoseGetAnimBaseDataPtr(s32 id);

}

}
