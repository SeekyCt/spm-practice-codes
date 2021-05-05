#include "evt_cmd.h"
#include "mapselectmenu.h"
#include "mainmenu.h"
#include "nandsettings.h"
#include "util.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_npc.h>
#include <spm/evt_seq.h>
#include <spm/mario_pouch.h>
#include <spm/mapdata.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/wpadmgr.h>
#include <wii/OSError.h>
#include <wii/string.h>
#include <wii/stdio.h>
#include <wii/wpad.h>

namespace mod {

struct EntranceNameList
{
    int count;
    const char * names[];
};

struct MapGroup
{
    char name[4];
    u16 count;
    EntranceNameList ** entranceNames;
};

// TODO: korea maps?
static MapGroup groups[] = {
    {"aa1",  2}, {"aa2",  2}, {"aa3",  1}, {"aa4",  1}, // cutscene
    {"bos",  1}, {"dos",  1}, {"dan", 70}, {"mac", 30}, // misc
    {"he1",  6}, {"he2",  9}, {"he3",  8}, {"he4", 12}, // chapter 1
    {"mi1", 11}, {"mi2", 11}, {"mi3",  6}, {"mi4", 15}, // chapter 2
    {"ta1",  9}, {"ta2",  6}, {"ta3",  8}, {"ta4", 15}, // chapter 3
    {"sp1",  7}, {"sp2", 10}, {"sp3",  7}, {"sp4", 17}, // chapter 4
    {"gn1",  5}, {"gn2",  6}, {"gn3", 16}, {"gn4", 17}, // chapter 5
    {"wa1", 27}, {"wa2", 25}, {"wa3", 25}, {"wa4", 26}, // chapter 6
    {"an1", 11}, {"an2", 10}, {"an3", 16}, {"an4", 12}, // chapter 7
    {"ls1", 12}, {"ls2", 18}, {"ls3", 13}, {"ls4", 13}, // chapter 8
//  {"mg1",  1}, {"mg2",  5}, {"mg3",  5}, {"mg4",  1}, // minigames
//  {"tst",  2}, {"kaw",  5}                            // half-removed
};

char MapSelectMenu::sFullMapStr[7];
bool MapSelectMenu::playEffectThisMapChange = false;

void MapSelectMenu::updateGroupDisp()
{
    // Change message to current group name
    mGroupDisp->mMsg = groups[mGroup].name;
}

void MapSelectMenu::updateMapDisp()
{
    // Format map number to string and display
    wii::stdio::sprintf(mMapStr, "%02d", mMap);
}

void MapSelectMenu::groupUp(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Decrement group and reset map
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    instance->mMap = 1;
    instance->mGroup -= 1;

    // Loop around if the end of the group list is reached
    if (instance->mGroup < 0)
        instance->mGroup = ARRAY_SIZEOF(groups) - 1;

    // Update displays
    instance->updateGroupDisp();
    instance->updateMapDisp();
}

void MapSelectMenu::groupDown(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Increment group and reset map
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    instance->mMap = 1;
    instance->mGroup += 1;

    // Loop around if the end of the group list is reached
    if (instance->mGroup >= (int) ARRAY_SIZEOF(groups))
        instance->mGroup = 0;
    
    // Update displays
    instance->updateGroupDisp();
    instance->updateMapDisp();
}

void MapSelectMenu::mapUp(MenuScroller * button, void * param)
{
    (void) button;

    // Get map number and group
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    int group = instance->mGroup;
    int map = instance->mMap + 1;

    // Validate map number
    if (map > groups[group].count)
    {
        map = 1;
    }
    else if (wii::string::strcmp(groups[group].name, "dan") == 0)
    {
        // dan is missing 5-10, 15-20, 25-29, 31-40, 45-60, 65-69
        switch (map)
        {
            case 5:
                map = 11;
                break;
            case 15:
                map = 21;
                break;
            case 25:
                map = 30;
                break;
            case 31:
                map = 41;
                break;
            case 45:
                map = 61;
                break;
            case 65:
                map = 70;
                break;
        }
    }
    else if (wii::string::strcmp(groups[group].name, "mac") == 0)
    {
        // mac is missing 10, 13, 20-21, 23-29
        switch (map)
        {
            case 10:
                map = 11;
                break;
            case 13:
                map = 14;
                break;
            case 20:
                map = 22;
                break;
            case 23:
                map = 30;
                break;
        }
    }

    // Store new map number and display
    instance->mMap = map;
    instance->updateMapDisp();

}

void MapSelectMenu::mapDown(MenuScroller * scroller, void * param)
{
    (void) scroller;
    
    // Get map number and group
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    int group = instance->mGroup;
    int map = instance->mMap - 1;

    // Validate map number
    if (map < 1)
    {
        map = groups[group].count;
    }
    else if (wii::string::strcmp(groups[group].name, "dan") == 0)
    {
        // dan is missing 5-10, 15-20, 25-29, 31-40, 45-60, 65-69
        switch (map)
        {
            case 10:
                map = 4;
                break;
            case 20:
                map = 14;
                break;
            case 29:
                map = 24;
                break;
            case 40:
                map = 30;
                break;
            case 60:
                map = 44;
                break;
            case 69:
                map = 64;
                break;
        }
    }
    else if (wii::string::strcmp(groups[group].name, "mac") == 0)
    {
        // mac is missing 10, 13, 20-21, 23-29
        switch (map)
        {
            case 10:
                map = 9;
                break;
            case 13:
                map = 12;
                break;
            case 21:
                map = 19;
                break;
            case 29:
                map = 22;
                break;
        }
    }

    // Store new map number and display
    instance->mMap = map;
    instance->updateMapDisp();
}

bool MapSelectMenu::doMapChange(MenuButton * button, void * param)
{
    (void) button;

    // Call non-static method to actually change map
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    instance->_doMapChange();

    // Close menu without returning to parent
    MenuWindow::sCurMenu = nullptr;
    delete instance;
    return false;
}

EVT_DECLARE_USER_FUNC(evt_get_cur_pixl, 1)

int evt_get_cur_pixl(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    int pixl = spm::mario_pouch::pouchGetCurPixl();
    spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], pixl);
    return 2;
}

