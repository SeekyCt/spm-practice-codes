#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_sub {

extern "C" {

EVT_DECLARE_USER_FUNC(evt_sub_intpl_msec_init, 4)
EVT_DECLARE_USER_FUNC(evt_sub_intpl_msec_get_value, 0)

// Gives a random number from 0 to max (inclusive)
// evt_sub_random(int max, &int ret)
EVT_DECLARE_USER_FUNC(evt_sub_random, 2)

EVT_DECLARE_USER_FUNC(func_800d4de4, 2)
EVT_DECLARE_USER_FUNC(func_800d74a0, 2)

}

}
