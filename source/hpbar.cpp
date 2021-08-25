#include "mod_ui_base/menuwindow.h"
#include "nandsettings.h"
#include "hpwindow.h"
#include "patch.h"
#include "util.h"

#include <types.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/evt_door.h>
#include <spm/mario.h>
#include <spm/npcdrv.h>
#include <spm/seqdrv.h>
#include <wii/mtx.h>
#include <wii/stdio.h>
#include <wii/string.h>
#include <wii/types.h>
#include <ogc/gx.h>

namespace mod {

#define BAR_WIDTH 50.0f
#define BAR_HEIGHT 12.5f
#define OUTLINE_SIZE 2.5f
#define INNER_WIDTH (BAR_WIDTH - (2 * OUTLINE_SIZE))
#define INNER_HEIGHT (BAR_HEIGHT - (2 * OUTLINE_SIZE))

static const wii::RGBA black {0x00, 0x00, 0x00, 0xff};
static const wii::RGBA red {0xff, 0x00, 0x00, 0xff};
static const wii::RGBA yellow {0xff, 0xff, 0x00, 0xff};

HPWindow * HPWindow::sInstance = nullptr;

HPWindow::HPWindow()
{
    mCamera = spm::camdrv::CAM_2D;
}

/*
    If a door has been entered in this map, checks that the NPC is shown by it
*/
static bool npcDoorCheck(const char * name)
{
    // Get the door desc for the current room
    spm::evt_door::DoorDesc * door = spm::evt_door::evtDoorGetActiveDoorDesc();

    // Never display while a door is open
    if (spm::evt_door::evtDoorWp->flags & EVT_DOOR_FLAG_DOOR_OPEN)
        return false;

    // Always display if not in a room
    if (door == nullptr)
        return true;    

    // Never display if the room has no NPCs
    if (door->npcNameList == nullptr)
        return false;
    
    // Display if belonging to room
    for (const char ** pName = door->npcNameList; *pName; pName++)
    {
        if (wii::string::strcmp(name, *pName) == 0)
            return true;
    }

    // Don't display if not in room
    return false;
}

void HPWindow::disp()
{
    // Don't draw over menu, if disabled or if not in game
    // TODO: make own setting
    if (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME || !gSettings->hudXYZ
        || MenuWindow::sCurMenu != nullptr)
        return;

    spm::npcdrv::NPCWork * wp = spm::npcdrv::npcGetWorkPtr();
    spm::npcdrv::NPCEntry * npc = wp->entries;
    for (s32 i = 0; i < wp->num; i++, npc++)
    {
        // Check NPC is visible
        // TODO: check Z to prevent issues with enemies very far behind &
        //       attempt to hide enemies blocked by objects
        if ((npc->flags_8 & 1) && (npc->flags_8 & 0x40000000) == 0 && npcDoorCheck(npc->name)
            && (npc->flag46C & 0x20000) == 0 && (npc->flags_c & 0x20) == 0)
        {
            // Get screen position
            wii::Vec3 pos;
            spm::camdrv::getScreenPoint(&npc->position, &pos);

            // Adjust x for the left of the bar
            pos.x -= BAR_WIDTH / 2.0f;

            // Draw the black outline
            Window::drawBoxGX(&black, pos.x, pos.y, BAR_WIDTH, BAR_HEIGHT);

            // Adjust coordinates for inside the outline
            pos.x += OUTLINE_SIZE;
            pos.y -= OUTLINE_SIZE;

            // Draw the red background
            Window::drawBoxGX(&red, pos.x, pos.y, INNER_WIDTH, INNER_HEIGHT);

            // Draw yellow hp bar
            float overlayWidth = INNER_WIDTH * ((float)npc->hp / (float)npc->maxHp);
            if (overlayWidth > 0.0f)
                Window::drawBoxGX(&yellow, pos.x, pos.y, overlayWidth, INNER_HEIGHT);

            // Draw hp
            char str[16];
            wii::stdio::sprintf(str, "%d", npc->hp);
            Window::drawString(str, pos.x + 50.0f, pos.y, &yellow, 1.0f, true);
        }
    }
}

}
