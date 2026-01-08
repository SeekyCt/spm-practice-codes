#pragma once

#include <common.h>
#include <spm/seqdrv.h>
#include <spm/evtmgr.h>
#include <spm/filemgr.h>

#include <wii/gx.h>

CPP_WRAPPER(spm::seq_load)

USING(spm::seqdrv::SeqWork)
USING(spm::evtmgr::EvtEntry)

UNKNOWN_FUNCTION(initNewSaveFile)
void seq_loadInit(SeqWork * work);
void seq_loadMain(SeqWork * work);
void seq_loadExit(SeqWork * work);

CPP_WRAPPER_END()
