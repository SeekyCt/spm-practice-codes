#include "mod_ui_base/colours.h"
#include "miscmenu.h"
#include "mod.h"
#include "nandsettings.h"
#include "util.h"

#include <types.h>
#include <wii/stdio.h>

namespace mod {

void MiscMenu::optionToggle(MenuButton * button, bool &setting)
{
    setting = !setting;
    button->mMsg = getToggleName(setting);
}

#define OPTION(message, setting) \
    new MenuButton(this, message, labelX, y, nullptr, nullptr, false, scale); \
    mOptions[n++] = new MenuButton(this, getToggleName(setting), valueX, y,   \
        [](MenuButton * button, void * param)                                 \
        {                                                                     \
            MiscMenu * instance = reinterpret_cast<MiscMenu *>(param);        \
            instance->optionToggle(button, setting);                          \
            return true;                                                      \
        },                                                                    \
        this, false, scale);                                                  \
    y -= yDiff

enum OptionIdxs
{
    OPTION_PIT_TEXT = 0,
    OPTION_LAST_KEY = 1
};

void MiscMenu::disp()
{
    // Run main display function
    MenuWindow::disp();
    
    // Display warning if selected
    if (mCurButton == mOptions[OPTION_PIT_TEXT] && !gIsRiivolution && !gIsDolphin && !gIsPatchedDisc)
    {
        drawString("Note: custom pit text is not supported without Riivolution",
                   -320.0f, -100.0f, &colours::red, 0.8f);
        drawString("or a patched ISO", -320.0f, -100.0f - FONT_HEIGHT, &colours::red, 0.8f);
    }
}

MiscMenu::MiscMenu()
{
    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;
    const f32 scale = 0.8f;
    const f32 yDiff = (FONT_HEIGHT * scale) + 5.0f;

    f32 y = 170;
    int n = 0;

    OPTION("Load Pit Text From File", gSettings->customPitText);
    OPTION("Force Last Key in Pit", gSettings->lastKey);

    for (int i = 1; i < MISC_SETTING_COUNT; i++)
        buttonLinkVertical(mOptions[i - 1], mOptions[i]);

    // Set starting button and title
    mCurButton = mOptions[0];
    mTitle = "Miscellaneous Options";
}

}
