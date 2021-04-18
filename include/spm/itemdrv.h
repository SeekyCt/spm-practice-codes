#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::itemdrv {

extern "C" {

void * itemEntry(const char * name, s32 type, f32 x, f32 y, f32 z, s32 behaviour,
                 spm::evtmgr::EvtScriptCode * pickupScript, int switchNumber);

}

}
