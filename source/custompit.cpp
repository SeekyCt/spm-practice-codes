#include "mod_ui_base/colours.h"
#include "consolewindow.h"
#include "custompit.h"
#include "nandsettings.h"
#include "patch.h"

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/filemgr.h>
#include <spm/lzss10.h>
#include <spm/lz_embedded.h>
#include <spm/memory.h>
#include <spm/itemdrv.h>
#include <spm/parse.h>
#include <spm/system.h>
#include <spm/rel/dan.h>
#include <wii/DVDFS.h>
#include <wii/string.h>

namespace mod {

using spm::evtmgr::EvtEntry;
using spm::dan::DanWork;
using spm::dan::DanDungeon;
using spm::dan::danWp;
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
            if (wii::DVDFS::DVDConvertPathToEntrynum(CUSTOM_PIT_FILENAME) != -1)
                useCustomText = true;
            else
                ConsoleWindow::push("ERROR: " CUSTOM_PIT_FILENAME " not found.", &colours::red);
        }

        // Allocate work
        danWp = new (spm::memory::Heap::MAP) DanWork;
        wii::string::memset(danWp, 0, sizeof(*danWp));
        danWp->dungeons = new (spm::memory::Heap::MAP) DanDungeon[DUNGEON_MAX];
        wii::string::memset(danWp->dungeons, 0, sizeof(DanDungeon[DUNGEON_MAX]));
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
        u32 size = spm::lzss10::lzss10GetDecompSize(pitText); // GCC can't handle lzss10ParseHeader
        decompPitText = new char[size];
        spm::lzss10::lzss10Decompress(pitText, decompPitText);
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
        if (wii::string::strcmp(itemName, "ITEM_ID_NULL") != 0 && wii::string::strcmp(itemName, "NULL") != 0)
            assertf(itemId != 0, "Invalid item name [%s]", itemName);
        danWp->dungeons[no].item = itemId;

        // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
        spm::parse::parseTagGet1("<map>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].map);

        // Read doors
        while (spm::parse::parsePush("<door>"))
        {
            s32 enter, exit;
            spm::parse::parseTagGet1("<enter>", spm::parse::PARSE_VALUE_TYPE_INT, &enter);
            assertf(1 <= enter && enter <= 32, "Invalid enter door id %d", enter);
            danWp->dungeons[no].doors[i].enter = enter;
            spm::parse::parseTagGet1("<exit>", spm::parse::PARSE_VALUE_TYPE_INT, &exit);
            assertf(1 <= exit && exit <= 32, "Invalid exit door id %d", exit);
            danWp->dungeons[no].doors[i].exit = exit;
            spm::parse::parsePopNext();
            i++;
        }
        danWp->dungeons[no].doorCount = i;

        // Read enemies
        i = 0;
        while (spm::parse::parsePush("<enemy>"))
        {
            spm::parse::parseTagGet1("<name>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].enemies[i].name);
            spm::parse::parseTagGet1("<num>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].enemies[i].num);
            spm::parse::parseTagGet1("<pos>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].enemies[i].pos);
            spm::parse::parsePopNext();
            i++;
        }
        danWp->dungeons[no].enemyCount = i;

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

static int (*evt_dan_decide_key_enemy_real)(EvtEntry *, bool);
static int lastKeyPatch(EvtEntry * entry, bool isFirstCall)
{
    if (gSettings->lastKey)
    {
        // Not assigning the key to an enemy makes it spawn 
        // on the door once all enemies are defeated
        return EVT_RET_CONTINUE;
    }
    else
    {
        return evt_dan_decide_key_enemy_real(entry, isFirstCall);
    }
}

void customPitPatch()
{
    writeBranch(spm::dan::evt_dan_read_data, 0, custom_evt_dan_read_data);
    evt_dan_decide_key_enemy_real = patch::hookFunction(spm::dan::evt_dan_decide_key_enemy, lastKeyPatch);
}

}
