#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_hit {

extern "C" {

// evt_hitobj_attr_onoff(bool group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_hitobj_attr_onoff, 4)

}

}
