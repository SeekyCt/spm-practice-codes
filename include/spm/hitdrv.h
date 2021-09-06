#pragma once

#include <types.h>

namespace spm::hitdrv {

struct HitObj
{
    u8 unknown_0x0[0x114 - 0x0];
};
static_assert(sizeof(HitObj) == 0x114);

#define HITOBJ_FLAG_DISABLE 1

extern "C" {

HitObj * hitNameToPtr(const char * name);

void hitGrpFlagOn(bool allowSubname, const char * name, u32 mask);
void hitGrpFlagOff(bool allowSubname, const char * name, u32 mask);
bool hitCheckFilter(float, float, float, float, float, float, void *, float *, float *,
                    float *, float *, float *, float *, float *);
void hitObjGetPos(const char * name, wii::Vec3 * ret);
void hitGetMapEntryBbox(int mapEntryIdx, wii::Vec3 * min, wii::Vec3 * max);


}

}