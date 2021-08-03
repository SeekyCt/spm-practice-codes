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
#include <wii/stdio.h>
#include <wii/string.h>
#include <wii/types.h>

namespace mod {

using spm::npcdrv::NPCWork;
using spm::npcdrv::NPCEntry;

static const wii::RGBA red {0xff, 0x00, 0x00, 0xff};
static const wii::RGBA yellow {0xff, 0xff, 0x00, 0xff};

static void disp(s8 cameraId, void * param)
{
    (void) cameraId;
    (void) param;

    // Don't draw over menu, if disabled or if not in game
    // TODO: make own setting
    if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME) || !gSettings->hudXYZ)
        return;

    NPCWork * wp = spm::npcdrv::npcGetWorkPtr();
    NPCEntry * npc = wp->entries;
    for (s32 i = 0; i < wp->num; i++, npc++)
    {
        if (npc->flags_8 & 1)
        {
            wii::Vec3 pos;
            spm::camdrv::getScreenPoint(&npc->position, &pos);
            pos.x -= 25.0f;
            Window::drawBox(0, &red, pos.x, pos.y, 50.0f, 10.0f, 0.0f);

            float overlayWidth = 50.0f * ((float)npc->hp / (float)npc->maxHp);
            if (overlayWidth > 0)
                 Window::drawBox(0, &yellow, pos.x, pos.y, overlayWidth, 10.0f, 0.0f);

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
