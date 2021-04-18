#pragma once

#include "evt_cmd.h"

namespace spm::evt_msg {

extern "C" {

// evt_msg_print(u32 flags, const char * msg, int, const char * speaker)
EVT_DECLARE_USER_FUNC(evt_msg_print, 4)

// evt_msg_continue()
EVT_DECLARE_USER_FUNC(evt_msg_continue, 0)

}

}