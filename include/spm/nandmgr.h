#pragma once

#include "evt_cmd.h"

#include <spm/mario_pouch.h>
#include <spm/spmario.h>

namespace spm::nandmgr {

struct SaveFile
{
    u16 flags;
    u8 unknown_0x2[0x8 - 0x2];
    spm::spmario::SpmarioGlobals spmarioGlobals;
    spm::mario_pouch::MarioPouchWork pouchWork;
    u8 unknown_0x21b0[0x25b0 - 0x21b0];
    u32 checksum;
    u32 checksumNOT;
};
static_assert(sizeof(SaveFile) == 0x25b8);

extern "C" {

void nandUpdateSave(int saveId);
void nandLoadSave(int saveId);
SaveFile * nandGetSaveFiles();

}

}