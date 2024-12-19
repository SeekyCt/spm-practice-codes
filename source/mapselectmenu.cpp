#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evt_door.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_npc.h>
#include <spm/evt_seq.h>
#include <spm/evt_sub.h>
#include <spm/fadedrv.h>
#include <spm/map_data.h>
#include <spm/memory.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <spm/wpadmgr.h>
#include <spm/rel/machi.h>
#include <wii/wpad.h>
#include <string.h>
#include <stdio.h>

#include "evt_cmd.h"
#include "mapselectmenu.h"
#include "nandsettings.h"
#include "util.h"

namespace mod {

struct EntranceNameList
{
    int count;
    const char * names[];
};

struct MapGroup
{
    char name[4];
    u16 firstId;
    u16 count;
    EntranceNameList ** entranceNames;
};

// TODO: korea maps?
static MapGroup groups[] = {
    {"aa1", 1,  2, 0}, {"aa2", 1,  2, 0}, {"aa3",  1,  1, 0}, {"aa4", 1,  1, 0}, // cutscene
    {"bos", 1,  1, 0}, {"dos", 1,  1, 0}, {"dan", 11, 14, 0}, {"mac", 1, 30, 0}, // misc
    {"he1", 1,  6, 0}, {"he2", 1,  9, 0}, {"he3",  1,  8, 0}, {"he4", 1, 12, 0}, // chapter 1
    {"mi1", 1, 11, 0}, {"mi2", 1, 11, 0}, {"mi3",  1,  6, 0}, {"mi4", 1, 15, 0}, // chapter 2
    {"ta1", 1,  9, 0}, {"ta2", 1,  6, 0}, {"ta3",  1,  8, 0}, {"ta4", 1, 15, 0}, // chapter 3
    {"sp1", 1,  7, 0}, {"sp2", 1, 10, 0}, {"sp3",  1,  7, 0}, {"sp4", 1, 17, 0}, // chapter 4
    {"gn1", 1,  5, 0}, {"gn2", 1,  6, 0}, {"gn3",  1, 16, 0}, {"gn4", 1, 17, 0}, // chapter 5
    {"wa1", 1, 27, 0}, {"wa2", 1, 25, 0}, {"wa3",  1, 25, 0}, {"wa4", 1, 26, 0}, // chapter 6
    {"an1", 1, 11, 0}, {"an2", 1, 10, 0}, {"an3",  1, 16, 0}, {"an4", 1, 12, 0}, // chapter 7
    {"ls1", 1, 12, 0}, {"ls2", 1, 18, 0}, {"ls3",  1, 13, 0}, {"ls4", 1, 13, 0}, // chapter 8
//  {"mg1", 1,  1, 0}, {"mg2", 1,  5, 0}, {"mg3",  1,  5, 0}, {"mg4", 1,  1, 0}, // minigames
//  {"tst", 1,  2, 0}, {"kaw", 1,  5, 0}                                         // half-removed
};

char MapSelectMenu::sFullMapStr[7];
char MapSelectMenu::sDoorStr[32];
bool MapSelectMenu::playEffectThisMapChange = false;

void MapSelectMenu::updateGroupDisp()
{
    // Change message to current group name
    mGroupDisp->mMsg = groups[mGroup].name;
}

void MapSelectMenu::updateMapDisp()
{
    // Format map number to string and display
    sprintf(mMapStr, "%02d", mMap);
}

void MapSelectMenu::updateEntranceDisp()
{
    // Change message to current entrance name
    if (mEntrance == 0)
        mEntranceDisp->mMsg = "default";
    else
        mEntranceDisp->mMsg = groups[mGroup].entranceNames[mMap - 1]->names[mEntrance - 1];
}

void MapSelectMenu::groupUp(MenuScroller * scroller, void * param)
{
    (void) scroller;

    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);

    // Decrement group, loop around if the end of the list is reached
    instance->mGroup -= 1;
    if (instance->mGroup < 0)
        instance->mGroup = ARRAY_SIZEOF(groups) - 1;

    // Reset map and entrance
    instance->mMap = groups[instance->mGroup].firstId;
    instance->mEntrance = 0;

    // Update displays
    instance->updateGroupDisp();
    instance->updateMapDisp();
    instance->updateEntranceDisp();
}

void MapSelectMenu::groupDown(MenuScroller * scroller, void * param)
{
    (void) scroller;

    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);

    // Increment group, loop around if the end of the list is reached
    instance->mGroup += 1;
    if (instance->mGroup >= (s32) ARRAY_SIZEOF(groups))
        instance->mGroup = 0;

    // Reset map and entrance
    instance->mMap = groups[instance->mGroup].firstId;
    instance->mEntrance = 0;
    
    // Update displays
    instance->updateGroupDisp();
    instance->updateMapDisp();
    instance->updateEntranceDisp();
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
        map = groups[group].firstId;
    }
    else if (strcmp(groups[group].name, "mac") == 0)
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

    // Store new map number, reset entrance and update display
    instance->mMap = map;
    instance->mEntrance = 0;
    instance->updateMapDisp();
    instance->updateEntranceDisp();
}

