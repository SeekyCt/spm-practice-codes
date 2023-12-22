#include <common.h>
#include <spm/evtmgr.h>
#include <wii/os.h>
#include <msl/stdio.h>

#include "consolewindow.h"
#include "mod.h"
#include "nandsettings.h"
#include "patch.h"
#include "util.h"

using EvtEntry = spm::evtmgr::EvtEntry;
using EvtScriptCode = spm::evtmgr::EvtScriptCode;

namespace mod {

// Handle a script being passed into any entry function
static void evtEntryLog(const EvtScriptCode * script)
{
    // Don't log scripts coming from this rel
    void * modRelAddr = getModRelLoadAddr();
    if ((u32)script >= (u32)modRelAddr)
        return;

    switch (gSettings->logOptions[OPTION_SCRIPT_LOG])
    {
        case LogType::NONE:
            break;

        case LogType::OSREPORT:
            // Write to OSReport
            wii::os::OSReport("Evt entry: 0x%x\n", (u32) script);
            break;

        case LogType::SCREEN:
            // Write to screen
            CONSOLE_PUSH_FMT("Evt entry: 0x%x", (u32) script);
            break;
    }
}

static EvtEntry *(*evtEntryOriginal)(const EvtScriptCode *, u32 priority, u8 flags) = nullptr;
static EvtEntry *(*evtEntryTypeOriginal)(const EvtScriptCode * script, u32 priority, u8 flags, u8 type) = nullptr;
static EvtEntry *(*evtChildEntryOriginal)(EvtEntry * entry, const EvtScriptCode * script, u8 flags) = nullptr;
static EvtEntry *(*evtBrotherEntryOriginal)(EvtEntry * entry, const EvtScriptCode * script, u8 flags) = nullptr;

void evtScriptLoggerPatch()
{
    evtEntryOriginal = patch::hookFunction(spm::evtmgr::evtEntry,
        [](const EvtScriptCode * script, u32 priority, u8 flags)
        {
            evtEntryLog(script);
            return evtEntryOriginal(script, priority, flags);
        }
    );
    evtEntryTypeOriginal = patch::hookFunction(spm::evtmgr::evtEntryType, 
        [](const EvtScriptCode * script, u32 priority, u8 flags, u8 type)
        {
            evtEntryLog(script);
            return evtEntryTypeOriginal(script, priority, flags, type);
        }
    );
    evtChildEntryOriginal = patch::hookFunction(spm::evtmgr::evtChildEntry,
        [](EvtEntry * entry, const EvtScriptCode * script, u8 flags)
        {
            evtEntryLog(script);
            return evtChildEntryOriginal(entry, script, flags);
        }
    );
    evtBrotherEntryOriginal = patch::hookFunction(spm::evtmgr::evtBrotherEntry,
        [](EvtEntry * entry, const EvtScriptCode * script, u8 flags)
        {
            evtEntryLog(script);
            return evtBrotherEntryOriginal(entry, script, flags);
        }
    );
}

}
