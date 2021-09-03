#pragma once

#include <types.h>
#include <spm/hitdrv.h>

namespace spm::casedrv {

struct CaseEntry
{
    u8 unknown_0x0[0xdc - 0x0];
};
static_assert(sizeof(CaseEntry) == 0xdc);

extern "C" {

CaseEntry * caseCheckHitObj(spm::hitdrv::HitObj * hitObj);

}

}
