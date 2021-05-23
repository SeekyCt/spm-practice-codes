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

void HudMenu::updateDelayStr()
{
    wii::stdio::sprintf(mDelayStr, "%d", settings->xyzInterval);
}

void HudMenu::updateDecimalPlaceStr()
{
    wii::stdio::sprintf(mDecimalPlaceStr, "%d", settings->xyzDP);
}

void HudMenu::optionToggle(MenuButton * button, bool &setting)
{
    setting = !setting;
    button->mMsg = getValueName(setting);
}

void HudMenu::optionDelta(s8 &setting, int change)
{
    setting += change;
    if (setting > 9)
        setting = 9;
    if (setting < 0)
        setting = 0;
}

#define OPTION(message, setting) \
    new MenuButton(this, message, labelX, y, nullptr, nullptr, false, scale); \
    options[i++] = new MenuButton(this, getValueName(setting), valueX, y,     \
        [](MenuButton * button, void * param)                                 \
        {                                                                     \
            HudMenu * instance = reinterpret_cast<HudMenu *>(param);          \
            instance->optionToggle(button, setting);                          \
            return true;                                                      \
        },                                                                    \
        this, false, scale);                                                  \
    y -= yDiff

#define SETTING_COUNT 4

HudMenu::HudMenu()
{
    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;
    const f32 scrollerX = valueX + 40.0f;
    const f32 scale = 0.8f;
    const f32 yDiff = (FONT_HEIGHT * scale) + 5.0f;
    const f32 arrowXDiff = 40.0f; 

    MenuButton * options[SETTING_COUNT];
    f32 y = 170;
    int i = 0;

    OPTION("Map & Door Name Display", settings->hudMapDoor);
    OPTION("Position Display", settings->hudXYZ);

    new MenuButton(this, "Position Update Delay", labelX, y, nullptr, nullptr, false, scale);
    updateDelayStr();
    options[i++] = new MenuScrollerH(this, mDelayStr, scrollerX, y, arrowXDiff,
        [](MenuScrollerH * scroller, void * param)
        {
            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(settings->xyzInterval, -1);
            instance->updateDelayStr();
        },
        [](MenuScrollerH * scroller, void * param)
        {
            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(settings->xyzInterval, 1);
            instance->updateDelayStr();
        },
        this, nullptr, nullptr, scale
    );
    y -= yDiff;

    new MenuButton(this, "Position Decimal Places", labelX, y, nullptr, nullptr, false, scale);
    updateDecimalPlaceStr();
    options[i++] = new MenuScrollerH(this, mDecimalPlaceStr, scrollerX, y, arrowXDiff,
        [](MenuScrollerH * scroller, void * param)
        {
            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(settings->xyzDP, -1);
            instance->updateDecimalPlaceStr();
        },
        [](MenuScrollerH * scroller, void * param)
        {
            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(settings->xyzDP, 1);
            instance->updateDecimalPlaceStr();
        },
        this, nullptr, nullptr, scale
    );

    for (int i = 1; i < SETTING_COUNT; i++)
        buttonLinkVertical(options[i - 1], options[i]);

    // Set starting button and title
    mCurButton = options[0];
    mTitle = "HUD Display Options";
}

void HudMenu::close()
{
    // Change back to parent menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

}
