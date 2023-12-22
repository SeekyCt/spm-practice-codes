#include <common.h>
#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <wii/os.h>
#include <msl/string.h>
#include <msl/stdio.h>

#include "consolewindow.h"
#include "mod.h"
#include "nandsettings.h"
#include "patch.h"
#include "scriptvarlog.h"
#include "scriptvarmenu.h"
#include "sequencemenu.h"

namespace mod {

enum
{
    LOGTYPE_GSW  = 0,
    LOGTYPE_GSWF = 1,
    LOGTYPE_LSW  = 2,
    LOGTYPE_LSWF = 3
};

static const char * logTypeNames[] = {"GSW", "GSWF", "LSW", "LSWF"};

static bool externalDisable = false;

void scriptVarLogOnOff(bool enabled)
{
    externalDisable = !enabled;
}

// Handle a variable changing
static void evtVarLog(s32 type, s32 id, s32 value)
{
    // Don't print when other code has disabled it
    if (externalDisable)
        return;

    // Only log changes to actual save
    if (msl::string::strcmp(spm::spmario::gp->saveName, "default") == 0)
        return;

    switch(gSettings->logOptions[OPTION_EVT_VAR_LOG])
    {
        case LogType::NONE:
            // Don't print when user has disabled it
            break;
        case LogType::OSREPORT:
            // Write to OSReport
            wii::os::OSReport("%s %d set to 0x%x\n", logTypeNames[type], id, value);
            break;
        case LogType::SCREEN:
            // Write to screem
            CONSOLE_PUSH_FMT("%s %d set to 0x%x", logTypeNames[type], id, value);
            break;
    }
}

static void (*swSet_real)(s32);
static void (*swClear_real)(s32);
static void (*swByteSet_real)(s32, s32);
static void (*_swSet_real)(s32);
static void (*_swClear_real)(s32);
static void (*_swByteSet_real)(s32, u8);

void evtVarLogPatch()
{
    swSet_real = patch::hookFunction(spm::swdrv::swSet,
        [](s32 id)
        {
            evtVarLog(LOGTYPE_GSWF, id, 1);
            swSet_real(id);
        }
    );
    swClear_real = patch::hookFunction(spm::swdrv::swClear,
        [](s32 id)
        {
            evtVarLog(LOGTYPE_GSWF, id, 0);
            swClear_real(id);
        }
    );
    swByteSet_real = patch::hookFunction(spm::swdrv::swByteSet,
        [](s32 id, s32 num)
        {
            evtVarLog(LOGTYPE_GSW, id, num);
            swByteSet_real(id, num);
        }
    );
    _swSet_real = patch::hookFunction(spm::swdrv::_swSet,
        [](s32 id)
        {
            evtVarLog(LOGTYPE_LSWF, id, 1);
            _swSet_real(id);
        }
    );
    _swClear_real = patch::hookFunction(spm::swdrv::_swClear,
        [](s32 id)
        {
            evtVarLog(LOGTYPE_LSWF, id, 0);
            _swClear_real(id);
        }
    );
    _swByteSet_real = patch::hookFunction(spm::swdrv::_swByteSet,
        [](s32 id, u8 num)
        {
            evtVarLog(LOGTYPE_LSW, id, num);
            _swByteSet_real(id, num);
        }
    );
}

}