void MapSelectMenu::mapDown(MenuScroller * scroller, void * param)
{
    (void) scroller;
    
    // Get map number and group
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    int group = instance->mGroup;
    int map = instance->mMap - 1;

    // Validate map number
    if (map < groups[group].firstId)
    {
        map = groups[group].count;
    }
    else if (strcmp(groups[group].name, "mac") == 0)
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

    // Store new map number, reset entrance and update display
    instance->mMap = map;
    instance->mEntrance = 0;
    instance->updateMapDisp();
    instance->updateEntranceDisp();
}

void MapSelectMenu::entranceUp(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Decrement entrance
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    instance->mEntrance -= 1;

    // Loop around if the end of the entrance list is reached
    if (instance->mEntrance < 0)
        instance->mEntrance = groups[instance->mGroup].entranceNames[instance->mMap - 1]->count;

    // Update display
    instance->updateEntranceDisp();
}

void MapSelectMenu::entranceDown(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Increment entrance
    MapSelectMenu * instance = reinterpret_cast<MapSelectMenu *>(param);
    instance->mEntrance += 1;

    // Loop around if the end of the group list is reached
    if (instance->mEntrance > groups[instance->mGroup].entranceNames[instance->mMap - 1]->count)
        instance->mEntrance = 0;
    
    // Update display
    instance->updateEntranceDisp();
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
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, PTR(MapSelectMenu::sFullMapStr), PTR(MapSelectMenu::sDoorStr))

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
    sprintf(sFullMapStr, "%s_%02d", groups[mGroup].name, mMap);

    // Prepare door string
    if (mEntrance == 0)
        strcpy(sDoorStr, "");
    else
        strcpy(sDoorStr, groups[mGroup].entranceNames[mMap - 1]->names[mEntrance - 1]);
    
    // Set Normal Transition
    spm::fadedrv::fadeSetMapChangeTransition(2, 1);

    if (gSettings->mapChangeEffect)
    {
        // Signal to play re-appear animation if enabled and not using a normal entrance
        spm::map_data::MapData * md = spm::map_data::mapDataPtr(sFullMapStr);
        bool hasDefaultEntrance = (md != nullptr) ? (md->fallbackDoorName != nullptr) : false;
        if ((mEntrance == 0) && !hasDefaultEntrance)
            playEffectThisMapChange = true;

        // Run script with effects
        spm::evtmgr::evtEntry(map_change_effect, 0, 0);
    }
    else
    {
        // Change map with no teleport effect
        spm::seqdrv::seqSetSeq(spm::seqdrv::SEQ_MAPCHANGE, sFullMapStr, sDoorStr);
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
    for (int i = 0; i < (s32) ARRAY_SIZEOF(groups); i++)
    {
        // Return the index if the map matches the group
        if (strncmp(map, groups[i].name, 3) == 0)
            return i;
    }

    // Return -1 if not found
    return -1;
}

MapSelectMenu::MapSelectMenu()
{
    // Try set to current map
    mGroup = getMapGroup(spm::spmario::gp->mapName);
    if (mGroup != -1)
    {
        // Extract current map number if group is valid
        sscanf(spm::spmario::gp->mapName + 4, "%d", &mMap);

        // dan has special handling since only the unused maps are available here
        if (strncmp(spm::spmario::gp->mapName, "dan", 3) == 0)
        {
            if ((mMap < 11) || (mMap > 14))
                mMap = 11;
        }
    }
    else
    {
        // Set to fallback, map is not known
        mGroup = 0;
        mMap = 1;
    }

    // Position constants
    const f32 mapLabelX = -180.0f;
    const f32 groupDispX = mapLabelX + 70.0f;
    const f32 mapDispX = groupDispX + 60.0f;
    const f32 entranceLabelX = mapDispX + 50.0f;
    const f32 entranceDispX = entranceLabelX + 140.0f;
    const f32 dispsY = 20.0f;
    
    // Init display buttons
    new MenuButton(this, "Map:", mapLabelX, dispsY);
    mGroupDisp = new MenuScroller(
        this, groups[mGroup].name, groupDispX, dispsY, 20.0f, groupUp, groupDown, this, doMapChange, this
    );
    updateMapDisp(); // generates mMapStr
    mMapDisp = new MenuScroller(
        this, mMapStr, mapDispX, dispsY, 10.0f, mapUp, mapDown, this, doMapChange, this
    );

    new MenuButton(this, "Entrance:", entranceLabelX, dispsY);
    mEntranceDisp = new MenuScroller(
        this, "default", entranceDispX, dispsY, 30.0f, entranceUp, entranceDown, this, doMapChange, this
    );

    buttonLinkHorizontal(mGroupDisp, mMapDisp);
    buttonLinkHorizontal(mMapDisp, mEntranceDisp);

    // Set starting button and title
    mCurButton = mMapDisp;
    mTitle = "Map Select";
}

