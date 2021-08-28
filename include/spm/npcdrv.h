#pragma once

#include <wii/types.h>

namespace spm::npcdrv {

struct NPCAnim
{
    s32 m_nPoseId;
    char animPoseName[32];
    u8 unknown_0x24[0x44 - 0x24];
    u32 flags;
    u8 unknown_0x48[0x244 - 0x48];
};
static_assert(sizeof(NPCAnim) == 0x244);

struct NPCPart
{
    u8 unknown_0x0[0x8 - 0x0];
    wii::Vec3 pos;
    u8 unknown_0x14[0x38c - 0x14];
    NPCPart * next;
    NPCPart * prev;
    u8 unknown_0x394[0x398 - 0x394];
};
static_assert(sizeof(NPCPart) == 0x398);

struct NPCEntry
{
    s32 id;
    s32 unknown_0x4;
    u32 flags_8;
    u32 flags_c;
    u32 flags_10;
    u8 unknown_0x14[0x24 - 0x14];
    char name[32];
    NPCAnim m_Anim;
    u8 unknown_0x288[0x2a0 - 0x288];
    wii::Vec3 position;
    u8 unknown_0x2ac[0x46c - 0x2ac];
    u32 flag46C;
    u8 unknown_0x470[0x49c - 0x470];
    s32 tribeId;
    u8 unknown_0x4a0[0x4ec - 0x4a0];
    u32 maxHp;
    u32 hp;
    u8 unknown_0x4f4[0x714 - 0x4f4];
    NPCPart * parts;
    u8 unknown_0x718[0x748 - 0x718];
};
static_assert(sizeof(NPCEntry) == 0x748);

struct NPCWork
{
    u32 flag;
    s32 num;
    NPCEntry * entries;
    u8 unknown_0xc[0xb48 - 0xc];
};
static_assert(sizeof(NPCWork) == 0xb48);

extern "C" {

NPCWork * npcGetWorkPtr();

}

}
