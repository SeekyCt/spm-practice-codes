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
    u16 pad_0x6;
    const char * animPoseName;
    const char * animName;
    const char * nameMsg;
    const char * descMsg;
    u16 buyPrice;
    u16 sellPrice;
    s16 hpGain;
    s16 xpGain;
    u8 cardBagChance;
    u8 cardShopChance;
    u16 cardShopBonusSeq;
    u16 cardShopMinGSW0;
    bool cardShopBlockDuplicate;
    u8 dispFlags;
    s16 tribe;
    s16 sortValue;
};
static_assert(sizeof(ItemData) == 0x2c);

extern "C" {

extern ItemData itemDataTable[ITEM_DATA_COUNT];

}

}