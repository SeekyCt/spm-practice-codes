#pragma once

#include <spm/evtmgr.h>

namespace spm::machi {

struct ElvDesc
{
    u8 unknown_0x0[0x4 - 0x0];
    const char * name;
    u8 unknown_0x8[0x2c - 0x8];
};
static_assert(sizeof(ElvDesc) == 0x2c);

extern "C" {

// evt_machi_set_elv_descs(ElvDesc * descs, int count)
EVT_DECLARE_USER_FUNC(evt_machi_set_elv_descs, 2)

}

}
