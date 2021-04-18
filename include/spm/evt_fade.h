#pragma once

#include "evt_cmd.h"

namespace spm::evt_fade {

extern "C" {

// evt_fade_end_wait(int)
EVT_DECLARE_USER_FUNC(evt_fade_end_wait, 1)

}

}
