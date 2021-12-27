#include "util.h"

#include <spm/camdrv.h>
#include <spm/evtmgr_cmd.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/spmario.h>
#include <wii/DVDFS.h>
#include <wii/OSModule.h>
#include <wii/string.h>

namespace mod {

int evt_get_cur_pixl(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    (void) firstRun;
    
    int pixl = spm::mario_pouch::pouchGetCurPixl();
    spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], pixl);
    return 2;
}

int evt_freeze_game(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    (void) entry;
    (void) firstRun;

    // Freeze player
    spm::mario::marioKeyOff();

    // Freeze world
    spm::spmario::spmarioSystemLevel(1);
    return 2;
}

int evt_unfreeze_game(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    (void) entry;
    (void) firstRun;
    
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

void * getModRelLoadAddr()
{
    // Assume the first rel loaded with a module id other than 1 is this
    wii::OSModule::RelHeader * curRel = wii::OSModule::firstRel;
    while (curRel->id == 1)
        curRel = curRel->next;
    
    return reinterpret_cast<void *>(curRel);
}

const char * getToggleName(bool value)
{
    switch (value)
    {
        case true:
            return "Enabled";
        case false:
            return "Disabled";
    }
}

s32 pow(s32 val, s32 power)
{
    s32 x = 1;
    for (s32 i = power; i > 0; i--)
        x *= val;
    return x;
}

bool check3d()
{
    spm::camdrv::CamEntry * cam = spm::camdrv::camGetPtr(spm::camdrv::CAM_3D);
    return !cam->isOrtho;
}

char * cloneString(const char * str)
{
    size_t len = wii::string::strlen(str);
    char * newStr = new char[len + 1]; // include terminating null
    wii::string::strcpy(newStr, str);
    return newStr;
}

s32 strcount(const char * str, char c)
{
    u32 n = 0;
    for (const char * p = str; *p; p++)
    {
        if (*p == c)
            n++;
    }
    return n;
}

bool fileExists(const char * path)
{
    return wii::DVDFS::DVDConvertPathToEntrynum(path) != -1;
}

bool isPitEnemyRoom()
{
    return wii::string::strncmp(spm::spmario::gp->mapName, "dan_0", 5) == 0  // Flipside 
        || wii::string::strncmp(spm::spmario::gp->mapName, "dan_4", 5) == 0; // Flopside
}

}
