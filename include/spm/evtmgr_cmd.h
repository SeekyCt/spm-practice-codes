#pragma once

#include <types.h>

#include <spm/evtmgr.h>

namespace spm::evtmgr_cmd {

#define EVTDAT_FLOAT_BASE 240000000
#define EVTDAT_UF_BASE 210000000
#define EVTDAT_UW_BASE 190000000
#define EVTDAT_GSW_BASE 170000000
#define EVTDAT_LSW_BASE 150000000
#define EVTDAT_GSWF_BASE 130000000
#define EVTDAT_LSWF_BASE 110000000
#define EVTDAT_GF_BASE 90000000
#define EVTDAT_LF_BASE 70000000
#define EVTDAT_GW_BASE 50000000
#define EVTDAT_LW_BASE 30000000

extern "C" {

s32 evtGetValue(spm::evtmgr::EvtEntry * entry, s32 var);
s32 evtSetValue(spm::evtmgr::EvtEntry * entry, s32 var, s32 val);

s32 evt_debug_put_msg(spm::evtmgr::EvtEntry * entry);
s32 evt_debug_put_reg(spm::evtmgr::EvtEntry * entry);
extern char evt_debug_put_reg_str[256];

// more

}

}
