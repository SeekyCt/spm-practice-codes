#include <common.h>
#include <msl/stdio.h>

#include "mod_ui_base/colours.h"
#include "miscmenu.h"
#include "mod.h"
#include "nandsettings.h"
#include "util.h"

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

#define NOTE_X -320.0f
#define NOTE_Y -100.0f
#define NOTE_SCALE 0.8f

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
    if (mCurButton == mOptions[OPTION_PIT_TEXT])
    {
        if (!gIsRiivolution && !gIsDolphin && !gIsPatchedDisc)
        {
            drawString("Note: custom pit text is not supported without Riivolution",
                    NOTE_X, NOTE_Y, &colours::red, 0.8f);
            drawString("or a patched ISO", NOTE_X, NOTE_Y - FONT_HEIGHT, &colours::red, NOTE_SCALE);
        }
    }
    else if (mCurButton == mOptions[OPTION_LAST_KEY])
    {
        if (isPitEnemyRoom())
        {
            drawString("Note: last key setting only updates on room reload",
                    NOTE_X, NOTE_Y, &colours::red, NOTE_SCALE);
        }
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
