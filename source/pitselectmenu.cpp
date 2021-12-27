#include "evt_cmd.h"
#include "pitselectmenu.h"
#include "nandsettings.h"
#include "scriptvarlog.h"
#include "util.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_npc.h>
#include <spm/evt_seq.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/wpadmgr.h>
#include <wii/string.h>
#include <wii/stdio.h>
#include <wii/wpad.h>

namespace mod {

const char * PitSelectMenu::sMapName;
int PitSelectMenu::sGSW1;

enum {
    PITGROUP_FLIPSIDE,
    PITGROUP_FLOPSIDE
};

static const char * getGroupName(int group)
{
    return group == PITGROUP_FLIPSIDE ? "Flipside" : "Flopside";
}

static const char * getBossStateName()
{
    s32 wracktailBeat = spm::evtmgr_cmd::evtGetValue(nullptr, GSWF(409));
    s32 encounterTimes = spm::evtmgr_cmd::evtGetValue(nullptr, GSW(24));
    if (!wracktailBeat)
        return "Boss state (<icon PAD_PLUS 0.8 0 35 0><col ffffffff>): Wracktail unbeaten";
    else if (encounterTimes == 0)
        return "Boss state (<icon PAD_PLUS 0.8 0 35 0><col ffffffff>): Wracktail beaten";
    else if (encounterTimes == 1)
        return "Boss state (<icon PAD_PLUS 0.8 0 35 0><col ffffffff>): Shadoo visited once";
    else
        return "Boss state (<icon PAD_PLUS 0.8 0 35 0><col ffffffff>): Shadoo beaten";
}

void PitSelectMenu::updateGroupDisp()
{
    // Change message to current group name
    mGroupScroller->mMsg = getGroupName(mGroup);
}

void PitSelectMenu::updateFloorDisp()
{
    // Set floor display to new number
    mFloorScroller->mDispValue = mFloor;
}

bool PitSelectMenu::bossStateToggle(MenuButton * button, void * param)
{
    (void) button;

    PitSelectMenu * instance = reinterpret_cast<PitSelectMenu *>(param);

    s32 wracktailBeat = spm::evtmgr_cmd::evtGetValue(nullptr, GSWF(409));
    s32 encounterTimes = spm::evtmgr_cmd::evtGetValue(nullptr, GSW(24));
    if (!wracktailBeat)
    {
        // Wracktail unbeaten -> wraacktail beaten
        spm::evtmgr_cmd::evtSetValue(nullptr, GSWF(409), true);
    }
    else if (encounterTimes == 0)
    {
        // Wracktail beaten -> shadoo visited once
        spm::evtmgr_cmd::evtSetValue(nullptr, GSW(24), 1);
    }
    else if (encounterTimes == 1)
    {
        // Shadoo visited once -> shadoo beaten
        spm::evtmgr_cmd::evtSetValue(nullptr, GSW(24), 2);
    }
    else
    {
        // Shadoo beaten -> wracktail unbeaten
        spm::evtmgr_cmd::evtSetValue(nullptr, GSWF(409), false);
        spm::evtmgr_cmd::evtSetValue(nullptr, GSW(24), 0);
    }

    instance->mBossState->mMsg = getBossStateName();

    return true;
}

void PitSelectMenu::groupSwap(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Decrement group and reset floor
    PitSelectMenu * instance = reinterpret_cast<PitSelectMenu *>(param);
    instance->mGroup = instance->mGroup == PITGROUP_FLIPSIDE ? PITGROUP_FLOPSIDE : PITGROUP_FLIPSIDE;
    instance->mFloor = 1;

    // Update displays
    instance->updateGroupDisp();
    instance->updateFloorDisp();
}

void PitSelectMenu::floorChange(MenuScrollGroup * scroller, s32 delta, void * param)
{
    (void) scroller;

    PitSelectMenu * instance = reinterpret_cast<PitSelectMenu *>(param);

    // Update value, ensure between 1 and 100
    instance->mFloor += delta;
    if (instance->mFloor > 100)
        instance->mFloor = 100;
    if (instance->mFloor < 1)
        instance->mFloor = 1;

    // Update display
    instance->updateFloorDisp();
}

bool PitSelectMenu::doMapChange(MenuButton * button, void * param)
{
    (void) button;

    // Call non-static method to actually change map
    PitSelectMenu * instance = reinterpret_cast<PitSelectMenu *>(param);
    instance->_doMapChange();

    // Close menu without returning to parent
    MenuWindow::sCurMenu->fullClose();
    return false;
}

// Script to animate the player vanishing and change the map
static EVT_BEGIN(map_change_effect_pit)

    // Disable player movement
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)

    // Hide Mario and play teleport effect
    USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 0, PTR("mario"))
    USER_FUNC(spm::evt_mario::evt_mario_flag8_onoff, 1, 1)

    // If present, hide Tippi and play teleport effect
    USER_FUNC(spm::evt_guide::evt_guide_check_flag0, 0x80, LW(0))
    IF_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 0, PTR("guide"))
        USER_FUNC(spm::evt_guide::evt_guide_flag2_onoff, 1, 1)
    END_IF()

    // If present, hide pixl and play teleport effect
    USER_FUNC(evt_get_cur_pixl, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
        USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 0, PTR("fairy"))
        USER_FUNC(spm::evt_fairy::evt_fairy_flag_onoff, 1, 2)
    END_IF()
    WAIT_FRM(60)

    // Handle pit variables
    GET_RAM(GSW(1), PTR(&PitSelectMenu::sGSW1))

    // Change map
    GET_RAM(LW(0), PTR(&PitSelectMenu::sMapName))
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, LW(0), PTR(nullptr))

    RETURN()
