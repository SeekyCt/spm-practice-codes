#include "mod_ui_base/bgwindow.h"

#include <types.h>
#include <spm/windowdrv.h>
#include <wii/mtx.h>

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
    // Draw background - windowDispGX_Waku_col doesn't exist in this game,
    // so an identity matrix is in GX2 to create an equivalent
    wii::Mtx34 mtx;
    wii::mtx::PSMTXIdentity(&mtx);
    spm::windowdrv::windowDispGX2_Waku_col(&mtx, 0, &mColour, mPosX, mPosY,
                                           mWidth, mHeight, mCurve);
}

}
