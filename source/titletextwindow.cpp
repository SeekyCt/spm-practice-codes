#include "mod_ui_base/colours.h"
#include "mod.h"
#include "titletextwindow.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>

namespace mod {

TitleTextWindow * TitleTextWindow::sInstance = nullptr;

TitleTextWindow::TitleTextWindow()
{
    // Set text scale, pos and colour
    mMsg = MOD_VERSION;
    mScale = 0.8f;
    mPosX = -(spm::fontmgr::FontGetMessageWidth(mMsg) * mScale) / 2;
    mPosY = gIs4_3 ? 260.0f : 200.0f;
    mColour = colours::green;
}

void TitleTextWindow::disp()
{
    // Only draw on title screen
    if (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_TITLE)
        return;

    // Draw string
    drawString(mMsg, mPosX, mPosY, &mColour, mScale, true);
}

void TitleTextWindow::init()
{
    sInstance = new TitleTextWindow();
}

}
