#include "mod_ui_base/window.h"
#include "consolewindow.h"
#include "evtdebug.h"
#include "exception.h"
#include "mainmenu.h"
#include "mapdoorwindow.h"
#include "mod.h"
#include "patch.h"
#include "mapselectmenu.h"
#include "nandsettings.h"
#include "romfontexpand.h"
#include "scriptlog.h"
#include "scriptvarlog.h"
#include "titletextwindow.h"
#include "xyzwindow.h"

#include <types.h>
#include <spm/charpixlitemwin.h>
#include <spm/effdrv.h>
#include <spm/fadedrv.h>
#include <spm/mario.h>
#include <spm/mario_motion.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/wpadmgr.h>
#include <wii/ipc.h>
#include <wii/OSError.h>
#include <wii/stdio.h>
#include <wii/wpad.h>

namespace mod {

bool isDolphin;

/*
    seq_title hooks
*/

static spm::seqdef::SeqDef seq_titleReal;

static void seq_titleInitOverride(spm::seqdrv::SeqWork * wp)
{
    TitleTextWindow::sInstance = new TitleTextWindow();

    seq_titleReal.init(wp);
}

static void seq_titleExitOverride(spm::seqdrv::SeqWork * wp)
{
    delete TitleTextWindow::sInstance;
    TitleTextWindow::sInstance = nullptr;

    seq_titleReal.exit(wp);
}

static void seq_titlePatch()
{
    seq_titleReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE];
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE] = {
        seq_titleInitOverride, seq_titleReal.main, seq_titleExitOverride
    };
}

/*
    seq_game hooks
*/

static spm::seqdef::SeqDef seq_gameReal;

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
            if (spm::fadedrv::fadeIsFinish() && !spm::mario::marioCtrlOffChk() &&
                !spm::mario::marioKeyOffChk() && spm::mario_motion::func_80146f0c() &&
                (spm::effdrv::func_80061d78() == 0) && spm::charpixlitemwin::charPixlItemWinIsClosed()
                && (spm::spmario::spmarioGetSystemLevel() == 0))
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

/*
    spmarioMain hook
*/

static void (*spmarioMainReal)();

void spmarioMainPatch()
{
    spmarioMainReal = patch::hookFunction(spm::spmario::spmarioMain,
        []()
        {
            Window::windowMain();
            spmarioMainReal();
        }
    );
}

/*
    Entrypoint
*/

void main()
{
    wii::OSError::OSReport(MOD_VERSION": main running\n");

    // Thanks to TheLordScruffy for telling me about this
    isDolphin = wii::IPC::IOS_Open("/sys", 1) == -106;

    // Fix dolphin hanging on game shutdown
    if (isDolphin)
        writeWord(spm::spmario_snd::spsndExit, 0, BLR);

    romfontExpand();
    exceptionPatch();

    ConsoleWindow::sInstance = new ConsoleWindow();
    MapDoorWindow::sInstance = new MapDoorWindow();
    XYZWindow::sInstance = new XYZWindow();

    spmarioMainPatch();
    seq_titlePatch();
    seq_gamePatch();
    evtScriptLoggerPatch();
    evtVarLogPatch();
    evtDebugPatch();
    nandSettingsPatch();
}

}