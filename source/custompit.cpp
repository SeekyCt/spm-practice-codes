#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/filemgr.h>
#include <spm/item_data.h>
#include <spm/lz_embedded.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/npcdrv.h>
#include <spm/npcmisc.h>
#include <spm/itemdrv.h>
#include <spm/item_data.h>
#include <spm/parse.h>
#include <spm/system.h>
#include <spm/rel/dan.h>
#include <wii/cx.h>
#include <wii/dvd.h>
#include <msl/string.h>

#include "mod_ui_base/colours.h"
#include "consolewindow.h"
#include "custompit.h"
#include "nandsettings.h"
#include "patch.h"
#include "util.h"

namespace mod {

using spm::evtmgr::EvtEntry;
using spm::dan::DanWork;
using spm::dan::DanDungeon;
using spm::dan::dan_wp;
using spm::lz_embedded::pitText;

// Re-implementation of decompiled function for custom text support
static int custom_evt_dan_read_data(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;

    static bool useCustomText;

    // Allocate work & check whether to use custom text on first run
    if (isFirstCall)
    {
        // If enabled, check that wiidungeon.xml exists
        useCustomText = false;
        if (gSettings->customPitText)
        {
            if (wii::dvd::DVDConvertPathToEntrynum(CUSTOM_PIT_FILENAME) != -1)
                useCustomText = true;
            else
                ConsoleWindow::push("ERROR: " CUSTOM_PIT_FILENAME " not found.", &colours::red);
        }

        // Allocate work
        dan_wp = new (spm::memory::Heap::HEAP_MAP) DanWork;
        msl::string::memset(dan_wp, 0, sizeof(*dan_wp));
        dan_wp->dungeons = new (spm::memory::Heap::HEAP_MAP) DanDungeon[DUNGEON_MAX];
        msl::string::memset(dan_wp->dungeons, 0, sizeof(DanDungeon[DUNGEON_MAX]));
    }

    // Prepare pit text to be read
    char * decompPitText = nullptr;
    spm::filemgr::FileEntry * file = nullptr;
    if (useCustomText)
    {
        if (spm::filemgr::fileAsyncf(0, 0, CUSTOM_PIT_FILENAME) == 0)
            return EVT_RET_BLOCK_WEAK;

        file = spm::filemgr::fileAllocf(0, CUSTOM_PIT_FILENAME);
        spm::parse::parseInit((char *) file->sp->data, file->sp->size);
    }
    else
    {
        u32 size = wii::cx::CXGetUncompressedSize(pitText); // GCC can't handle CXGetCompressionHeader
        decompPitText = new char[size];
        wii::cx::CXUncompressLZ(pitText, decompPitText);
        spm::parse::parseInit(decompPitText, size);
    }

    // Add all dungeon entries to work
    while (spm::parse::parsePush("<Dungeon>"))
    {
        // Read no (dungeon id)
        int no = 0;
        int i = 0;
        spm::parse::parseTagGet1("<no>", spm::parse::PARSE_VALUE_TYPE_INT, &no);
        // assertf(144, no >= 0 && no < DUNGEON_MAX, "なんか番号がおかしい [%d]", no);
        assertf(no >= 0 && no < DUNGEON_MAX, "Dungeon number out of range [%d]", no);

        // Read item id (chest contents in chest rooms, null & unused elsewhere)
        char itemName[64];
        spm::parse::parseTagGet1("<item>", spm::parse::PARSE_VALUE_TYPE_STRING, itemName);
        s32 itemId = spm::itemdrv::itemTypeNameToId(itemName);
        if (msl::string::strcmp(itemName, "ITEM_ID_NULL") != 0 && msl::string::strcmp(itemName, "NULL") != 0)
            assertf(itemId != 0, "Invalid item name [%s]", itemName);
        dan_wp->dungeons[no].item = itemId;

        // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
        spm::parse::parseTagGet1("<map>", spm::parse::PARSE_VALUE_TYPE_INT, &dan_wp->dungeons[no].map);

        // Read doors
        while (spm::parse::parsePush("<door>"))
        {
            s32 enter, exit;
            spm::parse::parseTagGet1("<enter>", spm::parse::PARSE_VALUE_TYPE_INT, &enter);
            assertf(1 <= enter && enter <= 32, "Invalid enter door id %d", enter);
            dan_wp->dungeons[no].doors[i].enter = enter;
            spm::parse::parseTagGet1("<exit>", spm::parse::PARSE_VALUE_TYPE_INT, &exit);
            assertf(1 <= exit && exit <= 32, "Invalid exit door id %d", exit);
            dan_wp->dungeons[no].doors[i].exit = exit;
            spm::parse::parsePopNext();
            i++;
        }
        dan_wp->dungeons[no].doorCount = i;

        // Read enemies
        i = 0;
        while (spm::parse::parsePush("<enemy>"))
        {
            spm::parse::parseTagGet1("<name>", spm::parse::PARSE_VALUE_TYPE_INT, &dan_wp->dungeons[no].enemies[i].name);
            spm::parse::parseTagGet1("<num>", spm::parse::PARSE_VALUE_TYPE_INT, &dan_wp->dungeons[no].enemies[i].num);
            spm::parse::parseTagGet1("<pos>", spm::parse::PARSE_VALUE_TYPE_INT, &dan_wp->dungeons[no].enemies[i].pos);
            spm::parse::parsePopNext();
            i++;
        }
        dan_wp->dungeons[no].enemyCount = i;

        // Move to next dungeon
        spm::parse::parsePopNext();
    }

    // Free pit text
    spm::parse::parsePop();
    if (useCustomText)
        spm::filemgr::fileFree(file);
    else
        delete decompPitText;

    return EVT_RET_CONTINUE;
}

static bool isLastEnemy(spm::npcdrv::NPCEntry * npc)
{
        spm::npcdrv::NPCWork * wp = spm::npcdrv::npcGetWorkPtr();
        for (s32 i = 0; i < wp->num; i++)
        {
            spm::npcdrv::NPCEntry * otherNpc = wp->entries + i;

            // Ignore this npc
            if (otherNpc == npc)
                continue;
            
            // False if any other alive
            if (otherNpc->flag8 & 1)
                return false;
        }

        // True if no others found alive
        return true;
}

static s32 getKeyItemId()
{
    if (spm::evtmgr_cmd::evtGetValue(nullptr, GSW(1)) >= 100)
        return spm::item_data::ITEM_ID_KEY_URA_DAN_KEY;
    else
        return spm::item_data::ITEM_ID_KEY_DAN_KEY;
}

static bool isKeyDropped()
{
    s32 key = getKeyItemId();
    return spm::itemdrv::itemCheckForId(key)
        || spm::mario_pouch::pouchCheckHaveItem(key);
}

static void (*npcDropItemReal)(spm::npcdrv::NPCEntry *, s32, s32);
static s32 (*evt_dan_decide_key_enemy_real)(EvtEntry *, bool);

static void handleKeyDrop(spm::npcdrv::NPCEntry * npc, s32 itemType, s32 coinCount)
{
    if (gSettings->lastKey && isPitEnemyRoom() && isLastEnemy(npc) && !isKeyDropped())
        itemType = getKeyItemId();

    npcDropItemReal(npc, itemType, coinCount);
}

static s32 handleKeyAssign(EvtEntry * entry, bool isFirstCall)
{
    // Key is manually dropped later in lastKey mode
    if (gSettings->lastKey)
        return EVT_RET_CONTINUE;
    else
        return evt_dan_decide_key_enemy_real(entry, isFirstCall);
}

void customPitPatch()
{
    writeBranch(spm::dan::evt_dan_read_data, 0, custom_evt_dan_read_data);
    evt_dan_decide_key_enemy_real = patch::hookFunction(spm::dan::evt_dan_decide_key_enemy, handleKeyAssign);
    npcDropItemReal = patch::hookFunction(spm::npcmisc::npcDropItem, handleKeyDrop);
}

}
