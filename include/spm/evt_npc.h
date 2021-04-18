#pragma once

#include <spm/evtmgr.h>

namespace spm::evt_npc {

extern "C" {

// evt_npc_teleport_effect(bool appearDisappear, const char * npcName)
// Found by JohnP55
EVT_DECLARE_USER_FUNC(evt_npc_teleport_effect, 2)

}

}
