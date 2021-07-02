/*
    Evt NAND API
    Wrappers for NAND async functions to be called in scripts
*/
#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace mod {

// evt_nand_create(const char * filename, u8 permissions, u8 attributes, NANDCommandBlock * commandBlock, &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_create, 5)

// evt_nand_delete(const char * filename, NANDCommandBlock * commandBlock, &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_delete, 3)

// evt_nand_read(NANDFileInfo * fileInfo, void * dest, u32 length, NANDCommandBlock * commandBlock, &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_read, 5)

}
