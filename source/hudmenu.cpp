#include <common.h>
#include <msl/stdio.h>

#include "hudmenu.h"
#include "nandsettings.h"
#include "util.h"

namespace mod {

void HudMenu::updateDelayStr()
{
    msl::stdio::sprintf(mDelayStr, "%d", gSettings->xyzInterval);
}

void HudMenu::updateDecimalPlaceStr()
{
    msl::stdio::sprintf(mDecimalPlaceStr, "%d", gSettings->xyzDP);
}

void HudMenu::optionToggle(MenuButton * button, bool &setting)
{
    setting = !setting;
    button->mMsg = getToggleName(setting);
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
    options[n++] = new MenuButton(this, getToggleName(setting), valueX, y,    \
        [](MenuButton * button, void * param)                                 \
        {                                                                     \
            HudMenu * instance = reinterpret_cast<HudMenu *>(param);          \
            instance->optionToggle(button, setting);                          \
            return true;                                                      \
        },                                                                    \
        this, false, scale);                                                  \
    y -= yDiff

#define SETTING_COUNT 8

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
    int n = 0;

    OPTION("Map & Door Name Display", gSettings->hudMapDoor);
    OPTION("Position Display", gSettings->hudXYZ);

    new MenuButton(this, "Position Update Delay", labelX, y, nullptr, nullptr, false, scale);
    updateDelayStr();
    options[n++] = new MenuScrollerH(this, mDelayStr, scrollerX, y, arrowXDiff,
        [](MenuScrollerH * scroller, void * param)
        {
            (void) scroller;

            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(gSettings->xyzInterval, -1);
            instance->updateDelayStr();
        },
        [](MenuScrollerH * scroller, void * param)
        {
            (void) scroller;

            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(gSettings->xyzInterval, 1);
            instance->updateDelayStr();
        },
        this, nullptr, nullptr, scale
    );
    y -= yDiff;

    new MenuButton(this, "Position Decimal Places", labelX, y, nullptr, nullptr, false, scale);
    updateDecimalPlaceStr();
    options[n++] = new MenuScrollerH(this, mDecimalPlaceStr, scrollerX, y, arrowXDiff,
        [](MenuScrollerH * scroller, void * param)
        {
            (void) scroller;

            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(gSettings->xyzDP, -1);
            instance->updateDecimalPlaceStr();
        },
        [](MenuScrollerH * scroller, void * param)
        {
            (void) scroller;

            HudMenu * instance = reinterpret_cast<HudMenu *>(param);
            instance->optionDelta(gSettings->xyzDP, 1);
            instance->updateDecimalPlaceStr();
        },
        this, nullptr, nullptr, scale
    );
    y -= yDiff;

    OPTION("Display Enemy HP Bars", gSettings->enemyHpBars);
    OPTION("Hide Enemy HP Bars in 3D", gSettings->hpBarHide3d);
    OPTION("Display Boss HP Bars", gSettings->bossHpBars);
    OPTION("Display Controller Inputs", gSettings->inputDisplay);

    for (int i = 1; i < SETTING_COUNT; i++)
        buttonLinkVertical(options[i - 1], options[i]);

    // Set starting button and title
    mCurButton = options[0];
    mTitle = "HUD Display Options";
}

}
