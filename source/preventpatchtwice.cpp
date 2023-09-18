#include <msl/string.h>
#include <spm/evt_msg.h>
#include <spm/nandmgr.h>
#include <spm/seq_load_sub.h>
#include <spm/spmario_snd.h>
#include <spm/wpadmgr.h>
#include <wii/wpad.h>

#include "patch.h"
#include "preventpatchtwice.h"

namespace mod {

#define SEQ_LOAD_STATE_REL_LOADER_TWICE 0x200

EVT_BEGIN(evt_rel_loader_twice)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<system><p>You have already executed\nthe REL Loader.\n<k>\n<o>"), 0, 0)
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    RETURN()
EVT_END()

s32 relLoaderTwiceEvtId = 0;
static s32 (*loadMainReal)();
void loadMainPatch()
{
    loadMainReal = patch::hookFunction(spm::seq_load_sub::loadMain,
        []()
        {
            if (!spm::evtmgr::evtCheckID(relLoaderTwiceEvtId) && spm::seq_load_sub::seq_load_sub_wp->state == SEQ_LOAD_STATE_REL_LOADER_TWICE) {
                spm::seq_load_sub::seq_load_sub_wp->state = 0xa;
            }
            if (spm::seq_load_sub::seq_load_sub_wp->state == 0xa && spm::wpadmgr::wpadGetButtonsPressed(0) & WPAD_BTN_2)
            {
                if (msl::string::strcmp(spm::nandmgr::nandmgr_wp->saves[spm::seq_load_sub::seq_load_sub_wp->selectedSave].spmarioGlobals.saveName, "REL Loader") == 0)
                {
                    spm::spmario_snd::spsndSFXOn("SFX_SYS_SELECT_NG1");

                    spm::seq_load_sub::seq_load_sub_wp->state = SEQ_LOAD_STATE_REL_LOADER_TWICE;
                    relLoaderTwiceEvtId = spm::evtmgr::evtEntry(evt_rel_loader_twice, 0, 0)->id;
                }
                
            }
            return loadMainReal();
        }
    );
}

}
