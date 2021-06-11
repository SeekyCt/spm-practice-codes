#include "evt_cmd.h"
#include "gamesavemenu.h"
#include "mainmenu.h"
#include "patch.h"
#include "util.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_mobj.h>
#include <spm/fontmgr.h>
#include <spm/nandmgr.h>
#include <spm/hud.h>
#include <spm/seq_mapchange.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <wii/OSError.h>
#include <wii/string.h>

namespace mod {

// Just running the save block script doesn't stop the player & enemies moving
static EVT_BEGIN(save_wrapper_evt)
    USER_FUNC(evt_freeze_game)
    RUN_CHILD_EVT(spm::evt_mobj::mobj_save_blk_sysevt)
    USER_FUNC(evt_unfreeze_game)
    RETURN()
EVT_END()

static bool saveGame(MenuButton * button, void * param)
{
    (void) button;
    (void) param;

    // Close menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = nullptr;

    // Run save script
    spm::evtmgr::evtEntryType(save_wrapper_evt, 0, 0, 0);

    // Signal that the menu was closed
    return false;
}

static bool reloadSave(MenuButton * button, void * param)
{
    (void) button;
    (void) param;

    // Close menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = nullptr;

    // Unload current map
    spm::seq_mapchange::_unload(spm::spmario::gp->mapName, nullptr, nullptr);
    wii::string::strcpy(spm::spmario::gp->mapName, "");
    wii::string::strcpy(spm::spmario::gp->doorName, "");

    // Load save file
    spm::nandmgr::nandLoadSave(spm::spmario::gp->saveFileId);

    // Various tasks the game does after loading a save
    spm::spmario::gp->flags |= 4;
    spm::hud::hudLoadStats();
    switch (spm::evtmgr_cmd::evtGetValue(nullptr, GSW(21)))
    {
        case 0x2:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "aa1_01", "pro_1_1");
            break;
        case 0x3:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "he2_01", nullptr);
            break;
        case 0x4:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "he3_01", nullptr);
            break;
        case 0x5:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "he4_01", nullptr);
            break;
        case 0x6:
        case 0xa:
        case 0xe:
        case 0x12:
        case 0x16:
        case 0x1a:
        case 0x1e:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ls4_11", nullptr);
            break;
        case 0x7:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mi2_01", nullptr);
            break;
        case 0x8:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mi3_01", nullptr);
            break;
        case 0x9:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mi4_01", nullptr);
            break;
        case 0xb:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ta2_01", nullptr);
            break;
        case 0xc:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ta3_01", nullptr);
            break;
        case 0xd:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ta4_01", nullptr);
            break;
        case 0xf:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "sp2_01", nullptr);
            break;
        case 0x10:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "sp3_01", nullptr);
            break;
        case 0x11:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "sp4_01", nullptr);
            break;
        case 0x13:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "gn2_01", nullptr);
            break;
        case 0x14:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "gn3_01", nullptr);
            break;
        case 0x15:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "gn4_01", nullptr);
            break;
        case 0x17:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "wa2_01", nullptr);
            break;
        case 0x18:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "wa3_01", nullptr);
            break;
        case 0x19:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "wa4_01", nullptr);
            break;
        case 0x1b:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "an2_01", nullptr);
            break;
        case 0x1c:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "an3_01", nullptr);
            break;
        case 0x1d:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "an4_01", nullptr);
            break;
        case 0x1f:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ls2_01", nullptr);
            break;
        case 0x20:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ls3_01", nullptr);
            break;
        case 0x21:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "ls4_01", nullptr);
            break;
        case 0x22:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", nullptr);
            break;
        case 0x23:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa6_l");
            break;
        case 0x24:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa8_l");
            break;
        case 0x25:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa9_l");
            break;
        case 0x26:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa10_l");
            break;
        case 0x27:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa11_l");
            break;
        case 0x28:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa12_l");
            break;
        case 0x29:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_02", "doa13_l");
            break;
        case 0x2a:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, "mac_12", "doa6_l");
            break;
        default:
            spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, spm::spmario::gp->mapName, "save");
            break;
    }

    // Signal that the menu was closed
    return false;
}

GameSaveMenu::GameSaveMenu()
{
    const f32 optionsX = -330.0f;
    f32 y = 170;
    MenuButton * buttons[2];
    buttons[0] = new MenuButton(this, "Save Game", optionsX, y, saveGame);
    buttons[1] = new MenuButton(this, "Reload Save", optionsX, y - (FONT_HEIGHT + 5), reloadSave);
    buttonLinkVertical(buttons[0], buttons[1]);

    mCurButton = buttons[0];

    mTitle = "Game Save Options";
}

void GameSaveMenu::close()
{
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

void (*nandUpdateSaveReal)(int saveId);

void GameSaveMenu::pitSavePatch()
{
    // The game increments GSW(1), which indicates which pit room to load next, after the
    // current room is loaded, so saving & reloading would put you into the next floor.
    // This fixes that by decreasing GSW(1) in the save but not in the live game.
    nandUpdateSaveReal = patch::hookFunction(spm::nandmgr::nandUpdateSave,
        [](int saveId)
        {
            nandUpdateSaveReal(saveId);
            if (wii::string::strncmp(spm::spmario::gp->mapName, "dan", 3) == 0)
            {
                spm::nandmgr::SaveFile * save = spm::nandmgr::nandGetSaveFiles() + saveId;
                save->spmarioGlobals.gsw[1] -= 1;
            }
        }
    );
}

}
