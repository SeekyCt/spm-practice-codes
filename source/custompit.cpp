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
#include <spm/rel/dan.h>
#include <wii/DVDFS.h>
#include <wii/string.h>

namespace mod {

using spm::evtmgr::EvtEntry;
using spm::dan::DanWork;
using spm::dan::DanDungeon;
using spm::dan::danWp;
using spm::lz_embedded::pitText;

// Re-implementation of decompiled function
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
            const wii::RGBA errorColour = {0xff, 0, 0, 0xff};
            if (wii::DVDFS::DVDConvertPathToEntrynum(CUSTOM_PIT_FILENAME) != -1)
                useCustomText = true;
            else
                ConsoleWindow::sInstance->push("ERROR: " CUSTOM_PIT_FILENAME " not found.",
                                               nullptr, &errorColour);
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

        // Read item id (chest contents in chest rooms, null & unused elsewhere)
        char itemName[64];
        spm::parse::parseTagGet1("<item>", spm::parse::PARSE_VALUE_TYPE_STRING, itemName);
        danWp->dungeons[no].item = spm::itemdrv::itemTypeNameToId(itemName);

        // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
        spm::parse::parseTagGet1("<map>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].map);

        // Read doors
        while (spm::parse::parsePush("<door>"))
        {
            spm::parse::parseTagGet1("<enter>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].doors[i].enter);
            spm::parse::parseTagGet1("<exit>", spm::parse::PARSE_VALUE_TYPE_INT, &danWp->dungeons[no].doors[i].exit);
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

void customPitPatch()
{
    writeBranch(spm::dan::evt_dan_read_data, 0, custom_evt_dan_read_data);
}

}
