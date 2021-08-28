#include "mod_ui_base/colours.h"
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

HPWindow * HPWindow::sInstance = nullptr;

HPWindow::HPWindow()
{
    mCamera = spm::camdrv::CAM_2D;
}

const s32 HPWindow::bossTribes[] = {
    270, 271, 272, 273, // o'chunks
    280, 282, 284, // mimi (TODO: disable chase version)
    286, 287, 289, 290, 292, 293, // dimentio
    295, // mr l
    296, 300, // brobot
    304, // nastasia
    305, 308, // bleck
    313, 314, // fracktail & wracktail
    315, 316, // bowser
    317, // big blooper
    318, // francis
    319, 320, 321, 322, // king croacus
    327, // bonechill
    330, 331, 332, 333, // shadoo

    // sammer guys
    338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357,
    358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377,
    378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397,
    398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417,
    418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437,
};

const s32 HPWindow::blacklistedTribes[] = {
    17, // mega koopa                                                                                           
    40, 42, 44, // dry/dull bones bone
    46, 48, 50, // hammer bro hammer
    52, 54, 56, // boomerang bro boomerang
    58, 60, 62, // fire bro fireball
    65, 68, 475, 478, 481, // magikoopa magic
    70, 72, 74, 76, 463, // koopa striker shell
    79, 81, // piranha breath
    103, 105, 107, // pokey projectile
    110, 112, 494, // ruff puff projectile
    116, 118, 120, // dayzee musical notes
    127, 129, 131, 133, // squig projectile
    143, 145, 147, 149, 505, // boomboxer projectiles
    151, 153, 155, // mr i projectile
    172, // bigmeow (TODO: remove?)
    180, // foton projectile
    195, 197, 199, // barribad projectile
    206, 208, 210, // floro sapien head
    212, 213, 215, 216, 218, 219, 221, 222, // ninjoe projectile & bomb
    231, // skellobit head
    236, // skellobait breath
    238, 240, 242, 244, // magiblot projectile
    260, 261, // howl/growl (TODO: remove?)
    262, 263, 264, // underhand
    265, 266, 267, // thwomp & spiny/spiky tromp
    274, 275, 276, 277, 278, 279, // o'chunks block
    281, 283, 285, // mimi rubee
    288, 291, 294, // dimentio magic
    297, 298, 299, 301, 302, 303, 511, 512, 513, 514, // brobot projectile
    306, 307, 309, 310, 311, // bleck projectile
    323, // king croacus projectile
    328, 329, // bonechill ice
    334, 335, // shell shock
    336, 337, // trap gold bar
    438, 439, // sammer projectile
    441, 443, 445, 447, 449, // cherbil projectile
    452, // frackle projectile
    489, 491, // pider/arantula projectile
    518, // dry bones head
};

bool HPWindow::npcBossTribeCheck(spm::npcdrv::NPCEntry * npc)
{
    for (u32 i = 0; i < ARRAY_SIZEOF(bossTribes); i++)
    {
        if (bossTribes[i] == npc->tribeId)
            return true;
    }
    return false;
}

bool HPWindow::npcEnemyTribeCheck(spm::npcdrv::NPCEntry * npc)
{
    if (npc->tribeId == 0 && wii::string::strcmp(npc->m_Anim.animPoseName, "e_kuribo") != 0)
        return false;
    
    if (npcBossTribeCheck(npc))
        return false;
    
    for (u32 i = 0; i < ARRAY_SIZEOF(blacklistedTribes); i++)
    {
        if (blacklistedTribes[i] == npc->tribeId)
            return false;
    }

    return true;
}

bool HPWindow::npcDoorCheck(const char * name)
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

bool HPWindow::npcPosCheck(spm::npcdrv::NPCEntry * npc)
{
    // Position doesn't matter in 2d because of panes
    if (!check3d())
        return true;

    spm::camdrv::CamEntry * cam = spm::camdrv::camGetPtr(spm::camdrv::CAM_3D);
    return cam->pos.x < npc->position.x;
}

void HPWindow::bossDisp()
{
    
}

void HPWindow::enemyDisp()
{
    spm::npcdrv::NPCWork * wp = spm::npcdrv::npcGetWorkPtr();
    spm::npcdrv::NPCEntry * npc = wp->entries;
    for (s32 i = 0; i < wp->num; i++, npc++)
    {
        // Check NPC is visible
        if ((npc->flags_8 & 1) && (npc->flags_8 & 0x40000000) == 0
            && (npc->flags_c & 0x20) == 0 && (npc->flag46C & 0x20000) == 0 
            && npcEnemyTribeCheck(npc) && npcPosCheck(npc) && npcDoorCheck(npc->name))
        {
            // Get screen position
            wii::Vec3 pos;
            spm::camdrv::getScreenPoint(&npc->position, &pos);

            // Adjust x for the left of the bar
            pos.x -= BAR_WIDTH / 2.0f;

            // Draw the black outline
            Window::drawBoxGX(&colours::black, pos.x, pos.y, BAR_WIDTH, BAR_HEIGHT);

            // Adjust coordinates for inside the outline
            pos.x += OUTLINE_SIZE;
            pos.y -= OUTLINE_SIZE;

            // Draw the red background
            Window::drawBoxGX(&colours::red, pos.x, pos.y, INNER_WIDTH, INNER_HEIGHT);

            // Draw yellow hp bar
            float overlayWidth = INNER_WIDTH * ((float)npc->hp / (float)npc->maxHp);
            if (overlayWidth > 0.0f)
                Window::drawBoxGX(&colours::yellow, pos.x, pos.y, overlayWidth, INNER_HEIGHT);

            // Draw hp
            char str[16];
            wii::stdio::sprintf(str, "%d", npc->hp);
            Window::drawString(str, pos.x + 50.0f, pos.y, &colours::yellow, 1.0f, true);
        }
    }
}

void HPWindow::disp()
{
    // Don't draw over menu or if not in game
    if (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME || MenuWindow::sCurMenu != nullptr)
        return;

    // Draw enemy hp bars if enabled
    if (gSettings->enemyHpBars && (!gSettings->hpBarHide3d || !check3d()))
        enemyDisp();

    // Draw boss hp bar if enabled
    if (gSettings->bossHpBars)
        bossDisp();
}

void HPWindow::init()
{
    sInstance = new HPWindow();
}

}
