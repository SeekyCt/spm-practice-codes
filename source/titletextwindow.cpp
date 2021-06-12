#include "mod.h"
#include "titletextwindow.h"

#include <types.h>
#include <spm/fontmgr.h>

namespace mod {

TitleTextWindow * TitleTextWindow::sInstance = nullptr;

TitleTextWindow::TitleTextWindow()
{
    // Set text scale, pos and colour
    mMsg = MOD_VERSION;
    mScale = 0.8f;
    mPosX = -(spm::fontmgr::FontGetMessageWidth(mMsg) * mScale) / 2;
    mPosY = gIs4_3 ? 260.0f : 200.0f;
    mColour = {0, 0xff, 0, 0xff};
}

void TitleTextWindow::disp()
{
    // Draw string
    drawString(mMsg, mPosX, mPosY, &mColour, mScale, true);
}

}
