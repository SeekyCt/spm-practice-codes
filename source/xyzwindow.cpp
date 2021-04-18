#include "mod_ui_base/menuwindow.h"
#include "nandsettings.h"
#include "xyzwindow.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/mario.h>
#include <spm/seqdrv.h>
#include <wii/stdio.h>
#include <wii/string.h>

namespace mod {

XYZWindow * XYZWindow::sInstance = nullptr;

XYZWindow::XYZWindow()
{
    // Set text pos, scale and colour
    mPosX = -365.0f;
    mPosY = 64.0f;
    mScale = 0.8f;
    mColour = {0, 0xff, 0, 0xff};
    
    // Init update delay properties
    mFrameCount = 0;
    mFrameDelay = 3;
}

void XYZWindow::disp()
{
	// Don't draw over menu, if disabled or if not in game
	if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME)
      || !settings->hudOptions[OPTION_XYZ])
		return;

    // Update if delay has passed
    if (mFrameCount++ > mFrameDelay)
    {
        // Reset
        mFrameCount = 0;

        // Create strings
        spm::mario::MarioWork * mp = spm::mario::marioGetPtr();
        wii::stdio::sprintf(mMsgX, "x: %.2f", mp->position.x);
        wii::stdio::sprintf(mMsgY, "y: %.2f", mp->position.y);
        if (wii::string::strcmp(mMsgY + 3, "-0.00") == 0)
            wii::string::memmove(mMsgY + 3, mMsgY + 4, 5);
        wii::stdio::sprintf(mMsgZ, "z: %.2f", mp->position.z);
    }

    // Draw strings
    const f32 dist = FONT_HEIGHT * mScale;
    drawString(mMsgX, mPosX, mPosY, &mColour, mScale, true);
    drawString(mMsgY, mPosX, mPosY - dist, &mColour, mScale, true);
    drawString(mMsgZ, mPosX, mPosY - (dist * 2), &mColour, mScale, true);
}

}