EVT_END()

void PitSelectMenu::_doMapChange()
{
    // Convert floor number and group to dungeon id
    if (mGroup == PITGROUP_FLOPSIDE)
        sGSW1 = mFloor + 100 - 1;
    else
        sGSW1 = mFloor - 1;
    
    // Choose map string
    switch (sGSW1)
    {
        // Flipside special
        case 9:
        case 19:
            sMapName = "dan_21";
            break;
        case 29:
        case 39:
            sMapName = "dan_22";
            break;
        case 49:
        case 59:
            sMapName = "dan_23";
            break;
        case 69:
        case 79:
        case 89:
            sMapName = "dan_24";
            break;
        case 99:
            sMapName = "dan_30";
            break;

        // Flopside special
        case 109:
        case 119:
            sMapName = "dan_61";
            break;
        case 129:
        case 139:
            sMapName = "dan_62";
            break;
        case 149:
        case 159:
            sMapName = "dan_63";
            break;
        case 169:
        case 179:
        case 189:
            sMapName = "dan_64";
            break;
        case 199:
            sMapName = "dan_70";
            break;

        // Enemy rooms
        default:
            // Flipside
            if ((sGSW1 >= 0) && (sGSW1 <= 24))
                sMapName = "dan_01";
            else if ((sGSW1 >= 25) && (sGSW1 <= 49))
                sMapName = "dan_02";
            else if ((sGSW1 >= 50) && (sGSW1 <= 74))
                sMapName = "dan_03";
            else if ((sGSW1 >= 75) && (sGSW1 <= 99))
                sMapName = "dan_04";

            // Flopside
            else if ((sGSW1 >= 100) && (sGSW1 <= 124))
                sMapName = "dan_41";
            else if ((sGSW1 >= 125) && (sGSW1 <= 149))
                sMapName = "dan_42";
            else if ((sGSW1 >= 150) && (sGSW1 <= 174))
                sMapName = "dan_43";
            else if ((sGSW1 >= 175) && (sGSW1 <= 199))
                sMapName = "dan_44";
    }

    if (gSettings->mapChangeEffect)
    {
        // Run script with effects
        spm::evtmgr::evtEntry(map_change_effect_pit, 0, 0);
    }
    else
    {
        // Change map with no teleport effect
        spm::evtmgr_cmd::evtSetValue(nullptr, GSW(1), sGSW1);
        spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, sMapName, nullptr);
    }
}

void PitSelectMenu::close()
{
    // Re-enable script variable logging
    scriptVarLogOnOff(true);

    // Close as normal
    MapMenu::close();
}

void PitSelectMenu::fullClose()
{
    // Re-enable script variable logging
    scriptVarLogOnOff(true);

    // Close as normal
    MapMenu::fullClose();
}

PitSelectMenu::PitSelectMenu()
{
    // Disable script variable logging while open
    scriptVarLogOnOff(false);

    // Try set to current floor, default to flipside 1 if not in pit
    if (wii::string::strncmp(spm::spmario::gp->mapName, "dan", 3) == 0)
    {
        int gsw1 = spm::evtmgr_cmd::evtGetValue(nullptr, GSW(1));
        if (gsw1 > 100)
        {
            mGroup = PITGROUP_FLOPSIDE;
            mFloor = gsw1 - 100;
        }
        else
        {
            mGroup = PITGROUP_FLIPSIDE;
            mFloor = gsw1;
        }
    }
    else
    {
        mGroup = PITGROUP_FLIPSIDE;
        mFloor = 1;
    }

    // Position constants
    const f32 groupLabelX = -170.0f;
    const f32 groupDispX = groupLabelX + 55.0f;
    const f32 floorLabelX = groupDispX + 125.0f;
    const f32 floorDispX = floorLabelX + 85.0f;
    const f32 dispsY = 20.0f;
    const f32 passiveButtonScale = 0.8f; // from MapMenu
    const f32 passiveButtonX = -320.0f; // from MapMenu
    const f32 effectToggleY = -100.0f; // from MapMenu
    const f32 bossToggleY = effectToggleY - 30.0f;
    
    // Init display buttons
    new MenuButton(this, "Pit:", groupLabelX, dispsY);
    mGroupScroller = new MenuScroller(
        this, getGroupName(mGroup), groupDispX, dispsY, 45.0f, groupSwap, groupSwap, this, doMapChange, this
    );
    new MenuButton(this, "Floor:", floorLabelX, dispsY);
    mFloorScroller = new MenuScrollGroup(this, mFloor, floorDispX, dispsY, floorChange, this, 3, false, doMapChange, this);
    buttonLinkHorizontal(mGroupScroller, mFloorScroller);
    mBossState = new PassiveButton(
        this, getBossStateName(), passiveButtonX, bossToggleY, WPAD_BTN_PLUS, bossStateToggle, this, passiveButtonScale, nullptr, true
    );

    // Set starting button and title
    mCurButton = mGroupScroller;
    mTitle = "Pit Floor Warp";
}

}
