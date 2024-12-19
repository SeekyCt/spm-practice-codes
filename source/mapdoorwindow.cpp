#include <common.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <stdio.h>

#include "mod_ui_base/colours.h"
#include "mod_ui_base/menuwindow.h"
#include "mapdoorwindow.h"
#include "nandsettings.h"

namespace mod {

MapDoorWindow * MapDoorWindow::sInstance = nullptr;

MapDoorWindow::MapDoorWindow()
{
    // Set text pos, scale and colour
    mPosX = -365.0f;
    mPosY = 120.0f;
    mScale = 0.8f;
    mColour = colours::green;
}

void MapDoorWindow::disp()
{
    // Don't draw over menu, if disabled or if not in game
    if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME) || !gSettings->hudMapDoor)
        return;

    // Create strings
    sprintf(mMapMsg, "map: %s", spm::spmario::gp->mapName);
    sprintf(mDoorMsg, "door: %s", spm::spmario::gp->doorName);

    // Draw strings
    drawString(mMapMsg, mPosX, mPosY, &mColour, mScale, true);
    drawString(mDoorMsg, mPosX, mPosY - (FONT_HEIGHT * mScale), &mColour, mScale, true);
}

void MapDoorWindow::init()
{
    sInstance = new MapDoorWindow();
}

}
