#pragma once

#include "evt_cmd.h"

namespace spm::evt_fairy {

extern "C" {

// evt_fairy_flag_onoff(bool onoff, u32 flags)
EVT_DECLARE_USER_FUNC(evt_fairy_flag_onoff, 2)

}

}