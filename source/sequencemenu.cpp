#include "sequencemenu.h"
#include "scriptlog.h"
#include "util.h"

#include <types.h>
#include <spm/evtmgr_cmd.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <wii/stdio.h>

namespace mod {

#define SEQUENCE_MAX 424

const char * SequenceMenu::getLevel()
{
    s32 seq = getVal();
    switch (seq)
    {
        case  11 ...  17: return "1-1";
        case  18 ...  28: return "1-2";
        case  29 ...  37: return "1-3";
        case  38 ...  65: return "1-4";
        case  66 ...  76: return "2-1";
        case  77 ...  82: return "2-2";
        case  83 ...  88: return "2-3";
        case  89 ... 100: return "2-4";
        case 101 ... 105: return "3-1";
        case 106 ... 111: return "3-2";
        case 112 ... 117: return "3-3";
        case 118 ... 128: return "3-4";
        case 129 ... 138: return "4-1";
        case 139 ... 149: return "4-2";
        case 150 ... 160: return "4-3";
        case 161 ... 178: return "4-4";
        case 179 ... 186: return "5-1";
        case 187 ... 194: return "5-2";
        case 195 ... 202: return "5-3";
        case 203 ... 224: return "5-4";
        case 225 ... 271: return "6-1";
        case 272 ... 289: return "6-2";
        case 290 ... 290: return "6-3";
        case 291 ... 291: return "6-4";
        case 292 ... 306: return "7-1";
        case 307 ... 327: return "7-2";
        case 328 ... 339: return "7-4";
        case 340 ... 358: return "7-4";
        case 359 ... 370: return "8-1";
        case 371 ... 386: return "8-2";
        case 387 ... 404: return "8-3";
        case 405 ... 420: return "8-4";
        default:
            if (seq < 11)
                return "Prologue";
            else
                return "Endgame";
    }
}

s32 SequenceMenu::getVal()
{
    return spm::evtmgr_cmd::evtGetValue(nullptr, GSW(0));
}

void SequenceMenu::setVal(s32 val)
{
    spm::evtmgr_cmd::evtSetValue(nullptr, GSW(0), val);
}

void SequenceMenu::valChange(MenuScrollGroup * scroller, s32 delta, void * param)
{
    (void) scroller;

    SequenceMenu * instance = reinterpret_cast<SequenceMenu *>(param);

    // Set value and enforce limits
    s32 val = instance->getVal();
    if ((val + delta) > SEQUENCE_MAX)
        val = SEQUENCE_MAX;
    else if ((val + delta) < 0)
        val = 0;
    else
        val = val + delta;
    instance->setVal(val);

    // Update display
    instance->mScroller->mDispValue = val;
    instance->mLevelDisp->mMsg = instance->getLevel();
}

void SequenceMenu::close()
{
    // Re-enable script variable logging
    scriptLogOnOff(true);

    // Close as normal
    MenuWindow::close();
}

void SequenceMenu::fullClose()
{
    // Re-enable script variable logging
    scriptLogOnOff(true);

    // Close as normal
    MenuWindow::fullClose();
}

SequenceMenu::SequenceMenu()
{
    // Position constants
    const f32 editX = FONT_WIDTH * -1.5f;
    const f32 editY = 20.0f;
    const f32 levelY = -100.0f;

    // Init buttons
    mLevelDisp = new CentredButton(this, getLevel(), levelY);
    mScroller = new MenuScrollGroup(this, getVal(), editX, editY, valChange, this, 3, false);   

    // Disable script variable logging while open
    scriptLogOnOff(false);

    // Set starting button and title
    mCurButton = mScroller;
    mTitle = "Sequence Position Editor";
}

}
