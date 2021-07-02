#include "miscmenu.h"
#include "mainmenu.h"
#include "nandsettings.h"
#include "util.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <wii/stdio.h>

namespace mod {

void MiscMenu::optionToggle(MenuButton * button, bool &setting)
{
    setting = !setting;
    button->mMsg = getToggleName(setting);
}

#define OPTION(message, setting) \
    new MenuButton(this, message, labelX, y, nullptr, nullptr, false, scale); \
    options[n++] = new MenuButton(this, getToggleName(setting), valueX, y,    \
        [](MenuButton * button, void * param)                                 \
        {                                                                     \
            MiscMenu * instance = reinterpret_cast<MiscMenu *>(param);        \
            instance->optionToggle(button, setting);                          \
            return true;                                                      \
        },                                                                    \
        this, false, scale);                                                  \
    y -= yDiff

#define SETTING_COUNT 1

MiscMenu::MiscMenu()
{
    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;
    const f32 scale = 0.8f;
    const f32 yDiff = (FONT_HEIGHT * scale) + 5.0f;

    MenuButton * options[SETTING_COUNT];
    f32 y = 170;
    int n = 0;

    OPTION("Load Pit Text From File", gSettings->customPitText);

    for (int i = 1; i < SETTING_COUNT; i++)
        buttonLinkVertical(options[i - 1], options[i]);

    // Set starting button and title
    mCurButton = options[0];
    mTitle = "Miscellaneous Options";
}

void MiscMenu::close()
{
    // Change back to parent menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

}
