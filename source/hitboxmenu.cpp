#include "hitboxmenu.h"
#include "hitboxmenu.h"
#include "nandsettings.h"
#include "patch.h"
#include "util.h"

#include <types.h>
#include <spm/npcdrv.h>

namespace mod {

void HitboxMenu::optionToggle(MenuButton * button, bool &setting)
{
    setting = !setting;
    button->mMsg = getToggleName(setting);
}

#define OPTION(message, setting) \
    new MenuButton(this, message, labelX, y, nullptr, nullptr, false, scale); \
    options[n++] = new MenuButton(this, getToggleName(setting), valueX, y,    \
        [](MenuButton * button, void * param)                                 \
        {                                                                     \
            HitboxMenu * instance = reinterpret_cast<HitboxMenu *>(param);    \
            instance->optionToggle(button, setting);                          \
            return true;                                                      \
        },                                                                    \
        this, false, scale);                                                  \
    y -= yDiff

#define SETTING_COUNT 2

HitboxMenu::HitboxMenu()
{
    const f32 labelX = -330.0f;
    const f32 valueX = labelX + 315.0f;
    const f32 scale = 0.8f;
    const f32 yDiff = (FONT_HEIGHT * scale) + 5.0f;

    MenuButton * options[SETTING_COUNT];
    f32 y = 170;
    int n = 0;

    OPTION("Display NPC Hitboxes", gSettings->hitboxNpc);
    OPTION("Show Disabled NPC Hitboxes", gSettings->hitboxNpcDisabled);

    for (int i = 1; i < SETTING_COUNT; i++)
        buttonLinkVertical(options[i - 1], options[i]);

    // Set starting button and title
    mCurButton = options[0];
    mTitle = "Hitbox Display Options";
}

void HitboxMenu::activateCheck(s8 cameraId, u8 renderMode, f32 z, spm::dispdrv::DispCallback * cb, void * cbParam)
{
    spm::npcdrv::NPCPart * part = reinterpret_cast<spm::npcdrv::NPCPart *>(cbParam);

    // Don't display if setting is disabled
    if (!gSettings->hitboxNpc)
        return;

    // Don't display if hitbox is disabled unless settings say otherwise
    bool in3d = check3d();
    bool partDisabled = in3d ? part->flag2c & 4 : part->flag2c & 2;
    if (!gSettings->hitboxNpcDisabled && partDisabled)
        return;
    
    // Display otherwise
    spm::dispdrv::dispEntry(cameraId, renderMode, z, cb, cbParam);
}

void HitboxMenu::hitboxPatch()
{
    // Remove hitbox check to implement our own
#if (defined SPM_EU0 || defined SPM_EU1 || defined SPM_KR0)
    const u32 baseOffset = 0x51c;
#else
    const u32 baseOffset = 0x504;
#endif
    writeWord(spm::npcdrv::npcDispMain, baseOffset, NOP);
    writeBranchLink(spm::npcdrv::npcDispMain, baseOffset + 0x1c, activateCheck);
}

}
