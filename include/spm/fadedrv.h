#pragma once
#include <wii/types.h>

namespace spm::fadedrv {

struct FadeWork {
    u8 _00[0x28-0x00];
    wii::Vec3 fadeCenterPosition;
    u8 _34[0x238-0x34];
    void* ip; // probably ptr to screencap
    u8 _23c[0x24c-0x23c];
    s32 inTransition;
    s32 outTransition;
    u8 _254[0x260-0x254];
};
static_assert(sizeof(FadeWork) == 0x260);

extern "C" {

extern FadeWork* fadedrvWp2;

bool fadeIsFinish();
void setTransition(s32 in, s32 out);

}

}
