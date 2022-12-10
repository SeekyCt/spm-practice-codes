#include "mod_ui_base/colours.h"
#include "mod_ui_base/menuwindow.h"
#include "assets.h"
#include "inputwindow.h"
#include "mod.h"
#include "nandsettings.h"
#include "util.h"

#include <types.h>
#include <spm/gxsub.h>
#include <spm/icondrv.h>
#include <spm/mario.h>
#include <spm/seqdrv.h>
#include <spm/wpadmgr.h>
#include <wii/stdio.h>
#include <wii/string.h>
#include <wii/wpad.h>

namespace mod {

InputWindow * InputWindow::sInstance = nullptr;

#define X_DIFF 17.5f
#define BASE_X -(X_DIFF * (ARRAY_SIZEOF(InputWindow::sDefs) / 2.0f))
#define BASE_Y -85.0f
#define SCALE 0.3125f

#define DPAD_ALL_MASK (WPAD_BTN_UP | WPAD_BTN_DOWN | WPAD_BTN_RIGHT | WPAD_BTN_LEFT)

enum InputTplPos
{
    INP_TPL_1 = 0,
    INP_TPL_2,
    INP_TPL_A,
    INP_TPL_PLUS,
    INP_TPL_MINUS,
    INP_TPL_HOME,
    INP_TPL_UP,
    INP_TPL_DOWN,
    INP_TPL_LEFT,
    INP_TPL_RIGHT,
    INP_TPL_UP_LEFT,
    INP_TPL_UP_RIGHT,
    INP_TPL_DOWN_LEFT,
    INP_TPL_DOWN_RIGHT,
    INP_TPL_DPAD_NONE,

    // Placeholder
    INP_TPL_DPAD = 0xff
};

InputWindow::ButtonDef InputWindow::sDefs[] = {
    {WPAD_BTN_MINUS, INP_TPL_MINUS},
    {WPAD_BTN_PLUS, INP_TPL_PLUS},
    {DPAD_ALL_MASK, INP_TPL_DPAD},
    {WPAD_BTN_2, INP_TPL_2},
    {WPAD_BTN_1, INP_TPL_1},
    {WPAD_BTN_A, INP_TPL_A},
    {WPAD_BTN_HOME, INP_TPL_HOME}
};

InputWindow::DpadDef InputWindow::sDpadDefs[] = {
    {WPAD_BTN_RIGHT, INP_TPL_UP},
    {WPAD_BTN_LEFT, INP_TPL_DOWN},
    {WPAD_BTN_UP, INP_TPL_LEFT},
    {WPAD_BTN_DOWN, INP_TPL_RIGHT},
    {WPAD_BTN_RIGHT | WPAD_BTN_UP, INP_TPL_UP_LEFT},
    {WPAD_BTN_RIGHT | WPAD_BTN_DOWN, INP_TPL_UP_RIGHT},
    {WPAD_BTN_LEFT | WPAD_BTN_UP, INP_TPL_DOWN_LEFT},
    {WPAD_BTN_LEFT | WPAD_BTN_DOWN, INP_TPL_DOWN_RIGHT},
    {WPAD_BTN_NONE, INP_TPL_DPAD_NONE}
};

InputWindow::InputWindow()
{
    // Draw over homebutton menu (since some tricks involve it)
    mDrawOverHbm = true;
}

void InputWindow::disp()
{
    // Don't draw over menu, if disabled or if not in game
    if ((MenuWindow::sCurMenu != nullptr) || (spm::seqdrv::seqGetSeq() != spm::seqdrv::SEQ_GAME) || !gSettings->inputDisplay)
        return;

    // Get current inputs
    u32 held = spm::wpadmgr::wpadGetButtonsHeld(0);

    // Draw enabled icons
    for (u32 i = 0; i < ARRAY_SIZEOF(sDefs); i++)
    {
        ButtonDef * def = sDefs + i;

        // Get tpl index
        u32 tplIdx = def->tplIdx;
        if (def->tplIdx == INP_TPL_DPAD)
        {
            // Special case for dpad since inputs merge
            for (u32 j = 0; j < ARRAY_SIZEOF(sDpadDefs); j++)
            {
                DpadDef * ddef = sDpadDefs + j;
                if (ddef->mask == (held & DPAD_ALL_MASK))
                    tplIdx = ddef->tplIdx;
            }
        }

        // Skip if not pressed
        if ((def->mask & held) == 0) {
            Window::drawTexture(&inpTpl, tplIdx, BASE_X + (i * X_DIFF), BASE_Y, SCALE, &colours::grey);
            continue;
        }        

        // Draw
        Window::drawTexture(&inpTpl, tplIdx, BASE_X + (i * X_DIFF), BASE_Y, SCALE, &colours::white);
    }
}

void InputWindow::init()
{
    sInstance = new InputWindow();
    wii::tpl::TPLBind(&inpTpl);
}

}
