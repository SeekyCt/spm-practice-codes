#pragma once

#include <types.h>

namespace spm::seq_mapchange {

struct SeqMapChangeWork
{
    const char areaName[32];
    const char mapName[32];
    const char beroName[32];
    spm::evtmgr::EvtEntry* initEvtEntry;
    s32 initEvtEntryId;
    u8 _68[0x74-0x68];
};
static_assert(sizeof(SeqMapChangeWork) == 0x74);

extern "C" {

void _unload(const char * curMapName, const char * nextMapName, const char * nextDoorName);
extern SeqMapChangeWork seqMapChangeWork;

}

}