#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_npc {

extern "C" {

// evt_npc_set_position(const char * instanceName, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_npc_set_position, 4)

// evt_npc_entry(const char * instanceName, const char * animPoseName, int instanceId)
EVT_DECLARE_USER_FUNC(evt_npc_entry, 3)

EVT_DECLARE_USER_FUNC(func_80103054, 1)
EVT_DECLARE_USER_FUNC(evt_npc_set_anim, 3)
EVT_DECLARE_USER_FUNC(func_80104694, 2)

// evt_npc_flag8_onoff(const char * instanceName, bool on, u32 mask)
EVT_DECLARE_USER_FUNC(evt_npc_flag8_onoff, 3)

// evt_npc_animflag_onoff(const char * instanceName, bool on, u32 mask)
EVT_DECLARE_USER_FUNC(evt_npc_animflag_onoff, 3)

// evt_npc_set_property(const char * instanceName, int propertyId, int value)
EVT_DECLARE_USER_FUNC(evt_npc_set_property, 3)

// evt_npc_set_property(const char * instanceName, int partId, int propertyId, int value)
// partId -1 updates all parts
EVT_DECLARE_USER_FUNC(evt_npc_modify_part, 4)

EVT_DECLARE_USER_FUNC(func_801059d0, 2)

// evt_npc_entry_from_template(int instanceIdOverride, int templateId, int x, int y,
//                             int z, &int instanceId, &char * instanceName)
EVT_DECLARE_USER_FUNC(evt_npc_entry_from_template, 7)

EVT_DECLARE_USER_FUNC(func_80107cfc, 0)
EVT_DECLARE_USER_FUNC(func_80107d20, 0)
EVT_DECLARE_USER_FUNC(func_80108194, 2)

// evt_npc_teleport_effect(bool appearDisappear, const char * instanceName)
// Found by JohnP55
EVT_DECLARE_USER_FUNC(evt_npc_teleport_effect, 2)

}

}
