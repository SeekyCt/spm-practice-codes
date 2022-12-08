#pragma once
#include <wii/types.h>

namespace spm::fadedrv {

struct FadeEntry {
    u8 _00[0x68];
    char* animPoseName;
    char* _6c;
    int animPoseAnimName;
    int unkAnimPoseId;
    int unkAnimPaperPoseId;
    u8 _7c[0x80-0x7c];
    void* ip;
    u8 _84[0x90-0x84];
};
static_assert(sizeof(FadeEntry) == 0x90);

struct FadeWork {
    u8 _00[0x08-0x00];
    FadeEntry entries[4];
    u8 _23c[0x24c-0x248];
    s32 mapChangeInTransition;
    s32 mapChangeOutTransition;
    u8 _254[0x260-0x254];
};
static_assert(sizeof(FadeWork) == 0x260);

extern "C" {

extern FadeWork* fadedrvWp;

bool fadeIsFinish();
void fadeSetMapChangeTransition(s32 in, s32 out); // Only called on map change and in minigames

}

}