// Script to animate the player vanishing and change the map
static EVT_BEGIN(map_change_effect)

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

    // Change map
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, 3, PTR(MapSelectMenu::sFullMapStr), 0)

RETURN()
EVT_END()

// Script to animate the player reappearing
static EVT_BEGIN(reappear)
    // Disable player movement
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)

    // Hide player, Tippi and pixl if present
    USER_FUNC(spm::evt_mario::evt_mario_flag8_onoff, 1, 1)
    USER_FUNC(spm::evt_guide::evt_guide_check_flag0, 0x80, LW(14))
    IF_EQUAL(LW(14), 0)
        USER_FUNC(spm::evt_guide::evt_guide_flag2_onoff, 1, 1)
    END_IF()
    USER_FUNC(evt_get_cur_pixl, LW(15))
    IF_NOT_EQUAL(LW(15), 0)
        USER_FUNC(spm::evt_fairy::evt_fairy_flag_onoff, 1, 2)
    END_IF()

    // Wait until the map has faded in
    USER_FUNC(spm::evt_fade::evt_fade_end_wait, -1)

    // Show the player and play teleport effect
    USER_FUNC(spm::evt_mario::evt_mario_flag8_onoff, 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, PTR("mario"))

    // If present, show Tippi and play teleport effect
    IF_EQUAL(LW(14), 0)
        USER_FUNC(spm::evt_guide::evt_guide_flag2_onoff, 0, 1)
        USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, PTR("guide"))
    END_IF()

    // If present, show pixl and play teleport effect
    IF_NOT_EQUAL(LW(15), 0)
        USER_FUNC(spm::evt_fairy::evt_fairy_flag_onoff, 0, 2)
        USER_FUNC(spm::evt_npc::evt_npc_teleport_effect, 1, PTR("fairy"))
    END_IF()

    WAIT_MSEC(600)

    // Enable player movement
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    
    RETURN()
EVT_END()

void MapSelectMenu::_doMapChange()
{
    // Build map string
    wii::stdio::sprintf(sFullMapStr, "%s_%02d", groups[mGroup].name, mMap);

    if (settings->mapChangeEffect)
    {
        // Signal to play animation if enabled
        playEffectThisMapChange = true;

        // Run script
        spm::evtmgr::evtEntry(map_change_effect, 0, 0);
    }
    else
    {
        spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, sFullMapStr, 0);
    }
}

void MapSelectMenu::onMapChanged()
{
    // If this was a forced map change, run animation script
    if (playEffectThisMapChange)
    {
        spm::evtmgr::evtEntry(reappear, 0, 0);
        playEffectThisMapChange = false;
    }
}

int getMapGroup(const char * map)
{
    // Iterate over all known groups
    for (int i = 0; i < (int) ARRAY_SIZEOF(groups); i++)
    {
        // Return the index if the map matches the group
        if (wii::string::strncmp(map, groups[i].name, 3) == 0)
            return i;
    }

    // Return -1 if not found
    return -1;
}

void MapSelectMenu::disp()
{
    // Run main display function
    MenuWindow::disp();

    // Toggle teleport if minus is pressed
    if (spm::wpadmgr::wpadGetButtonsPressed(0) & WPAD_BTN_MINUS)
        settings->mapChangeEffect = !settings->mapChangeEffect;
    
    // Display option text
    const char * msg = settings->mapChangeEffect ?
        "Display teleport effect (<icon PAD_MINUS 0.8 0 35 0><col ffffffff>): on" : 
        "Display teleport effect (<icon PAD_MINUS 0.8 0 35 0><col ffffffff>): off";
    drawMessage(msg, -320.0f, -100.0f, nullptr, 0.8f);
}

