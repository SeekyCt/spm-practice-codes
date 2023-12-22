#include <common.h>

#include <spm/camdrv.h>
#include <spm/charpixlitemwin.h>
#include <spm/effdrv.h>
#include <spm/fadedrv.h>
#include <spm/homebuttondrv.h>
#include <spm/mario.h>
#include <spm/mario_motion.h>
#include <spm/relmgr.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/system.h>
#include <spm/wpadmgr.h>
#include <wii/dvd.h>
#include <wii/ipc.h>
#include <wii/os.h>
#include <wii/wpad.h>
#include <msl/string.h>

#include "mod_ui_base/window.h"
#include "apwindow.h"
#include "chainloader.h"
#include "consolewindow.h"
#include "custompit.h"
#include "evtdebug.h"
#include "exception.h"
#include "gamesavemenu.h"
#include "gcpad.h"
#include "hitboxmenu.h"
#include "hpwindow.h"
#include "inputwindow.h"
#include "mainmenu.h"
#include "mapdoorwindow.h"
#include "mapfixes.h"
#include "mapselectmenu.h"
#include "mod.h"
#include "nandsettings.h"
#include "patch.h"
#include "pyconsole.h"
#include "parsepatches.h"
#include "preventpatchtwice.h"
#include "romfontexpand.h"
#include "scriptlog.h"
#include "scriptvarlog.h"
#include "titletextwindow.h"
#include "xyzwindow.h"

namespace mod {

bool gIsDolphin;
bool gIsRiivolution;
bool gIsPatchedDisc;
bool gIs4_3;

/*
    seq_game hooks
*/

static spm::seqdef::SeqDef seq_gameReal;
static spm::seqdef::SeqDef seq_mapChangeReal;

static void seq_gameInitOverride(spm::seqdrv::SeqWork * wp)
{
    MapSelectMenu::onMapChanged();

    seq_gameReal.init(wp);
}

static void seq_gameMainOverride(spm::seqdrv::SeqWork *wp)
{
    // Check if a window could be opened
    if (spm::wpadmgr::wpadGetButtonsPressed(0) & WPAD_BTN_B)
    {
        if (MenuWindow::sCurMenu == nullptr)
        {
            if (spm::fadedrv::fadeIsFinish() &&
                !spm::mario::marioCtrlOffChk() &&
                !spm::mario::marioKeyOffChk() &&
                spm::mario_motion::func_80146f0c() &&
                spm::effdrv::func_80061d78() == 0 &&
                spm::charpixlitemwin::charPixlItemWinIsClosed() &&
                spm::spmario::spmarioGetSystemLevel() == 0 &&
                (spm::homebuttondrv::homebuttondrv_wp->flags & HOMEBUTTON_FLAG_OPEN) == 0 &&
                (spm::homebuttondrv::homebuttondrv_wp->flags & HOMEBUTTON_FLAG_FORBIDDEN) == 0
                )
                MenuWindow::sCurMenu = new MainMenu();
        }
        else
        {
            MenuWindow::sCurMenu->fullClose();
        }
    }

    seq_gameReal.main(wp);
}

static void seq_gamePatch()
{
    seq_gameReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME];
    spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME] = {
        seq_gameInitOverride, seq_gameMainOverride, seq_gameReal.exit
    };
}

static void seq_mapChangeInitOverride(spm::seqdrv::SeqWork *wp) {
    // Fix potential crashes
    mapLoadCrashFixes(wp);
    
    // Call real function
    seq_mapChangeReal.init(wp);
}

static void seq_mapChangePatch()
{
    seq_mapChangeReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_MAPCHANGE];
    spm::seqdef::seq_data[spm::seqdrv::SEQ_MAPCHANGE] = {
        seq_mapChangeInitOverride, seq_mapChangeReal.main, seq_mapChangeReal.exit
    };
}

/*
    spmarioMain hook
*/

static void (*spmarioMainReal)();

void spmarioMainPatch()
{
    spmarioMainReal = patch::hookFunction(spm::spmario::spmarioMain,
        []()
        {
#ifdef PYCONSOLE_PROTOTYPE
            PyConsole::main();
#endif
            Window::windowMain();
            spmarioMainReal();
        }
    );
}

/*
    Platform detection to adjust certain settings
*/

static void checkForDolphin()
{
    // Thanks to TheLordScruffy for telling me about this
    gIsDolphin = wii::ipc::IOS_Open("/sys", 1) == -106;

    // If they ever fix that, it'll be in a version that's definitely new enough to have /dev/dolphin
    if (!gIsDolphin)
    {
        int ret = wii::ipc::IOS_Open("/dev/dolphin", 0);
        if (ret >= 0)
        {
            gIsDolphin = true;
            wii::ipc::IOS_Close(ret);
        }
    }
}

static void checkForRiivolution()
{
    // Thanks to TheLordScruffy for telling me about this
    gIsRiivolution = msl::string::strcmp(wii::dvd::devDiStr, "/dev/di") != 0;
}

static void checkForPatchedDisc()
{
    gIsPatchedDisc = wii::dvd::DVDConvertPathToEntrynum("./mod/") != -1;
}

/*
    Entrypoint
*/

void main()
{
    wii::os::OSReport(MOD_VERSION": main running\n");

    checkForDolphin();
    checkForRiivolution();
    checkForPatchedDisc();

#if !(defined SPM_JP0 || defined SPM_JP1 || defined SPM_US0)
    // Fix dolphin hanging on game shutdown
    if (gIsDolphin)
        writeWord(spm::spmario_snd::spsndExit, 0, BLR);
#endif

    romfontExpand();
    exceptionPatch();

    // Handle 4:3
    spm::camdrv::CamEntry * cam = spm::camdrv::camGetPtr(spm::camdrv::CAM_ID_DEBUG_3D);
    gIs4_3 = (cam->flag & CAMFLAG_16_9) == 0;
    if (gIs4_3)
        cam->pos.z = 1350.0f;

    ConsoleWindow::init();
    HPWindow::init();
    MapDoorWindow::init();
    XYZWindow::init();
    TitleTextWindow::init();

    spmarioMainPatch();
    seq_gamePatch();
    seq_mapChangePatch();
    evtScriptLoggerPatch();
    evtVarLogPatch();
    evtDebugPatch();
    nandSettingsPatch();
    GameSaveMenu::pitSavePatch();
    MapSelectMenu::scanEntrances();
    customPitPatch();
    parsePatch();
#ifdef PYCONSOLE_PROTOTYPE
    PyConsole::init();
    APWindow::init();
#endif
    InputWindow::init();
    HitboxMenu::hitboxPatch();
    gcpadPatch();

    loadMainPatch();

    /*
        Uncomment this if you'd like to load another mod alongside practice codes
        (for example, to debug it). If you'd like to link against this mod, then
        make sure to compile it with EXTRAFLAGS="-fno-function-sections -fno-data-sections"
        to prevent anything you need being deadstripped
    */
    // tryChainload();
}

}
