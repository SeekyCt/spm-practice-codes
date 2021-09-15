#include "mod_ui_base/colours.h"
#include "mod_ui_base/menuwindow.h"
#include "inputwindow.h"
#include "nandsettings.h"
#include "util.h"

#include <types.h>
#include <spm/icondrv.h>
#include <spm/mario.h>
#include <spm/seqdrv.h>
#include <spm/wpadmgr.h>
#include <wii/stdio.h>
#include <wii/string.h>
#include <wii/wpad.h>

namespace mod {

InputWindow * InputWindow::sInstance = nullptr;

#define X_DIFF 35.0f
#define BASE_X (X_DIFF / 2.0f) - (X_DIFF * 2.0f)
// TODO: fix height on 4:3
#define BASE_Y -200.0f
#define DPAD_DISP_X BASE_X
#define DPAD_DISP_Y -200.0f

InputWindow::ButtonDef InputWindow::sDefs[] = {
    {WPAD_BTN_LEFT | WPAD_BTN_RIGHT | WPAD_BTN_UP | WPAD_BTN_DOWN,
    spm::icondrv::ICON_DPAD_NEUTRAL, 0.6f, 0.6f},
    {WPAD_BTN_1, spm::icondrv::ICON_BTN_1, 1.0f, 0.8f},
    {WPAD_BTN_2, spm::icondrv::ICON_BTN_2, 1.0f, 0.8f},
    {WPAD_BTN_A, spm::icondrv::ICON_BTN_A, 0.9f, 0.7f}
};

InputWindow::DpadDef InputWindow::sDpadDefs[] = {
    {WPAD_BTN_UP, -14.5f, 22.5f},
    {WPAD_BTN_DOWN, 4.0f, 22.5f},
    {WPAD_BTN_LEFT, -5.0f, 14.0f},
    {WPAD_BTN_RIGHT, -5.0f, 30.0f}
};

u32 InputWindow::sPressed[3] = {0, 0, 0};
u32 InputWindow::sPressedIdx = 0;

InputWindow::InputWindow()
{
}

bool InputWindow::showAsPressed(u32 mask)
{
    for (u32 i = 0; i < ARRAY_SIZEOF(sPressed); i++)
    {
        if (sPressed[i] & mask)
            return true;
    }

    return false;
}

void InputWindow::updatePressed()
{
    sPressedIdx = (sPressedIdx + 1) % ARRAY_SIZEOF(sPressed);
    sPressed[sPressedIdx] = spm::wpadmgr::wpadGetButtonsPressed(0);
}

void InputWindow::disp()
{
    // Don't draw over menu, if disabled or if not in game
    // TODO: make own setting
    if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME) || !gSettings->enemyHpBars)
        return;

    // Process current inputs
    updatePressed();
    u32 held = spm::wpadmgr::wpadGetButtonsHeld(0);

    // Draw normal icons
    for (u32 i = 0; i < ARRAY_SIZEOF(sDefs); i++)
    {
        ButtonDef * def = sDefs + i;

        f32 scale;
        if (showAsPressed(def->mask))
            scale = def->pressScale;
        else if (def->mask & held)
            scale = def->holdScale;
        else
            continue;
        
        wii::Vec3 pos = {BASE_X + (i * X_DIFF), BASE_Y, 0.0f};
        spm::icondrv::iconDispGx(scale, &pos, 0x18, def->icon);
    }

    // Draw dpad directions
    // TODO: use custom textures instead?
    for (u32 i = 0; i < ARRAY_SIZEOF(sDpadDefs); i++)
    {
        DpadDef * def = sDpadDefs + i;

        const wii::RGBA * colour;
        
        if (showAsPressed(def->mask))
            colour = &colours::red;
        else if (def->mask & held)
            colour = &colours::yellow;
        else
            continue;

        Window::drawBoxGX(colour, DPAD_DISP_X + def->xOffset, DPAD_DISP_Y +
                          def->yOffset, 10.0f, 10.0f);
    }
}

void InputWindow::init()
{
    sInstance = new InputWindow();
}

}
