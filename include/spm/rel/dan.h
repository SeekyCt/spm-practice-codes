#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::dan {

#define DUNGEON_MAX 200
#define DAN_PARTS_COUNT 16

struct DanDoor
{
    s32 enter;
    s32 exit;
};
static_assert(sizeof(DanDoor) == 0x8);

struct DanEnemy
{
    s32 name;
    s32 num;
    s32 pos;
};
static_assert(sizeof(DanEnemy) == 0xc);

struct DanDungeon
{
    u32 map;
    s32 item;
    s32 doorCount;
    s32 enemyCount;
    DanDoor doors[20];
    DanEnemy enemies[16];
};
static_assert(sizeof(DanDungeon) == 0x170);

struct DanWork
{
    DanDungeon * dungeons;
    DanDoor doorInfo;
    int spawnTableCount;
    int spawnTable[32];
    u8 unknown_0x90[0x110 - 0x90];
    char enterDoorName_l[64];
    char enterDoorName_r[64];
    char enterDoorHitName2d[64];
    char enterDoorHitName3d[64];
    char prevMapName[64];
    char enterDoor_desc0x18[64];
    char exitDoorName_l[64];
    char exitDoorName_r[64];
    char exitDoorHitName2d[64];
    char exitDoorHitName3d[64];
    char nextMapName[64];
    char exitDoor_desc0x18[64];
    u8 unknown_0x410[0x510 - 0x410];
    char enterDoorName[64];
    char exitDoorName[64];
};
static_assert(sizeof(DanWork) == 0x590);

extern "C" {

extern DanWork * danWp;

// evt_dan_read_data()
EVT_DECLARE_USER_FUNC(evt_dan_read_data, 0)

}

}
