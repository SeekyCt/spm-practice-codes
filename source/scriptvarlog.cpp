#include "consolewindow.h"
#include "mod.h"
#include "nandsettings.h"
#include "patch.h"
#include "scriptvarlog.h"

#include <types.h>
#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <wii/OSError.h>
#include <wii/string.h>
#include <wii/stdio.h>

namespace mod {

enum
{
	LOGTYPE_GSW  = 0,
	LOGTYPE_GSWF = 1,
	LOGTYPE_LSW  = 2,
	LOGTYPE_LSWF = 3
};

static const char *logTypeNames[] = {"GSW", "GSWF", "LSW", "LSWF"};

// Handle a variable changing
static void evtVarLog(s32 type, s32 id, s32 value)
{
	// Only log changes to actual save
	if (wii::string::strcmp(spm::spmario::gp->saveName, "default") == 0)
		return;

	char * p;
	switch(settings->logOptions[OPTION_EVT_VAR_LOG])
	{
		case LogType::NONE:
			break;
		case LogType::OSREPORT:
			// Write to OSReport
			wii::OSError::OSReport("%s %d set to 0x%x\n", logTypeNames[type], id, value);
			break;
		case LogType::SCREEN:
			// Format string
			p = new char[32];
			wii::stdio::sprintf(p, "%s %d set to 0x%x", logTypeNames[type], id, value);

			// Write to console
			ConsoleWindow::sInstance->push(p,
				[](const char * line)
				{
					delete[] line;
				}
			);
			break;
	}
}

static void (*swSet_real)(s32);
static void (*swClear_real)(s32);
static void (*swByteSet_real)(s32, s32);
static void (*_swSet_real)(s32);
static void (*_swClear_real)(s32);
static void (*_swByteSet_real)(s32, s8);

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
		[](s32 id, s8 num)
		{
			evtVarLog(LOGTYPE_LSW, id, num);
			_swByteSet_real(id, num);
		}
	);
}

}
