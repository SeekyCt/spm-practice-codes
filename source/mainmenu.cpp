#include "gamesavemenu.h"
#include "hudmenu.h"
#include "inventorymenu.h"
#include "logmenu.h"
#include "mainmenu.h"
#include "mapselectmenu.h"
#include "miscmenu.h"
#include "mod.h"
#include "nandmenu.h"
#include "pitselectmenu.h"
#include "quitmenu.h"
#include "scriptvarmenu.h"
#include "statmenu.h"

#include <types.h>
#include <spm/fontmgr.h>

namespace mod {

s32 MainMenu::sLastOption = 0;

MainMenu::MainMenu()
{
    const f32 optionsX = -330.0f;
    f32 y = 170;
    int n = 0;
    mOptions[n++] = new MenuButton(this, "Change Map", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new MapSelectMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Warp To Pit Floor", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new PitSelectMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Edit HUD Display Options", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new HudMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Edit Logging Options", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new LogMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Edit Script Variables", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new ScriptVarMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Edit Inventory", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new InventoryMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Edit Stats", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new StatMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Miscellaneous Options", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new MiscMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Manage Saved Settings", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new NandMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Game Save Options", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new GameSaveMenu();
            return false;
        }
    );
    y -= FONT_HEIGHT + 5;
    mOptions[n++] = new MenuButton(this, "Return to Title Screen", optionsX, y,
        [](MenuButton * button, void * param)
        {
            (void) button;
            (void) param;

            delete MenuWindow::sCurMenu;
            MenuWindow::sCurMenu = new QuitMenu();
            return false;
        }
    );

    for (int i = 0; i < MAIN_MENU_OPTION_COUNT - 1; i++)
        buttonLinkVertical(mOptions[i], mOptions[i + 1]);
    buttonLinkVertical(mOptions[MAIN_MENU_OPTION_COUNT - 1], mOptions[0]);

    mCurButton = mOptions[sLastOption];

    mTitle = MOD_VERSION;
}

void MainMenu::recordLastOption()
{
    for (int i = 0; i < MAIN_MENU_OPTION_COUNT; i++)
    {
        if (mOptions[i] == mCurButton)
        {
            sLastOption = i;
            return;
        }
    }
    sLastOption = 0;
}

MainMenu::~MainMenu()
{
    recordLastOption();
}

}
