#include "util.h"

#include <spm/evtmgr_cmd.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/spmario.h>

int evt_get_cur_pixl(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    int pixl = spm::mario_pouch::pouchGetCurPixl();
    spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], pixl);
    return 2;
}

int evt_freeze_game(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // Freeze player
    spm::mario::marioKeyOff();

    // Freeze world
    spm::spmario::spmarioSystemLevel(1);
    return 2;
}

int evt_unfreeze_game(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    // Unfreeze world
    spm::spmario::spmarioSystemLevel(0);

    // Unfreeze player
    spm::mario::marioKeyOn();
    return 2;
}

void updateSaveChecksum(spm::nandmgr::SaveFile * save)
{
    // Initialise fields for calculation
    save->checksum = 0;
    save->checksumNOT = ~save->checksum;

    // Calculate new checksum
    u32 checksum = 0;
    u8 * data = reinterpret_cast<u8 *>(save);
    for (u32 i = 0; i < sizeof(*save); i++)
        checksum += data[i];
    
    // Update save
    save->checksum = checksum;
    save->checksumNOT = ~checksum;
}

const char * getGameRegion()
{
    char * regionId = (char *) 0x80000003;
    switch(*regionId)
    {
        case 'P':
            return "PAL";
        case 'E':
            return "NTSC-U";
        case 'J':
            return "NTSC-J";
        case 'K':
            return "NTSC-K";
        default:
            return "Invalid Region";
    }
}

int getGameRevision()
{
    u8 * revision = (u8 *) 0x80000007;
    return *revision;
}
