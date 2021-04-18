#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::mapdata {

#define MAP_ID_MAX 0x1d4

struct MapData
{
    const char * name;
    const char * filename;
    const char * fallbackDoorName;
    wii::Vec3 fallbackSpawnPos;
    void * script; // set by game's rel prolog
};
static_assert(sizeof(MapData) == 0x1c);

extern "C" {

extern MapData * mapDataList[MAP_ID_MAX];

MapData * mapDataPtr(const char * mapName);

}

}
