#pragma once

#include "evt_cmd.h"

#include <types.h>

namespace spm::evt_door {

struct DoorDesc
{
    u8 unknown_0x0[0xc - 0x0];
    const char * name;
    u8 unknown_0x10[0x44 - 0x10];
    const char ** npcNameList;
    u8 unknown_0x48[0x58 - 0x48];
};
static_assert(sizeof(DoorDesc) == 0x58);

struct DokanDesc
{
    int scriptNumber;
    u16 flags;
    u16 _pad_6;
    const char * name;
    const char * mapName;
    const char * hitName2d;
    const char * hitName3d;
    const char * destMapName;
    const char * unknown_0x1c;
};
static_assert(sizeof(DokanDesc) == 0x20);

struct MapDoorDesc
{
    u32 flags;
    const char * name;
    const char * name_r;
    const char * hitName2d;
    const char * hitName3d;
    const char * destMapName;
    const char * unknown_0x18;
    int unknown_0x1c;
};
static_assert(sizeof(MapDoorDesc) == 0x20);

#define EVT_DOOR_FLAG_DOOR_OPEN 1

struct EvtDoorWork
{
    u16 flags;
    u8 unknown_0x2[0x57c - 0x2];
};
static_assert(sizeof(EvtDoorWork) == 0x57c);

extern "C" {

EVT_DECLARE(door_init_evt)

extern EvtDoorWork * evtDoorWp;

DoorDesc * evtDoorGetActiveDoorDesc();

EVT_DECLARE_USER_FUNC(evt_door_wait_flag, 1)

// evt_door_set_door_descs(DoorDesc * descs, int count)
EVT_DECLARE_USER_FUNC(evt_door_set_door_descs, 2)

// evt_door_set_dokan_descs(DokanDesc * descs, int count)
EVT_DECLARE_USER_FUNC(evt_door_set_dokan_descs, 2)

// evt_door_set_map_door_descs(MapDoorDesc * descs, int count)
EVT_DECLARE_USER_FUNC(evt_door_set_map_door_descs, 2)

// evt_door_set_map_door_descs(bool enable, char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_map_door_desc, 2)

}

}
