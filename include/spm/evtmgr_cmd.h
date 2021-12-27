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

// Stop execution of all scripts this frame
#define EVT_RET_END_FRAME -1

// Keep re-executing current instruction, continue next frame if too much time passed
#define EVT_RET_BLOCK_WEAK 0

// Move to next instruction, continue next frame if too much time passed
#define EVT_RET_CONTINUE_WEAK 1

// Move to next instruction
#define EVT_RET_CONTINUE 2

// Keep re-executing current instruction (unused?)
#define EVT_RET_BLOCK 3

// Stop running current script
#define EVT_RET_END 0xff

extern "C" {

s32 evtmgrCmd(spm::evtmgr::EvtEntry * entry);

s32 evt_debug_put_msg(spm::evtmgr::EvtEntry * entry);
s32 evt_debug_put_reg(spm::evtmgr::EvtEntry * entry);
extern char evt_debug_put_reg_str[256];

s32 evtGetValue(spm::evtmgr::EvtEntry * entry, s32 var);
s32 evtSetValue(spm::evtmgr::EvtEntry * entry, s32 var, s32 val);
f32 evtSetFloat(spm::evtmgr::EvtEntry * entry, s32 var, f32 val);

// more

}

}
