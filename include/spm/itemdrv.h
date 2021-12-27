#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::itemdrv {

struct ItemEntry
{
    u32 flags;
    u8 unknown_0x4[0x10 - 0x4];
    s16 type;
    s16 behaviour;
    wii::Vec3 position;
    u8 unknown_0x20[0x88 - 0x20];
};
static_assert(sizeof(ItemEntry) == 0x88);

struct ItemWork
{
    int num;
    ItemEntry * entries;
    u8 unknown_0x8[0x80 - 0x8];
};
static_assert(sizeof(ItemWork) == 0x80);

extern "C" {

ItemWork * itemGetWork();
void * itemEntry(const char * name, s32 type, f32 x, f32 y, f32 z, s32 behaviour,
                 spm::evtmgr::EvtScriptCode * pickupScript, int switchNumber);
int itemTypeNameToId(const char * itemName);
bool itemCheckForId(int itemId);

}

}
