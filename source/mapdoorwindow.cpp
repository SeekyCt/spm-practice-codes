#include "mod_ui_base/menuwindow.h"
#include "mapdoorwindow.h"
#include "nandsettings.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <wii/stdio.h>
#include <wii/types.h>

namespace mod {

MapDoorWindow * MapDoorWindow::sInstance = nullptr;

MapDoorWindow::MapDoorWindow()
{
    // Set text pos, scale and colour
    mPosX = -365.0f;
    mPosY = 128.0f;
    mScale = 0.8f;
    mColour = {0, 0xff, 0, 0xff};
}

void MapDoorWindow::disp()
{
	// Don't draw over menu, if disabled or if not in game
	if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME) || !settings->hudMapDoor)
		return;

    // Create strings
	wii::stdio::sprintf(mMapMsg, "map: %s", spm::spmario::gp->mapName);
	wii::stdio::sprintf(mDoorMsg, "door: %s", spm::spmario::gp->doorName);

    // Draw strings
    drawString(mMapMsg, mPosX, mPosY, &mColour, mScale, true);
    drawString(mDoorMsg, mPosX, mPosY - (FONT_HEIGHT * mScale), &mColour, mScale, true);
}

}
