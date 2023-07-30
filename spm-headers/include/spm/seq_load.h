#pragma once

#include <common.h>
#include <spm/seqdrv.h>
#include <spm/evtmgr.h>
#include <spm/filemgr.h>

#include <wii/gx.h>

CPP_WRAPPER(spm::seq_load)

USING(spm::seqdrv::SeqWork)
USING(spm::evtmgr::EvtEntry)
USING(spm::filemgr::FileEntry)
USING(wii::gx::GXColor)

typedef struct {
    u32 flags;
    u32 state;
    FileEntry* fileselTpl;
    s32 selectedSave;
    u8 _10[0x2c-0x10];
    EvtEntry* saveUpdateEvt;
    s32 saveUpdateEvtId;
    u8 _34[0x48-0x34];
    GXColor bgGradientTop;
    GXColor bgGradientBottom;
    u8 _50[0x5c-0x50];
} SeqLoadWork;
SIZE_ASSERT(SeqLoadWork, 0x5c);

DECOMP_STATIC(SeqLoadWork* seqLoadWp)

UNKNOWN_FUNCTION(initNewSaveFile);
void seq_loadInit(SeqWork * work);
void seq_loadMain(SeqWork * work);
void seq_loadExit(SeqWork * work);

s32 seq_load_sub_loadMain(); // from seq_load_sub

CPP_WRAPPER_END()
