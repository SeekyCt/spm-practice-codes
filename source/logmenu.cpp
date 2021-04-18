#include "logmenu.h"
#include "mainmenu.h"
#include "nandsettings.h"
#include "scriptlog.h"

#include <types.h>
#include <spm/fontmgr.h>

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
    switch (settings->logOptions[i])
    {
        case LogType::NONE:
            settings->logOptions[i] = LogType::OSREPORT;
            break;
        case LogType::OSREPORT:
            settings->logOptions[i] = LogType::SCREEN;
            break;
        case LogType::SCREEN:
            settings->logOptions[i] = LogType::NONE;
            break;
    }
    instance->mOptions[i].value->mMsg = getValueName(settings->logOptions[i]);

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
        mOptions[i].value = new MenuButton(this, getValueName(settings->logOptions[i]), valueX, y, optionCycle, this, false, 0.8f);
        y -= (FONT_HEIGHT * 0.8f) + 5;
        if (i != 0)
            buttonLinkVertical(mOptions[i-1].value, mOptions[i].value);
    }

    // Set starting button and title
    mCurButton = mOptions[0].value;
    mTitle = "Logging Options";
}

void LogMenu::close()
{
    // Change back to parent menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

}
