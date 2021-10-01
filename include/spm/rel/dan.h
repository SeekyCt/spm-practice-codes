#pragma once

#include <types.h>
#include <spm/evt_door.h>
#include <spm/evtmgr.h>
#include <spm/evt_shop.h>
#include <spm/npcdrv.h>

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

extern const char * danMapParts[16];

extern spm::evt_door::DokanDesc danDokanDescs[8];
extern spm::evt_door::MapDoorDesc danMapDoorDescs[2];

extern int danFlipsideLockItems[];
extern int danFlopsideLockItems[];

EVT_DECLARE(dan_lock_interact_evt)
EVT_DECLARE(dan_lock_open_evt)

EVT_DECLARE(dan_enemy_room_init_evt)

extern spm::evt_door::DokanDesc danChestRoomDokanDesc;
extern spm::evt_door::MapDoorDesc danChestRoomMapDoorDescs[2];

EVT_DECLARE(dan_exit_pipe_sign_interact_evt)
EVT_DECLARE(dan_chest_open_evt)
extern int rotenShopItems[];
extern spm::evt_shop::EvtShopDef rotenShopDef;
extern spm::npcdrv::NPCTribeAnimDef rotenTribeAnimDefs[];
EVT_DECLARE(dan_chest_room_init_evt)

EVT_DECLARE(dan_30_map_door_desc)
EVT_DECLARE(dan_30_dokan_desc)
EVT_DECLARE(dan_70_dokan_desc)
EVT_DECLARE(dan_30_init_evt)
EVT_DECLARE(dan_70_init_evt)
extern spm::npcdrv::NPCTribeAnimDef dashellTribeAnimDefs[];
EVT_DECLARE(dan_30_chest_interact_evt)
EVT_DECLARE(dan_30_chest_open_evt)
extern spm::npcdrv::NPCTribeAnimDef wracktailTribeAnimDefs[];
EVT_DECLARE(dan_wracktail_main_evt)
EVT_DECLARE(dan_start_wracktail_evt)
EVT_DECLARE(dan_70_mario_chest_open_evt)
EVT_DECLARE(dan_70_dark_mario_chest_open_evt)
EVT_DECLARE(dan_70_peach_chest_open_evt)
EVT_DECLARE(dan_70_dark_peach_chest_open_evt)
EVT_DECLARE(dan_70_bowser_chest_open_evt)
EVT_DECLARE(dan_70_dark_bowser_chest_open_evt)
EVT_DECLARE(dan_70_luigi_chest_open_evt)
EVT_DECLARE(dan_70_dark_luigi_chest_open_evt)
EVT_DECLARE(dan_shadoo_main_evt)
EVT_DECLARE(dan_shadoo_fight_evt)
EVT_DECLARE(dan_shadoo_defeat_evt)
EVT_DECLARE(dan_70_reward_appear_evt)
EVT_DECLARE(dan_start_shadoo_evt)

// evt_dan_read_data()
EVT_DECLARE_USER_FUNC(evt_dan_read_data, 0)

// evt_dan_handle_map_parts(int no)
EVT_DECLARE_USER_FUNC(evt_dan_handle_map_parts, 1)

// evt_dan_handle_dokans(int no)
EVT_DECLARE_USER_FUNC(evt_dan_handle_dokans, 1)

// evt_dan_handle_doors(int no, int room, &char * enterDoor, &char * exitDoor, &float lockX, &float lockY, &float lockZ)
// EVT_DECLARE_USER_FUNC(evt_dan_handle_doors, 7)
EVT_DECLARE_USER_FUNC(evt_dan_handle_doors, 10)

// evt_dan_get_door_names(&char * enterName, &char * exitName)
EVT_DECLARE_USER_FUNC(evt_dan_get_door_names, 2)

// evt_dan_get_exit_door_name_l(&char * name)
EVT_DECLARE_USER_FUNC(evt_dan_get_exit_door_name_l, 1)

// evt_dan_get_enemy_info(int no, int enemyIdx, &int templateId, &int num)
EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_info, 4)

// evt_dan_make_spawn_table(int no)
EVT_DECLARE_USER_FUNC(evt_dan_make_spawn_table, 1)

// evt_dan_get_enemy_spawn_pos(int num, int no, int enemyIdx, &float x, &float y, &float z)
EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_spawn_pos, 6)

// evt_dan_decide_key_enemy(int itemId)
EVT_DECLARE_USER_FUNC(evt_dan_decide_key_enemy, 1)

void danCountdownDone();

// evt_dan_start_countdown()
EVT_DECLARE_USER_FUNC(evt_dan_start_countdown, 0);

bool danCheckKeyInMapBbox();
bool danCheckKeyEnemyInMapBbox();

// evt_dan_handle_key_failsafe()
EVT_DECLARE_USER_FUNC(evt_dan_handle_key_failsafe, 0)

// evt_dan_handle_chest_room_dokans_and_doors(int no)
EVT_DECLARE_USER_FUNC(evt_dan_handle_chest_room_dokans_and_doors, 1)

// evt_dan_get_chest_room_item(&int itemId)
EVT_DECLARE_USER_FUNC(evt_dan_get_chest_room_item, 1)

// evt_dan_boss_room_set_door_name()
EVT_DECLARE_USER_FUNC(evt_dan_boss_room_set_door_name, 0)

void func_80c839cc(const char * param_1, bool param_2);
void wracktailDispCb(void * param, int animGroupIdx, int param_3);
EVT_DECLARE_USER_FUNC(evt_dan_set_wracktail_disp_cb, 0)
void screenBlinkDisp(s8 cameraId, void * param);
EVT_DECLARE_USER_FUNC(evt_dan_screen_blink, 0)
const char * func_80c83f6c(const char * param_1);

}

}
