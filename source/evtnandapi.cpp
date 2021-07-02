#include "mod.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/system.h>
#include <wii/NAND.h>
#include <wii/OSError.h>

namespace mod {

using wii::NAND::NANDCommandBlock;
using spm::evtmgr::EvtScriptCode;
using spm::evtmgr_cmd::evtGetValue;
using spm::evtmgr_cmd::evtSetValue;
using wii::NAND::NANDCreateAsync;
using wii::NAND::NANDDeleteAsync;

#define NAND_ASSERT(condition, ret) \
    assertf((condition), MOD_VERSION " Unhandled NAND error %d\n(please report this)", (ret))

#define NAND_LOG_RESULT(ret) \
    wii::OSError::OSReport("%s: result %d\n", __FUNCTION__, (ret))

struct AsyncResult
{
    bool set;
    s32 val;
};
static AsyncResult asyncResult;

static void cb(s32 result, wii::NAND::NANDCommandBlock * cmd)
{
    (void) cmd;
    asyncResult.val = result;
    asyncResult.set = true;
}

s32 evt_nand_create(spm::evtmgr::EvtEntry * entry, bool firstRun)
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
        NAND_ASSERT(ret >= 0, ret);
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

s32 evt_nand_delete(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    EvtScriptCode * args = entry->pCurData;
    const char * filename = (const char *) evtGetValue(entry, args[0]);
    NANDCommandBlock * commandBlock = (NANDCommandBlock *) evtGetValue(entry, args[1]);
    s32 destVar = args[2];

    // On first run, try delete file
    if (firstRun)
    {
        asyncResult.set = false;
        s32 ret = NANDDeleteAsync(filename, cb, commandBlock);;
        NAND_ASSERT(ret >= 0, ret);
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