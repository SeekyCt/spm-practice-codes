#pragma once

namespace spm::npcdrv {

struct NPCEntry
{
    s32 id;
    s32 unknown_0x4;
    u32 flags_8;
    u32 flags_c;
    u32 flags_10;
    u8 unknown_0x14[0x2a0 - 0x14];
    wii::Vec3 position;
    u8 unknown_0x2ac[0x4ec - 0x2ac];
    u32 maxHp;
    u32 hp;
    u8 unknown_0x4f4[0x748 - 0x4f4];
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
