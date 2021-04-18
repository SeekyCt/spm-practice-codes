#pragma once

#include "evt_cmd.h"

namespace spm::evt_mario {

extern "C" {

// evt_mario_flag8_onoff(bool onoff, u32 flags)
EVT_DECLARE_USER_FUNC(evt_mario_flag8_onoff, 2)

// evt_mario_key_on()
EVT_DECLARE_USER_FUNC(evt_mario_key_on, 0)

// evt_mario_key_off(int)
EVT_DECLARE_USER_FUNC(evt_mario_key_off, 1)

}

}