#include "hudmenu.h"
#include "mainmenu.h"
#include "nandsettings.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <wii/OSError.h>
#include <wii/string.h>
#include <wii/stdio.h>

namespace mod {

static const char * optionNames[] = {
    "Map & Door Name Display",
    "Position Display"
};

static const char * getValueName(bool state)
{
    switch (state)
    {
        case true:
            return "Enabled";
        case false:
            return "Disabled";
    }
}

bool HudMenu::optionToggle(MenuButton * button, void * param)
{
    HudMenu * instance = reinterpret_cast<HudMenu *>(param);

    int i;
    for (i = 0; i < HUD_OPTION_COUNT; i++)
    {
        if (instance->mOptions[i].value == button)
            break;
    }

    settings->hudOptions[i] = !settings->hudOptions[i];
    instance->mOptions[i].value->mMsg = getValueName(settings->hudOptions[i]);

    return true;
}

HudMenu::HudMenu()
{
    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;

    f32 y = 170;
    for (int i = 0; i < HUD_OPTION_COUNT; i++)
    {
        mOptions[i].label = new MenuButton(this, optionNames[i], labelX, y, nullptr, nullptr, false, 0.8f);
        mOptions[i].value = new MenuButton(this, getValueName(settings->hudOptions[i]), valueX, y, optionToggle, this, false, 0.8f);
        y -= (FONT_HEIGHT * 0.8f) + 5;
        if (i != 0)
            buttonLinkVertical(mOptions[i-1].value, mOptions[i].value);
    }

    // Set starting button and title
    mCurButton = mOptions[0].value;
    mTitle = "HUD Display Options";
}

void HudMenu::close()
{
    // Change back to parent menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

}
