#include <common.h>
#include <spm/evtmgr_cmd.h>
#include <wii/os.h>
#include <string.h>

#include "consolewindow.h"
#include "evtdebug.h"
#include "nandsettings.h"
#include "patch.h"
#include "util.h"

namespace mod {

static s32 debugPutMsg(spm::evtmgr::EvtEntry * entry)
{
    // Write first arg to on-screen console
    const char * str = (char *) spm::evtmgr_cmd::evtGetValue(entry, entry->pCurData[0]);
    switch (gSettings->logOptions[OPTION_DEBUG_INSTR])
    {
        case LogType::NONE:
            break;

        case LogType::OSREPORT:
            // Write to OSReport
            wii::os::OSReport("%s\n", str);
            break;

        case LogType::SCREEN:
            // Write to screen
            ConsoleWindow::push(str);
            break;
    }

    return 2;
}

static s32 debugPutReg(s32 ret)
{
    switch(gSettings->logOptions[OPTION_DEBUG_INSTR])
    {
        case LogType::NONE:
            break;

        case LogType::OSREPORT:
            // Write to OSReport
            wii::os::OSReport("%s\n", spm::evtmgr_cmd::evt_debug_put_reg_str);
            break;

        case LogType::SCREEN:
            // Write to screen (cloned since it could be changed while still in use)
            ConsoleWindow::pushClone(spm::evtmgr_cmd::evt_debug_put_reg_str);
            break;
    }

    return ret;
}

void evtDebugPatch()
{
    // Replace debug_put_msg
    writeBranch(spm::evtmgr_cmd::evt_debug_put_msg, 0, debugPutMsg);

    // Hook on the blr of debug_put_reg
    writeBranch(spm::evtmgr_cmd::evt_debug_put_reg, 0x7b4, debugPutReg);
}

}