MapSelectMenu::MapSelectMenu()
{
    // Try set to current map
    mGroup = getMapGroup(spm::spmario::gp->mapName);
    if (mGroup != -1)
    {
        // Extract current map number if group is valid
        wii::string::sscanf(spm::spmario::gp->mapName + 4, "%d", &mMap);
    }
    else
    {
        // Set to fallback, map is not known
        mGroup = 0;
        mMap = 1;
    }

    // Position constants
    const f32 groupDispX = -50.0f;
    const f32 mapDispX = groupDispX + 60.0f;
    const f32 dispsY = 20.0f;
    
    // Init display buttons
    mGroupDisp = new MenuScroller(
        this, groups[mGroup].name, groupDispX, dispsY, 20.0f, groupUp, groupDown, this, doMapChange, this
    );
    wii::stdio::sprintf(mMapStr, "%02d", mMap);
    mMapDisp = new MenuScroller(
        this, mMapStr, mapDispX, dispsY, 10.0f, mapUp, mapDown, this, doMapChange, this
    );
    buttonLinkHorizontal(mGroupDisp, mMapDisp);

    // Set starting button and title
    mCurButton = mGroupDisp;
    mTitle = "Map Select";
}

void MapSelectMenu::close()
{
    // Change back to parent menu
    delete MenuWindow::sCurMenu;
    MenuWindow::sCurMenu = new MainMenu();
}

static EntranceNameList * scanScript(const int * script)
{
    spm::evt_door::DoorDesc * doors = nullptr;
    int doorCount = 0;
    spm::evt_door::DokanDesc * dokans = nullptr;
    int dokanCount = 0;
    spm::evt_door::MapDoorDesc * mapDoors = nullptr;
    int mapDoorCount = 0;
    
    int cmdn;
    int cmd = 0;
    while (cmd != 1) // endscript
    {
        const short * p = reinterpret_cast<const short *>(script);
        cmd = p[1];
        cmdn = p[0];

        if (cmd == 0x5c) // user_func
        {
            u32 func = script[1];
            if (func == (u32) spm::evt_door::evt_door_set_door_descs)
            {
                doors = reinterpret_cast<spm::evt_door::DoorDesc *>(script[2]);
                doorCount = script[3];
            }
            else if (func == (u32) spm::evt_door::evt_door_set_dokan_descs)
            {
                
                dokans = reinterpret_cast<spm::evt_door::DokanDesc *>(script[2]);
                dokanCount = script[3];
            }
            else if (func == (u32) spm::evt_door::evt_door_set_map_door_descs)
            {
                mapDoors = reinterpret_cast<spm::evt_door::MapDoorDesc *>(script[2]);
                mapDoorCount = script[3];
            }
        }

        script += cmdn + 1;
    }

    int entranceCount = doorCount + dokanCount + mapDoorCount;
    
    if (entranceCount == 0)
        return nullptr;
    
    EntranceNameList * list = reinterpret_cast<EntranceNameList *>(new int[entranceCount + 1]);
    list->count = entranceCount;

    int n = 0;
    for (int i = 0; i < doorCount; i++)
        list->names[n++] = doors[i].name;
    for (int i = 0; i < dokanCount; i++)
        list->names[n++] = dokans[i].name;
    for (int i = 0; i < mapDoorCount; i++)
        list->names[n++] = mapDoors[i].name;

    return list;
}

void MapSelectMenu::scanEntrances()
{
    for (u32 i = 0; i < ARRAY_SIZEOF(groups); i++)
    {
        groups[i].entranceNames = new EntranceNameList * [groups[i].count];
        for (int j = 0; j < groups[i].count; j++)
        {
            char name[32];
            wii::stdio::sprintf(name, "%s_%02d", groups[i].name, j + 1);

            spm::mapdata::MapData * md = spm::mapdata::mapDataPtr(name);
            if ((md != nullptr) && (md->script != nullptr))
                groups[i].entranceNames[j] = scanScript((int *) md->script);
            else
                groups[i].entranceNames[j] = nullptr;
        }
    }

    /*
    for (u32 i = 0; i < ARRAY_SIZEOF(groups); i++)
    {
        EntranceNameList ** lists = groups[i].entranceNames;
        for (int j = 0; j < groups[i].count; j++)
        {
            EntranceNameList * list = lists[j];
            if (list == nullptr)
                continue;

            wii::OSError::OSReport("%s_%02d %x:\n", groups[i].name, j + 1, list);
            
            for (int n = 0; n < list->count; n++)
                wii::OSError::OSReport("\t%s\n", list->names[n]);
        }
    }
    */
}

}
