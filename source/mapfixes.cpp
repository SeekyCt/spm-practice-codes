#include <common.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <string.h>

#include "mapfixes.h"

namespace mod {

void mapLoadCrashFixes(spm::seqdrv::SeqWork * wp)
{
    // If you change maps by locking the seq_mapChangeWork values in DME this will crash the game; adding a nullptr check fixes this
    if (wp->p0 == nullptr || wp->p1 == nullptr)
        return;

    // Fix crash when entering Francis's room with sequence position lower than 124
    if (strcmp(wp->p0, "ta4_13") == 0 && strcmp(wp->p1, "K_doa_L") == 0)
    {
        if (spm::spmario::gp->gsw0 < 124)
            spm::spmario::gp->gsw0 = 124;
    }

    // Fix crash when entering Dimension D with sequence position between 3-3 Dimentio and 5-2 Chunks
    if (strcmp(wp->p0, "bos_01") == 0)
    {
        if (spm::spmario::gp->gsw0 > 116 && spm::spmario::gp->gsw0 < 179)
            spm::spmario::gp->gsw0 = 180;
    }
    
    // Fix hang when entering credits with music playing
    if (strcmp(wp->p0, "aa3_01") == 0 && strcmp(wp->p1, "epi_01") == 0)
    {
        // Stop music like before the actual credits warp
        if (spm::spmario_snd::spsndCheckBgmPlaying(0))
            spm::spmario_snd::spsndBGMOff_f_d(0, 4000);
    }
}

}