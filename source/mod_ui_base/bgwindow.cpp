#include <common.h>
#include <spm/windowdrv.h>
#include <wii/mtx.h>

#include "mod_ui_base/bgwindow.h"

namespace mod {

BgWindow::BgWindow()
{
    // Init properties - these should be changed by inheriting classes
    mPosX = 0.0f;
    mPosY = 0.0f;
    mWidth = 0.0f;
    mHeight = 0.0f;
    mCurve = 0.0f;
    mColour = {0, 0, 0, 0};
}

void BgWindow::disp()
{
    drawBox(0, &mColour, mPosX, mPosY, mWidth, mHeight, mCurve);
}

}
