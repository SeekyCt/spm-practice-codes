#pragma once

#include "evt_cmd.h"

namespace spm::evt_guide {

extern "C" {

// evt_guide_flag2_onoff(bool onoff, u32 flags)
EVT_DECLARE_USER_FUNC(evt_guide_flag2_onoff, 2)

// evt_guide_check_flag0(u32 flags, bool &ret)
EVT_DECLARE_USER_FUNC(evt_guide_check_flag0, 2)


}

}