#include "util.h"

#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/mario_pouch.h>

int evt_get_cur_pixl(spm::evtmgr::EvtEntry * entry, bool firstRun)
{
    int pixl = spm::mario_pouch::pouchGetCurPixl();
    spm::evtmgr_cmd::evtSetValue(entry, entry->pCurData[0], pixl);
    return 2;
}
