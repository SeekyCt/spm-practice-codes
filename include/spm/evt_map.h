#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_map {

extern "C" {

// evt_mapobj_flag_onoff(bool group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mapobj_flag_onoff, 4)

// evt_mapobj_flag4_onoff(bool group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mapobj_flag4_onoff, 4)

// evt_mapobj_trans(const char * name, int x, int y, int z)
EVT_DECLARE_USER_FUNC(evt_mapobj_trans, 4)

// evt_mapobj_color(bool group, const char * name, u8 r, u8 g, u8 b, u8 a)
EVT_DECLARE_USER_FUNC(evt_mapobj_color, 6)

}

}
