/*
    Evt NAND API
    Wrappers for NAND async functions to be called in scripts

    Note: currently, only one of these functions can be used at a time
    and they must not run while the game is doing NAND operations
*/
#pragma once

#include <common.h>
#include <spm/evtmgr.h>

namespace mod {

// evt_nand_create(const char * filename, u8 permissions, u8 attributes,
//                 NANDCommandBlock * commandBlock, &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_create, 5)

// evt_nand_delete(const char * filename, NANDCommandBlock * commandBlock,
//                 &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_delete, 3)

// evt_nand_read(NANDFileInfo * fileInfo, void * dest, u32 length,
//               NANDCommandBlock * commandBlock, &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_read, 5)

// evt_nand_write(NANDFileInfo * fileInfo, void * data, u32 length,
//                NANDCommandBlock * commandBlock, &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_write, 5)

// evt_nand_open(const char * filename, NANDFileInfo * fileInfo, u8 mode,
//               void * buffer, u32 bufferSize, NANDCommandBlock * commandBlock
//               &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_open, 7)

// evt_nand_close(NANDFileInfo * fileInfo, NANDCommandBlock * commandBlock,
//                &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_close, 3)

// evt_nand_check(u32 blockCount, u32 inodeCount, NANDCommandBlock * commandBlock,
//                &s32 ret)
EVT_DECLARE_USER_FUNC(evt_nand_check, 4)

}
