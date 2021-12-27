#pragma once

#include <types.h>

namespace spm::item_data {

#define ITEM_DATA_COUNT 538

enum ItemType
{
    URA_DAN_KEY = 44,
    DAN_KEY = 48
    // more
};

struct ItemData 
{
    const char * itemName;
    s16 iconId;
    u8 unknown_0x6[0x8 - 0x6];
    const char * animPoseName;
    u8 unknown_0xc[0x10 - 0xc];
    const char * nameMsg;
    const char * descMsg;
    u8 unknown_0x18[0x2c - 0x18];
};
static_assert(sizeof(ItemData) == 0x2c);

extern "C" {

extern ItemData itemDataTable[ITEM_DATA_COUNT];

}

}