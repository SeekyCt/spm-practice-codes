#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::item_event_data {

#define ITEM_EVENT_DATA_COUNT 33

struct ItemEventData
{
    int itemId;
    const spm::evtmgr::EvtScriptCode * useEvtScript;
    const char * useMsg;
};
static_assert(sizeof(ItemEventData) == 0xc);

extern "C" {

extern ItemEventData itemEventDataTable[ITEM_EVENT_DATA_COUNT];

}

}