static EntranceNameList * scanScript(const int * script)
{
    // Return an empty list if there's no script
    if (script == nullptr)
    {
        EntranceNameList * list = reinterpret_cast<EntranceNameList *>(new int[1]);
        list->count = 0;
        return list;
    }

    // Initialise entrance type information
    spm::evt_door::DokanDesc * dokans = nullptr;
    int dokanCount = 0;
    spm::evt_door::MapDoorDesc * mapDoors = nullptr;
    int mapDoorCount = 0;
    spm::machi::ElvDesc * elvs = nullptr;
    int elvCount = 0;

    // Initialize 15 entrances for cutscenes and other stuff
    #define OTHERS_MAX 15
    char ** others = new char*[OTHERS_MAX];
    int othersCount = 0;
    
    // Find entrances
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
            if (func == (u32) spm::evt_door::evt_door_set_dokan_descs)
            {
                dokans = reinterpret_cast<spm::evt_door::DokanDesc *>(script[2]);
                dokanCount = script[3];
            }
            else if (func == (u32) spm::evt_door::evt_door_set_map_door_descs)
            {
                mapDoors = reinterpret_cast<spm::evt_door::MapDoorDesc *>(script[2]);
                mapDoorCount = script[3];
            }
            else if (func == (u32) spm::machi::evt_machi_set_elv_descs)
            {
                elvs = reinterpret_cast<spm::machi::ElvDesc *>(script[2]);
                elvCount = script[3];
            }
            else if (func == (u32) spm::evt_sub::evt_sub_get_entername)
            {
                const int * next_script = script + cmdn + 1;
                const short * next_p = reinterpret_cast<const short *>(next_script);
                int next_cmd = next_p[1];

                if (next_cmd == 0xc || next_cmd == 0xd) // if_str_equal, if_str_not_equal
                {
                    assert(othersCount < OTHERS_MAX, "Other entrances table too small");
                    
                    char* entrance = reinterpret_cast<char *>(next_script[2]);

                    // Prevent duplicates
                    bool exists = false;

                    // Without severe refactoring this is how we're gonna do it
                    for (int i = 0; i < othersCount; i++)
                        if (strcmp(others[i], entrance) == 0)
                        {
                            exists = true;
                            break;
                        }
                    
                    for (int i = 0; i < dokanCount; i++)
                        if (strcmp(dokans[i].name, entrance) == 0)
                        {
                            exists = true;
                            break;
                        }
                    
                    for (int i = 0; i < mapDoorCount; i++)
                        if (strcmp(mapDoors[i].name_l, entrance) == 0)
                        {
                            exists = true;
                            break;
                        }
                    
                    for (int i = 0; i < elvCount; i++)
                        if (strcmp(elvs[i].name, entrance) == 0)
                        {
                            exists = true;
                            break;
                        }

                    if (!exists)
                    {
                        others[othersCount] = entrance;
                        othersCount++;
                    }
                    
                }
            }
        }

        script += cmdn + 1;
    }

    // Create list
    int entranceCount = dokanCount + mapDoorCount + elvCount + othersCount;
    
    EntranceNameList * list = reinterpret_cast<EntranceNameList *>(new int[entranceCount + 1]);
    list->count = entranceCount;

    int n = 0;
    for (int i = 0; i < dokanCount; i++)
        list->names[n++] = dokans[i].name;
    for (int i = 0; i < mapDoorCount; i++)
        list->names[n++] = mapDoors[i].name_l;
    for (int i = 0; i < elvCount; i++)
        list->names[n++] = elvs[i].name;
    for (int i = 0; i < othersCount; i++)
         list->names[n++] = others[i];

    delete[] others;

    return list;
}

void MapSelectMenu::scanEntrances()
{
    // Scan all maps for their entrances
    for (u32 i = 0; i < ARRAY_SIZEOF(groups); i++)
    {
        // Create list pointer array for this group
        groups[i].entranceNames = new EntranceNameList * [groups[i].count];

        // Run for all maps in this group
        for (int j = 0; j < groups[i].count; j++)
        {
            // Generate map name string
            char name[32];
            sprintf(name, "%s_%02d", groups[i].name, j + 1);

            // Generate list for this map
            spm::map_data::MapData * md = spm::map_data::mapDataPtr(name);
            int * script = md != nullptr ? (int *) md->initScript : nullptr;
            groups[i].entranceNames[j] = scanScript(script);
        }
    }
}

}
