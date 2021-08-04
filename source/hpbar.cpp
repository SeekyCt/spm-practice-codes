#include "mod_ui_base/menuwindow.h"
#include "nandsettings.h"
#include "hpbar.h"
#include "patch.h"
#include "util.h"

#include <types.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/mario.h>
#include <spm/npcdrv.h>
#include <spm/seqdrv.h>
#include <wii/mtx.h>
#include <wii/stdio.h>
#include <wii/string.h>
#include <wii/types.h>
#include <ogc/gx.h>

namespace mod {

#define BAR_WIDTH (50.0f)
#define BAR_HEIGHT (12.5f)
#define OUTLINE_SIZE 2.5f
#define INNER_WIDTH (BAR_WIDTH - (2 * OUTLINE_SIZE))
#define INNER_HEIGHT (BAR_HEIGHT - (2 * OUTLINE_SIZE))

static const wii::RGBA black {0x00, 0x00, 0x00, 0xff};
static const wii::RGBA red {0xff, 0x00, 0x00, 0xff};
static const wii::RGBA yellow {0xff, 0xff, 0x00, 0xff};

static void disp(s8 cameraId, void * param)
{
    (void) cameraId;
    (void) param;

    // Don't draw over menu, if disabled or if not in game
    // TODO: make own setting
    if (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME || !gSettings->hudXYZ
        || MenuWindow::sCurMenu != nullptr)
        return;

    spm::npcdrv::NPCWork * wp = spm::npcdrv::npcGetWorkPtr();
    spm::npcdrv::NPCEntry * npc = wp->entries;
    for (s32 i = 0; i < wp->num; i++, npc++)
    {
        if (npc->flags_8 & 1)
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

void (*animMainReal)();
void hpBarPatch()
{
    animMainReal = patch::hookFunction(spm::animdrv::animMain,
        []()
        {
            animMainReal();
            spm::dispdrv::dispEntry(spm::camdrv::CAM_2D, 0, 908.0f, disp, nullptr);
        }
    );
}

}
