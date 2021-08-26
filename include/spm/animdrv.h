#pragma once

#include <wii/mtx.h>

namespace spm::animdrv {

extern "C" {

s32 animPoseEntry(const char * filename, s32 releaseType);
void animPoseMain(s32 id);
void animPoseDrawMtx(s32 id, wii::Mtx34 * mtx, s32 xluStage, f32 rotY, f32 scale);
s32 animPoseRelease(s32 id);
void animPoseAutoRelease(s32 releaseType);

}

}
