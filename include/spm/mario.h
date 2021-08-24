#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::mario {

enum
{
    GRAV_DOWN,
    GRAV_UP,
    GRAV_LEFT,
    GRAV_RIGHT
};

enum
{
    PLAYER_MARIO,
    PLAYER_PEACH,
    PLAYER_BOWSER,
    PLAYER_LUIGI
};

struct MarioWork
{
    u32 flags;
    u8 unknown_0x4[0x38 - 0x4];
    u8 character;
    u8 unknown_0x39[0x5c - 0x39];
    wii::Vec3 position;
    u8 unknown_0x68[0x120 - 0x68];
    s32 camId;
    u8 unknown_0x124[0x14c - 0x124];
    f32 startspeed;
    f32 maxSpeed;
    u8 unknown_0x154[0x194 - 0x154];
    f32 hitboxWidth;
    f32 hitboxHeight;
    u8 unknown_0x19c[0x156c - 0x19c];
};
static_assert(sizeof(MarioWork) == 0x156c);

extern "C" {

MarioWork * marioGetPtr();
bool marioCtrlOffChk();
bool marioKeyOffChk();
int marioKeyOff();
int marioKeyOn();
void marioSetGravity(s32 type);

}

}
