#include "statmenu.h"
#include "mainmenu.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/mario_pouch.h>
#include <spm/wpadmgr.h>
#include <wii/OSError.h>
#include <wii/string.h>
#include <wii/stdio.h>
#include <wii/wpad.h>

namespace mod {

enum {
    STAT_LEVEL = 0,
    STAT_XP = 1,
    STAT_ATTACK = 2,
    STAT_MAX_HP = 3,
    STAT_HP = 4,
    STAT_COINS = 5,
    STAT_ARCADE = 6
};

struct StatDef
{
    const char * name;
    int max; // inclusive
};

static StatDef stats[STAT_COUNT] = {
    {"Level", 99},
    {"XP", 99999999},
    {"Attack", 999},
    {"Max HP", 999},
    {"HP", 999},
    {"Coins", 999},
    {"Arcade Tokens", 9999},    
};

int StatMenu::getValue(int stat)
{
    spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
    switch (stat)
    {
        case STAT_LEVEL:
            return pp->level;
        case STAT_XP:
            return pp->xp;
        case STAT_ATTACK:
            return pp->attack;
        case STAT_MAX_HP:
            return pp->maxHp;
        case STAT_HP:
            return pp->hp;
        case STAT_COINS:
            return pp->coins;
        case STAT_ARCADE:
            return pp->arcadeTokens;
        default:
            return 0;
    }
}

void StatMenu::setValue(int stat, int value)
{
    spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
    switch (stat)
    {
        case STAT_LEVEL:
            pp->level = value;
            break;
        case STAT_XP:
            pp->xp = value;
            break;
        case STAT_ATTACK:
            pp->attack = value;
            break;
        case STAT_MAX_HP:
            pp->maxHp = value;
            break;
        case STAT_HP:
            pp->hp = value;
            break;
        case STAT_COINS:
            pp->coins = value;
            break;
        case STAT_ARCADE:
            pp->arcadeTokens = value;
            break;
        default:
            break;
    }
}

void StatMenu::updateStatDisps()
{
    for (int i = 0; i < STAT_COUNT; i++)
        wii::stdio::sprintf(mStatStrs[i], "%d", getValue(i));
}

void StatMenu::valueUp(MenuScrollerH * scroller, void * param)
{
    StatMenu * instance = reinterpret_cast<StatMenu *>(param);
    int i;
    for (i = 0; i < STAT_COUNT; i++)
        if (scroller == instance->mStats[i].value)
            break;
    
    int value = instance->getValue(i) + instance->mIncrement;
    if (value > stats[i].max)
        value = stats[i].max;
    instance->setValue(i, value);
    instance->updateStatDisps();
}

void StatMenu::valueDown(MenuScrollerH * scroller, void * param)
{
    StatMenu * instance = reinterpret_cast<StatMenu *>(param);
    int i;
    for (i = 0; i < STAT_COUNT; i++)
        if (scroller == instance->mStats[i].value)
            break;
    
    int value = instance->getValue(i) - instance->mIncrement;
    if (value < 0)
        value = 0;
    instance->setValue(i, value);
    instance->updateStatDisps();
}

void StatMenu::disp()
{
    MenuWindow::disp();

    if (mShouldClose)
    {
        // Change back to parent menu
        delete MenuWindow::sCurMenu;
        MenuWindow::sCurMenu = new MainMenu();
        return;
    }

    u32 btn = spm::wpadmgr::wpadGetButtonsPressed(0);
    if (btn & WPAD_BTN_PLUS)
    {
        if (mIncrement < 10000000)
            mIncrement *= 10;
    }
    if (btn & WPAD_BTN_MINUS)
    {
        if (mIncrement > 1)
            mIncrement /= 10;
    }
    wii::stdio::sprintf(mIncrementStr, "Increment (<icon PAD_MINUS 0.8 0 35 0> <icon PAD_PLUS 0.8 0 35 0><col ffffffff>): %d", mIncrement);
    drawMessage(mIncrementStr, -330, -80);
}

StatMenu::StatMenu()
{
    mIncrement = 1;
    mShouldClose = false;

    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;
    const f32 scale = 0.8f;
    const f32 width = 65.0f;

    f32 y = 170;
    updateStatDisps();
    for (int i = 0; i < STAT_COUNT; i++)
    {
        mStats[i].label = new MenuButton(this, stats[i].name, labelX, y, nullptr, nullptr, false, scale);
        mStats[i].value = new MenuScrollerH(this, mStatStrs[i], valueX, y, width, valueDown, valueUp, this, nullptr, nullptr, scale);
        y -= (FONT_HEIGHT * scale) + 5;
        if (i != 0)
            buttonLinkVertical(mStats[i-1].value, mStats[i].value);
    }

    // Set starting button and title
    mCurButton = mStats[0].value;
    mTitle = "Stat Editor";
}

void StatMenu::close()
{
    mShouldClose = true;
}

}
