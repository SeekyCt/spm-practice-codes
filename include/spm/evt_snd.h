#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_snd {

extern "C" {

// evt_snd_bgmon(int param_1, const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_bgmon, 2)

EVT_DECLARE_USER_FUNC(func_800d36f4, 1)

}

}
