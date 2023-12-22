#include <common.h>

#include "logmenu.h"
#include "nandsettings.h"
#include "scriptlog.h"

namespace mod {

static const char * optionNames[] = {
    "Evt Script Tracking",
    "Evt Variable Tracking",
    "Evt Debug Instructions"
};

static const char * getValueName(LogType type)
{
    switch (type)
    {
        case LogType::NONE:
            return "Disabled";
        case LogType::OSREPORT:
            return "OSReport";
        case LogType::SCREEN:
            return "Screen";
        default:
            return nullptr;
    }
}

bool LogMenu::optionCycle(MenuButton * button, void * param)
{
    LogMenu * instance = reinterpret_cast<LogMenu *>(param);

    int i;
    for (i = 0; i < LOG_OPTION_COUNT; i++)
    {
        if (instance->mOptions[i].value == button)
            break;
    }

    instance->mCurButton = instance->mOptions[i].value;
    switch (gSettings->logOptions[i])
    {
        case LogType::NONE:
            gSettings->logOptions[i] = LogType::OSREPORT;
            break;
        case LogType::OSREPORT:
            gSettings->logOptions[i] = LogType::SCREEN;
            break;
        case LogType::SCREEN:
            gSettings->logOptions[i] = LogType::NONE;
            break;
    }
    instance->mOptions[i].value->mMsg = getValueName(gSettings->logOptions[i]);

    return true;
}

LogMenu::LogMenu()
{
    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;

    f32 y = 170;
    for (int i = 0; i < LOG_OPTION_COUNT; i++)
    {
        mOptions[i].label = new MenuButton(this, optionNames[i], labelX, y, nullptr, nullptr, false, 0.8f);
        mOptions[i].value = new MenuButton(this, getValueName(gSettings->logOptions[i]), valueX, y, optionCycle, this, false, 0.8f);
        y -= (FONT_HEIGHT * 0.8f) + 5;
        if (i != 0)
            buttonLinkVertical(mOptions[i-1].value, mOptions[i].value);
    }

    // Set starting button and title
    mCurButton = mOptions[0].value;
    mTitle = "Logging Options";
}

}
