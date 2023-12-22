#include <common.h>
#include <spm/mario.h>
#include <spm/seqdrv.h>
#include <msl/stdio.h>
#include <msl/string.h>

#include "mod_ui_base/colours.h"
#include "mod_ui_base/menuwindow.h"
#include "nandsettings.h"
#include "xyzwindow.h"

namespace mod {

XYZWindow * XYZWindow::sInstance = nullptr;

XYZWindow::XYZWindow()
{
    // Set text pos, scale and colour
    mPosX = -365.0f;
    mPosY = 64.0f;
    mScale = 0.8f;
    mColour = colours::green;
    mFrameCount = 0;
}

void XYZWindow::disp()
{
    // Don't draw over menu, if disabled or if not in game
    if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME) || !gSettings->hudXYZ)
        return;

    // Update if interval has passed / is disabled
    if (++mFrameCount > gSettings->xyzInterval)
    {
        // Reset
        mFrameCount = 0;

        // Generate format for desired number of decimal places
        char fmt[9];
        msl::stdio::sprintf(fmt, "%%c: %%.%df", gSettings->xyzDP);

        // Create strings
        spm::mario::MarioWork * mp = spm::mario::marioGetPtr();
        f32 * pos = reinterpret_cast<f32 *>(&mp->position);
        for (int i = 0; i < 3; i++)
        {
            msl::stdio::sprintf(mMsgs[i], fmt, 'x' + i, pos[i]);

            // Don't display negative zero since it'll often flicker between signs
            if (mMsgs[i][3] == '-')
            {
                bool negativeZero = true;
                int len = msl::string::strlen(mMsgs[i]);
                for (int j = 4; j < len; j++)
                {
                    char c = mMsgs[i][j];
                    if ((c != '0') && (c != '.'))
                    {
                        if (c != 0)
                            negativeZero = false;
                        break;
                    }
                }
                if (negativeZero)
                    msl::string::memmove(mMsgs[i] + 3, mMsgs[i] + 4, len - 4 + 1);
            }
        }
    }

    // Draw strings
    for (int i = 0; i < 3; i++)
        drawString(mMsgs[i], mPosX, mPosY - ((FONT_HEIGHT * mScale) * i), &mColour, mScale, true);
}

void XYZWindow::init()
{
    sInstance = new XYZWindow();
}

}
