#pragma once

#include "evt_cmd.h"

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_shop {

struct EvtShopDef
{
    int type;
    const char * npcName;
    int * items; // list of itemId, price, unknown int, terminated by -1 itemId
    const char * case1Name;
    const char * case2Name;
};

extern "C" {

// evt_shop_set_defs(EvtShopDef * defs, int count)
EVT_DECLARE_USER_FUNC(evt_shop_set_defs, 2)

}

}
