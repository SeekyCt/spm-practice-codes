#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/system.h>
#include <wii/NAND.h>
#include <wii/os.h>

#include "mod.h"

namespace mod {

using wii::nand::NANDCommandBlock;
using wii::nand::NANDFileInfo;
using wii::nand::NANDCreateAsync;
using wii::nand::NANDDeleteAsync;
using wii::nand::NANDReadAsync;
using wii::nand::NANDWriteAsync;
using wii::nand::NANDSafeOpenAsync;
using wii::nand::NANDSafeCloseAsync;
using wii::nand::NANDCheckAsync;
using spm::evtmgr::EvtScriptCode;
using spm::evtmgr::EvtEntry;
using spm::evtmgr_cmd::evtGetValue;
using spm::evtmgr_cmd::evtSetValue;

#define NAND_VERIFY_RET(ret) \
    assertf((ret) >= 0, MOD_VERSION " Unhandled NAND error %d\n(please report this)", (ret))

#define NAND_LOG_RESULT(ret) \
    wii::os::OSReport("%s: result %d\n", __FUNCTION__, (ret))

struct AsyncResult
{
    bool set;
    s32 val;
};
static AsyncResult asyncResult;
static u32 checkAnswer;

static void cb(s32 result, wii::nand::NANDCommandBlock * cmd)
{
    (void) cmd;
    asyncResult.val = result;
    asyncResult.set = true;
}

s32 evt_nand_create(EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    const char * filename = (const char *) evtGetValue(entry, args[0]);
    u8 permissions = (u8) evtGetValue(entry, args[1]);
    u8 attributes = (u8) evtGetValue(entry, args[2]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[3]);
    s32 destVar = args[4];

    // On first run, try create the file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDCreateAsync(filename, permissions, attributes, cb, commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

s32 evt_nand_delete(EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    const char * filename = (const char *) evtGetValue(entry, args[0]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[1]);
    s32 destVar = args[2];

    // On first run, try delete file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDDeleteAsync(filename, cb, commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

s32 evt_nand_read(EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    NANDFileInfo * fileInfo = (NANDFileInfo *) evtGetValue(entry, args[0]);
    void * dest = (void *) evtGetValue(entry, args[1]);
    u32 length = (u32) evtGetValue(entry, args[2]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[3]);
    s32 destVar = args[4];

    // On first run, try read the file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDReadAsync(fileInfo, dest, length, cb, commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

s32 evt_nand_write(EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    NANDFileInfo * fileInfo = (NANDFileInfo *) evtGetValue(entry, args[0]);
    void * data = (void *) evtGetValue(entry, args[1]);
    u32 length = (u32) evtGetValue(entry, args[2]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[3]);
    s32 destVar = args[4];

    // On first run, try read the file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDWriteAsync(fileInfo, data, length, cb, commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

s32 evt_nand_open(EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    const char * filename = (const char *) evtGetValue(entry, args[0]);
    NANDFileInfo * fileInfo = (NANDFileInfo *) evtGetValue(entry, args[1]);
    u8 mode = (u8) evtGetValue(entry, args[2]);
    void * buffer = (void *) evtGetValue(entry, args[3]);
    u32 bufferSize = (u32) evtGetValue(entry, args[4]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[5]);
    s32 destVar = args[6];

    // On first run, try open the file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDSafeOpenAsync(filename, fileInfo, mode, buffer, bufferSize, cb,
                                    commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

s32 evt_nand_close(EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    NANDFileInfo * fileInfo = (NANDFileInfo *) evtGetValue(entry, args[0]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[1]);
    s32 destVar = args[2];

    // On first run, try close file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDSafeCloseAsync(fileInfo, cb, commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

static void checkCb(s32 result, wii::nand::NANDCommandBlock * cmd)
{
    (void) cmd;

    // Store result
    asyncResult.val = result;
    
    // If there was no error checking, review the answer
    if (result == NAND_CODE_OK)
    {
        // Game checks for these separately, but both mean we shouldn't continue
        if (checkAnswer & (0xa | 0x5))
            asyncResult.val = -128;

        asyncResult.set = true;
    }
}

s32 evt_nand_check(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    u32 blockCount = (u32) evtGetValue(entry, args[0]);
    u32 inodeCount = (u32) evtGetValue(entry, args[1]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[2]);
    s32 destVar = args[3];

    // On first run, start check process
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDCheckAsync(blockCount, inodeCount, &checkAnswer, checkCb, commandBlock);
        NAND_VERIFY_RET(ret);
    }

    // If the async process has finished, return to script
    if (asyncResult.set)
    {
        NAND_LOG_RESULT(asyncResult.val);
        evtSetValue(entry, destVar, asyncResult.val);
        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

}