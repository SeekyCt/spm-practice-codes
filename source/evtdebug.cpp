#include "consolewindow.h"
#include "evtdebug.h"
#include "nandsettings.h"
#include "patch.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <wii/OSError.h>
#include <wii/string.h>

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
            wii::OSError::OSReport("%s\n", str);
            break;
        case LogType::SCREEN:
            // Write to screen
            ConsoleWindow::sInstance->push(str);
            break;
    }

    return 2;
}

static s32 debugPutReg(s32 ret)
{
    char * str;
    switch(gSettings->logOptions[OPTION_DEBUG_INSTR])
    {
        case LogType::NONE:
            break;
        case LogType::OSREPORT:
            // Write to OSReport
            wii::OSError::OSReport("%s\n", spm::evtmgr_cmd::evt_debug_put_reg_str);
            break;
        case LogType::SCREEN:
            // String could be changed while still in use, so a copy is made
            str = new char[wii::string::strlen(spm::evtmgr_cmd::evt_debug_put_reg_str) + 1];
            wii::string::strcpy(str, spm::evtmgr_cmd::evt_debug_put_reg_str);

            // Write to screen
            ConsoleWindow::sInstance->push(str,
                [](const char * line)
                {
                    delete[] line;
                }
            );
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
