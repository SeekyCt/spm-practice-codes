#include "util.h"

#include <spm/evtmgr.h>
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
    spm::mario::marioKeyOff();
    spm::spmario::spmarioSystemLevel(1);
    return 2;
}

int evt_unfreeze_game(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    spm::spmario::spmarioSystemLevel(0);
    spm::mario::marioKeyOn();
    return 2;
}
